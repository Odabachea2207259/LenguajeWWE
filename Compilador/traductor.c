/**
 * @mainpage Compilador-Traductor de Lenguaje WWE
 * 
 * Este proyecto implementa un traductor para un lenguaje propio
 * a c, para despues realizar la compilación y regresar un ejectuable,
 * funcionando como cualquier compilador
 * 
 * ##Contenido principal
 * -Analis léxico
 * -Parser/Análisis semántico
 * -Creación de enunciados
 * -Traduccion
 * -Compilacion de archivo y retorno de ejecutable
 * 
 * ##Autores
 * -Alexander Arellano Odabachea
 * 
 * @author Alexander Arellano Odabachea
 */
/**
 * @file traductor.c
 * @brief Funciones de la escritura a archivo
 * 
 * @author Alexander Arellano Odabachea
 */
#include "SymbolTable.h"
#include "Enunciados.h"

void traductor(Programa *programa,FILE *fptr);

/**
 * @defgroup funciones_escritura Funciones de escritura a archivo
 * @{
 */
void escribirPrograma(Programa* programa, FILE* fptr);
void escribirValor(Valor val,FILE *fptr);
void escribirOperador(OPRelacional val,FILE *fptr);
void escribirExpresion(Expresion* expr,FILE *fptr);
void escribirEnunciado(Enunciado* e, FILE *fptr);
/**@}*/

int main(int argc, char *argv[])
{
	Tokens tokens;
	inicializarTokens(&tokens);

	if(argc != 4 || strcmp(argv[2],"-o") != 0)
	{
		printf("No ingreso el nombre del archivo\n");
		return 0;
	}

	char* filename = argv[1];
	char* archivo_salida = argv[3];
	char archivo_c[256];
	snprintf(archivo_c,sizeof(archivo_c), "%s_temp.c",archivo_salida);

	lexerAnalysis(filename,&tokens);

	if(!parser(filename,&tokens))
	{
		printf("El programa no esta correcto\n");
		vaciarPilaCompleto(&copia);
		borrarLista(&tokens);
		return 0;
	}
	
	//printTabla();
	
	Programa *programa = (Programa*)calloc(1,sizeof(Programa));
	programa->lista_enunciados = NULL;
	programa->ultimo_enunciado = NULL;
	
	tokens.actual = tokens.inicio;
	
	crearEnunciados(programa,&tokens);
	//imprimirPrograma(programa);

	FILE *fptr = fopen(archivo_c,"w");
	traductor(programa,fptr);


	char comando_gcc[512];
	snprintf(comando_gcc,sizeof(comando_gcc),"gcc %s Listas.c -o %s",archivo_c,archivo_salida);

	int res = system(comando_gcc);
	if(res != 0)
	{
		printf("Error al compilar\n");
		vaciarPilaCompleto(&copia);
		borrarLista(&tokens);
		eliminarPrograma(programa);
		free(programa);
		remove(archivo_c);
		return 0;
	}

	remove(archivo_c);
	vaciarPilaCompleto(&copia);
	borrarLista(&tokens);
	eliminarPrograma(programa);
	free(programa);
	return 0;
}

/**
 * @brief Traductor
 * @ingroup funciones_compilador
 * 
 * Método que se encarga de escribir en un archivo nuevo lo esencial para el archivo .c
 * 
 * @param programa Lista de enunciados completa
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void traductor(Programa* programa,FILE *fptr)
{
	fprintf(fptr,"#include \"Listas.h\"\n\n");
	fprintf(fptr,"int main(){\n");

	escribirPrograma(programa,fptr);
	fprintf(fptr,"return 0;\n}\n");
	fclose(fptr);
}

/**
 * @brief Escribir enunciado
 * @ingroup funciones_escritura
 * 
 * Método que compara el tipo de enunciado actual y escribe lo necesario en el archivo .c
 * 
 * @param e Enunciado a revisar
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void escribirEnunciado(Enunciado* e, FILE *fptr) {
    while (e != NULL) {
        switch (e->tipo) {
            case ENUNC_DECLARACION:
			    for (int i = 0; i < e->declaracion.cantidad; i++) {
					fprintf(fptr,"float %s;\n",e->declaracion.variables[i]);
                }
                break;
            case ENUNC_DECLARACION_LISTA:
                //printf("Declaración de %d variables:\n", e->declaracion.cantidad);
                for (int i = 0; i < e->declaracion.cantidad; i++) {
					fprintf(fptr,"Lista %s;\n",e->declaracion.variables[i]);
					fprintf(fptr,"inicializarArray(&%s);\n",e->declaracion.variables[i]);
                }
                break;

            case ENUNC_ASIGNACION:
				//fprintf(fptr,"%s = ",e->asignacion.variable);
				escribirValor(e->asignacion.variable,fptr);
				fprintf(fptr," = ");
				escribirExpresion(e->asignacion.expresion,fptr);
				fprintf(fptr,";\n");
				break;
            case ENUNC_ASIGNACION_LISTA:
				fprintf(fptr,"pushLista(&%s,",e->asignacion.variable.nombre);
                escribirExpresion(e->asignacion.expresion,fptr);
				fprintf(fptr,");\n");
                break;

            case ENUNC_LEER:
				fprintf(fptr,"scanf(\"%%f\",&%s);\n",e->leer.variable);
                break;
            case ENUNC_LEER_LISTA:
				fprintf(fptr,"float temp;\n");
				fprintf(fptr,"scanf(\"%%f\",&temp);\n");
				fprintf(fptr,"pushLista(&%s,temp);\n",e->leer.variable);
                break;
            case ENUNC_ANUNCIAR:
                if (e->anunciar.esCadena) {
					fprintf(fptr,"printf(%s);\n",e->anunciar.cadena);
                } else {
					fprintf(fptr,"printf(\"%%.2f\",");
                    escribirExpresion(e->anunciar.expresion,fptr);
					fprintf(fptr,");\n");
                }
                break;

            case ENUNC_SI:
				fprintf(fptr,"if(");
                escribirValor(e->si.comp.izquierda,fptr);
				escribirOperador(e->si.comp.operador,fptr);
                escribirValor(e->si.comp.derecha,fptr);
				fprintf(fptr,"){\n");
                escribirPrograma(e->si.entonces,fptr);
                if(e->si.si_no) {
					fprintf(fptr,"}else{\n");
                    escribirPrograma(e->si.si_no,fptr);
                }
				fprintf(fptr,"}\n");
                break;

            case ENUNC_MIENTRAS:
				fprintf(fptr,"while(");
                escribirValor(e->mientras.comp.izquierda,fptr);
				escribirOperador(e->mientras.comp.operador,fptr);
                escribirValor(e->mientras.comp.derecha,fptr);
				fprintf(fptr,"){\n");
                escribirPrograma(e->mientras.cuerpo,fptr);
				fprintf(fptr,"}\n");
                break;

            default:
                printf("Enunciado no reconocido.\n");
                break;
        }
        e = e->siguiente;
    }
}

/**
 * @brief Escribir expresion
 * @ingroup funciones_escritura
 * 
 * Método que compara el tipo de expresión actual y escribe lo necesario en el archivo .c
 * 
 * @param expr Expresión a revisar
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void escribirExpresion(Expresion* expr,FILE *fptr) {
    if (!expr) return;
    if (expr->tipo == EXPR_VALOR) {
        escribirValor(expr->valor,fptr);
    } else if (expr->tipo == EXPR_BINARIA) {
    	if(!strcmp(expr->binaria.oparitmetico,"%"))
		fprintf(fptr,"(int)(");
        escribirExpresion(expr->binaria.izquierda,fptr);
    	if(!strcmp(expr->binaria.oparitmetico,"%"))
		fprintf(fptr,")");
        fprintf(fptr," %s ", expr->binaria.oparitmetico);
    	if(!strcmp(expr->binaria.oparitmetico,"%"))
		fprintf(fptr,"(int)(");
        escribirExpresion(expr->binaria.derecha,fptr);
    	if(!strcmp(expr->binaria.oparitmetico,"%"))
		fprintf(fptr,")");
    }
}

/**
 * @brief Escribir valor
 * @ingroup funciones_escritura
 * 
 * Método que compara el tipo de valor actual y escribe lo necesario en el archivo .c
 * 
 * @param val Valor a revisar
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void escribirValor(Valor val,FILE *fptr) {
        switch (val.tipo) {
            case VAL_VARIABLE:
                fprintf(fptr,"%s", val.nombre);
                break;
            case VAL_NUMERO:
                fprintf(fptr,"%.2f", val.numero);
                break;
            case VAL_LISTA_INDEX:
                fprintf(fptr,"%s.arr[%d]", val.nombre, val.indice);
                break;
			case VAL_LISTA_INDEX_VAR:
				fprintf(fptr,"%s.arr[(int)%s]",val.nombre,val.var);
				break;
            case VAL_LISTA_F:
                fprintf(fptr,"getPrimero(&%s)",val.nombre);
                break;
            case VAL_LISTA_L:
                fprintf(fptr,"getUltimo(&%s)",val.nombre);
                break;
            case VAL_LISTA_S:
                fprintf(fptr,"getSize(&%s)",val.nombre);
                break;
            default:
                printf("Valor desconocido");
        }
}

/**
 * @brief Escribir Operador
 * @ingroup funciones_escritura
 * 
 * Método que compara el tipo de operador relacional actual y escribe lo necesario en el archivo .c
 * 
 * @param val Operador relacional a revisar
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void escribirOperador(OPRelacional val,FILE *fptr) {
        switch (val) {
            case OP_DIFERENTE:
                fprintf(fptr,"!=");
                break;
            case OP_IGUAL:
                fprintf(fptr,"==");
                break;
            case OP_MAYOR:
                fprintf(fptr,">");
                break;
            case OP_MAYOR_IGUAL:
                fprintf(fptr,">=");
                break;
            case OP_MENOR:
                fprintf(fptr,"<");
                break;
            case OP_MENOR_IGUAL:
                fprintf(fptr,"<=");
                break;
            default:
                printf("Valor desconocido");
        }
}

/**
 * @brief Escribir Programa
 * @ingroup funciones_escritura
 * 
 * Método que va mandando enunciado por enunciado y escribe los free necesarios al finalizar las listas.
 * 
 * @param programa Lista de enunciados completa
 * @param fptr Apuntador al archivo nuevo a escribir
 */
void escribirPrograma(Programa* programa, FILE* fptr) {
    //printf("== Programa ==\n");
    escribirEnunciado(programa->lista_enunciados,fptr);

	if(programa->listas){
		for(int i = 0; i < programa->cantidad;i++)
		{
			fprintf(fptr,"free(%s.arr);\n",programa->variables[i]);
		}
	}
}
