/**
 * @file Enunciados.c
 * @brief Implementación de creación de enunciados.
 *
 * Revisa la lista de tokens completa y crea
 * enunciado por enunciado depende la situación.
 *
 * @author Alexander Arellano Odabachea
 */

#include "Lexer.h"
#include "SymbolTable.h"
#include "Enunciados.h"

/**
 * @brief Crea un enunciado.
 * @ingroup funciones_enunciados
 *
 * Revisa cada token para ir creando los enunciados necesarios.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

void crearEnunciados(Programa* programa, Tokens* tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"INICIO-PROGRAMA"))
        tokens->actual = tokens->actual->next;
    
    while(esEnunciado(programa,tokens));
}

/**
 * @brief Revisa cada enunciado.
 * @ingroup funciones_enunciados
 *
 * Compara cada token para crear el enuncuado necesario.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int esEnunciado(Programa* programa, Tokens* tokens)
{
    if(tokens->actual == NULL)
        return 0;
    
    if(!strcmp(tokens->actual->token->tipoToken,"FIN-LINEA"))
    {
        tokens->actual = tokens->actual->next;
        return 1;
    }
    
    //printf("Token: %s\n",tokens->actual->token->cadena);
    Nodo *prev = tokens->actual;

    if(inicializar(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(inicializarLista(programa,tokens))
    {
        return 1;
    }
    tokens->actual = prev;

    if(leer(programa,tokens))
    {
        return 1;
    }
    tokens->actual = prev;

    if(asignacion_trad(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(asignacion_lista(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(si_trad(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(mientras_trad(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(anunciar_trad(programa,tokens))
    {
        return 1;
    }

    tokens->actual = prev;

    if(!strcmp(tokens->actual->token->tipoToken,"SINO") || !strcmp(tokens->actual->token->tipoToken,"FIN-SI") || !strcmp(tokens->actual->token->tipoToken,"FIN-MIENTRAS"))
    {
        return 0;
    }

    tokens->actual = tokens->actual->next;
}

/**
 * @brief Token Inicializar
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado de la inicializacion de variables
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int inicializar(Programa* programa, Tokens* tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"INICIALIZAR")) {
        tokens->actual = tokens->actual->next;

        Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
        nuevo->tipo = ENUNC_DECLARACION;

        nuevo->declaracion.variables = NULL;
        nuevo->declaracion.cantidad = 0; 

        while (tokens->actual && !strcmp(tokens->actual->token->tipoToken,"VARIABLE")) { 
            char* variable = strdup(tokens->actual->token->cadena);

            nuevo->declaracion.cantidad++;
            nuevo->declaracion.variables = (char**)realloc(nuevo->declaracion.variables,nuevo->declaracion.cantidad * sizeof(char*));

            nuevo->declaracion.variables[nuevo->declaracion.cantidad - 1] = variable;

            tokens->actual = tokens->actual->next;
        }

        pushEnunciado(programa,nuevo);
        return 1;
    }
    return 0;
}

/**
 * @brief Token InicializarLista
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado de la inicializacion de variables tipo lista
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int inicializarLista(Programa* programa, Tokens* tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"NEXUS")) {
        tokens->actual = tokens->actual->next;
        programa->listas = 1;

        Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
        nuevo->tipo = ENUNC_DECLARACION_LISTA;

        nuevo->declaracion.variables = NULL;
        nuevo->declaracion.cantidad = 0; 

        while (tokens->actual && !strcmp(tokens->actual->token->tipoToken,"VARIABLE")) { 
            char* variable = strdup(tokens->actual->token->cadena);

            nuevo->declaracion.cantidad++;
            nuevo->declaracion.variables = (char**)realloc(nuevo->declaracion.variables,nuevo->declaracion.cantidad * sizeof(char*));

            nuevo->declaracion.variables[nuevo->declaracion.cantidad - 1] = variable;

            tokens->actual = tokens->actual->next;
        }

        programa->variables = nuevo->declaracion.variables;
        programa->cantidad = nuevo->declaracion.cantidad;

        pushEnunciado(programa,nuevo);
        return 1;
    }

    return 0;
}

/**
 * @brief Token leer
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo leer para la lectura de variables desde el teclado.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int leer(Programa* programa, Tokens *tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE"))
    {
        Nodo *var = tokens->actual;
        tokens->actual = tokens->actual->next;

        if(!strcmp(tokens->actual->token->tipoToken,"IGUAL"))
        {
            tokens->actual = tokens->actual->next;
            if(!strcmp(tokens->actual->token->tipoToken,"PROMO"))
            {
                Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
                nuevo->tipo = ENUNC_LEER;

                nuevo->leer.variable = strdup(var->token->cadena);
                tokens->actual = tokens->actual->next;
                pushEnunciado(programa,nuevo);
                return 1;
            }
            return 0;
        }
        else if(!strcmp(tokens->actual->token->tipoToken,"FIN-ANUNCIAR"))
        {
            tokens->actual = tokens->actual->next;
            if(!strcmp(tokens->actual->token->tipoToken,"PROMO"))
            {
                Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
                nuevo->tipo = ENUNC_LEER_LISTA;
                
                //strcpy(nuevo->leer.variable,var->token->cadena);
                nuevo->leer.variable = strdup(var->token->cadena);
                tokens->actual = tokens->actual->next;
                pushEnunciado(programa,nuevo);
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

/**
 * @brief Token Asignacion
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo asignacion para la asignacion a variables.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int asignacion_trad(Programa* programa, Tokens* tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE"))
    {
        int esLista = 0;
        Nodo *var = tokens->actual;
        Valor valor;

        tokens->actual = tokens->actual->next;
        if(!strcmp(tokens->actual->token->tipoToken,"LBRACKET"))
        {
            esLista = 1;
            valor.tipo = VAL_LISTA_INDEX;
            valor.nombre = strdup(var->token->cadena);
            tokens->actual = tokens->actual->next;
            if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE"))
            {
                valor.tipo = VAL_LISTA_INDEX_VAR;
                valor.var = strdup(tokens->actual->token->cadena);
            }
            else
                valor.indice = atof(tokens->actual->token->cadena);
            tokens->actual = tokens->actual->next;
            tokens->actual = tokens->actual->next;
        }

        if(!strcmp(tokens->actual->token->tipoToken,"IGUAL"))
        {
            Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
            nuevo->tipo = ENUNC_ASIGNACION;
            
            if(!esLista){
                valor.tipo = VAL_VARIABLE;
                valor.nombre = strdup(var->token->cadena);
            }

            nuevo->asignacion.variable = valor;
            tokens->actual = tokens->actual->next;

            Expresion* expresion = (Expresion*)calloc(1,sizeof(Expresion));

            Valor valorExpr = esValor(programa,tokens);

            if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
            {
                expresion->tipo = EXPR_BINARIA;
                expresion->binaria.izquierda = (Expresion*)calloc(1,sizeof(Expresion));
                expresion->binaria.izquierda->tipo = EXPR_VALOR;
                expresion->binaria.izquierda->valor = valorExpr;

                expresion->binaria.oparitmetico = strdup(tokens->actual->token->cadena);
                tokens->actual = tokens->actual->next;

                //Checar si es expresion derecha
                expresion->binaria.derecha = (Expresion*)calloc(1,sizeof(Expresion));
                expDerecha(programa,tokens,expresion->binaria.derecha);
            }
            else{
                expresion->tipo = EXPR_VALOR;
                expresion->valor = valorExpr;
            }

            nuevo->asignacion.expresion = expresion;

            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);

            return 1;
        }

        if(esLista)
        {
            free(valor.nombre);
        }

        return 0;
    }

    return 0;
}

/**
 * @brief Token AsignacionLista
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo asignacion para la asignacion a variables tipo lista.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */

int asignacion_lista(Programa* programa, Tokens *tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE"))
    {
        Nodo *var = tokens->actual;

        tokens->actual = tokens->actual->next;

        if(!strcmp(tokens->actual->token->tipoToken,"FIN-ANUNCIAR"))
        {
            Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
            nuevo->tipo = ENUNC_ASIGNACION_LISTA;
            
            nuevo->asignacion.variable.tipo = VAL_VARIABLE;
            nuevo->asignacion.variable.nombre = strdup(var->token->cadena);
            tokens->actual = tokens->actual->next;

            Expresion* expresion = (Expresion*)calloc(1,sizeof(Expresion));

            Valor valor = esValor(programa,tokens);

            if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
            {
                expresion->tipo = EXPR_BINARIA;
                expresion->binaria.izquierda = (Expresion*)calloc(1,sizeof(Expresion));
                expresion->binaria.izquierda->tipo = EXPR_VALOR;
                expresion->binaria.izquierda->valor = valor;

                expresion->binaria.oparitmetico = strdup(tokens->actual->token->cadena);
                tokens->actual = tokens->actual->next;

                //Checar si es expresion derecha
                expresion->binaria.derecha = (Expresion*)calloc(1,sizeof(Expresion));
                expDerecha(programa,tokens,expresion->binaria.derecha);
            }
            else{
                expresion->tipo = EXPR_VALOR;
                expresion->valor = valor;
            }

            nuevo->asignacion.expresion = expresion;

            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);

            return 1;
        }
    }
    return 0;
}
/**
 * @brief Token Si
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo si.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */
int si_trad(Programa *programa,Tokens *tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"SI"))
    {
        Enunciado *nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
        nuevo->tipo = ENUNC_SI;

        tokens->actual = tokens->actual->next;

        nuevo->si.comp.izquierda = esValor(programa,tokens);
        nuevo->si.comp.operador = tipoOperador(tokens->actual->token->cadena);

        tokens->actual = tokens->actual->next;
        nuevo->si.comp.derecha = esValor(programa,tokens);


        nuevo->si.entonces = (Programa*)calloc(1,sizeof(Programa));
        while(esEnunciado(nuevo->si.entonces,tokens));

        if(!strcmp(tokens->actual->token->tipoToken,"SINO"))
        {
            tokens->actual = tokens->actual->next;
            nuevo->si.si_no = (Programa*)calloc(1,sizeof(Programa));
            while(esEnunciado(nuevo->si.si_no,tokens));
        }
        
        if(!strcmp(tokens->actual->token->tipoToken,"FIN-SI"))
        {
            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);

            return 1;
        }
    }

    return 0;
}
/**
 * @brief Token Mientras
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo mientras.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */
int mientras_trad(Programa *programa,Tokens *tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"MIENTRAS"))
    {
        Enunciado *nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
        nuevo->tipo = ENUNC_MIENTRAS;

        tokens->actual = tokens->actual->next;

        nuevo->mientras.comp.izquierda = esValor(programa,tokens);
        nuevo->mientras.comp.operador = tipoOperador(tokens->actual->token->cadena);

        tokens->actual = tokens->actual->next;
        nuevo->mientras.comp.derecha = esValor(programa,tokens);


        nuevo->mientras.cuerpo = (Programa*)calloc(1,sizeof(Programa));
        while(esEnunciado(nuevo->mientras.cuerpo,tokens));
        
        if(!strcmp(tokens->actual->token->tipoToken,"FIN-MIENTRAS"))
        {
            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);

            return 1;
        }
    }

    return 0;
}
/**
 * @brief Token Anunciar
 * @ingroup funciones_enunciados
 *
 * Crea un enunciado del tipo anunciar.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 *
 */
int anunciar_trad(Programa *programa,Tokens *tokens)
{
    if(!strcmp(tokens->actual->token->tipoToken,"ANUNCIAR"))
    {
        Enunciado* nuevo = (Enunciado*)calloc(1,sizeof(Enunciado));
        nuevo->tipo = ENUNC_ANUNCIAR;

        tokens->actual = tokens->actual->next;
        tokens->actual = tokens->actual->next;

        if(!strcmp(tokens->actual->token->tipoToken,"STRING"))
        {
            nuevo->anunciar.esCadena = 1;
            nuevo->anunciar.cadena = strdup(tokens->actual->token->cadena);
            tokens->actual = tokens->actual->next;
            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);
            return 1;
        }
        else{
            nuevo->anunciar.esCadena = 0;
            Expresion* expresion = (Expresion*)calloc(1,sizeof(Expresion));

            Valor valor = esValor(programa,tokens);

            if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
            {
                expresion->tipo = EXPR_BINARIA;
                expresion->binaria.izquierda = (Expresion*)calloc(1,sizeof(Expresion));
                expresion->binaria.izquierda->tipo = EXPR_VALOR;
                expresion->binaria.izquierda->valor = valor;

                expresion->binaria.oparitmetico = strdup(tokens->actual->token->cadena);
                tokens->actual = tokens->actual->next;

                //Checar si es expresion derecha
                expresion->binaria.derecha = (Expresion*)calloc(1,sizeof(Expresion));
                expDerecha(programa,tokens,expresion->binaria.derecha);
            }
            else{
                expresion->tipo = EXPR_VALOR;
                expresion->valor = valor;
            }

            nuevo->anunciar.expresion = expresion;
            tokens->actual = tokens->actual->next;
            tokens->actual = tokens->actual->next;

            pushEnunciado(programa,nuevo);
            return 1;
        }
    }

    return 0;
}
/**
 * @brief Expresion Derecha
 * @ingroup funciones_tipo
 *
 * Revisa y crea enunciados del tipo expresion hasta que ya no encuentre una.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 * @param derecha Expresion en la cual se va almacenar la nueva/s expresion/es.
 */
void expDerecha(Programa* programa, Tokens* tokens, Expresion *derecha)
{
    Valor valor = esValor(programa,tokens);

    if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
    {
        derecha->tipo = EXPR_BINARIA;
        derecha->binaria.izquierda = (Expresion*)calloc(1,sizeof(Expresion));
        derecha->binaria.izquierda->tipo = EXPR_VALOR;
        derecha->binaria.izquierda->valor = valor;

        derecha->binaria.oparitmetico = strdup(tokens->actual->token->cadena);
        tokens->actual = tokens->actual->next;

        //Recursion hasta encontrar un valor simple
        derecha->binaria.derecha = (Expresion*)calloc(1,sizeof(Expresion));
        expDerecha(programa,tokens,derecha->binaria.derecha);
    }
    else{
        derecha->tipo = EXPR_VALOR;
        derecha->valor = valor;
    }
}
/**
 * @brief EsValor
 * @ingroup funciones_tipo
 *
 * Revisa que tipo de valor es el actual y retorna el tipo.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param tokens Lista de todos los tokens almacenado.
 * @return Valor del token actual.
 */
Valor esValor(Programa* programa, Tokens* tokens)
{
    Valor valor;
    if (!strcmp(tokens->actual->token->tipoToken, "VARIABLE")) {
        valor.tipo = VAL_VARIABLE;
        valor.nombre = strdup(tokens->actual->token->cadena);  // Variable

        tokens->actual = tokens->actual->next;
        if (!strcmp(tokens->actual->token->tipoToken, "LBRACKET")) {

            tokens->actual = tokens->actual->next;
            if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE")){
                valor.tipo = VAL_LISTA_INDEX_VAR;
                valor.var = strdup(tokens->actual->token->cadena);
            }
            else{
                valor.tipo = VAL_LISTA_INDEX;
                valor.indice = atoi(tokens->actual->token->cadena);  // Índice de lista
            }
            tokens->actual = tokens->actual->next;
            tokens->actual = tokens->actual->next;
        }
        else if(!strcmp(tokens->actual->token->tipoToken,"FIRST")){
            valor.tipo = VAL_LISTA_F;
            tokens->actual = tokens->actual->next;
        }
        else if(!strcmp(tokens->actual->token->tipoToken,"LAST")){
            valor.tipo = VAL_LISTA_L;
            tokens->actual = tokens->actual->next;
        }
        else if(!strcmp(tokens->actual->token->tipoToken,"SIZE")){
            valor.tipo = VAL_LISTA_S;
            tokens->actual = tokens->actual->next;
        }
        else {
            valor.tipo = VAL_VARIABLE;
        }

    } else if (!strcmp(tokens->actual->token->tipoToken, "NUM") || !strcmp(tokens->actual->token->tipoToken,"FLOAT")) {
        valor.tipo = VAL_NUMERO;
        valor.numero = atof(tokens->actual->token->cadena);  // Número
        tokens->actual = tokens->actual->next;
    }

    return valor;
}
/**
 * @brief OperadorRelacional
 * @ingroup funciones_tipo
 *
 * Revisa el token actual y retorna el tipo de operador relacional que es.
 *
 * @param operador String el cual se comparara para retornar un operador.
 * @return OPRelacional del token actual.
 */
OPRelacional tipoOperador(char* operador)
{
    if(!strcmp(operador,"<>"))
        return OP_DIFERENTE;
    if(!strcmp(operador,"=="))
        return OP_IGUAL;
    if(!strcmp(operador,">="))
        return OP_MAYOR_IGUAL;
    if(!strcmp(operador,">"))
        return OP_MAYOR;
    if(!strcmp(operador,"<="))
        return OP_MENOR_IGUAL;
    if(!strcmp(operador,"<"))
        return OP_MENOR;
}
/**
 * @brief Push
 * @brief funciones_eliminar
 *
 * Toma un enunciado nuevo y hace push en la pila de enunciados.
 *
 * @param programa Lista de todos los enunciados actuales.
 * @param enunciado Enunciado nuevo que se ingresara en la lista.
 */
void pushEnunciado(Programa* programa, Enunciado* enunciado)
{
    if(programa->lista_enunciados == NULL)
        programa->lista_enunciados = enunciado;
    else   
        programa->ultimo_enunciado->siguiente = enunciado;
    
    programa->ultimo_enunciado = enunciado;
}

/**
 * @brief Imprimir Enunciado
 * @brief funciones_imprimir
 *
 * Compara que tipo de enunciado es y lo va imprimiendo
 *
 * @param enunciado Enunciado nuevo que se ingresara en la lista.
 */
void imprimirEnunciado(Enunciado* e) {
    while (e != NULL) {
        switch (e->tipo) {
            case ENUNC_DECLARACION:
            case ENUNC_DECLARACION_LISTA:
                printf("Declaración de %d variables:\n", e->declaracion.cantidad);
                for (int i = 0; i < e->declaracion.cantidad; i++) {
                    printf("  - %s\n", e->declaracion.variables[i]);
                }
                break;

            case ENUNC_ASIGNACION:
            case ENUNC_ASIGNACION_LISTA:
                printf("Asignación:\n  Variable: ");
                imprimirValor(e->asignacion.variable);
                printf("Expresion: ");
                imprimirExpresion(e->asignacion.expresion);
                printf("\n");
                break;

            case ENUNC_LEER:
            case ENUNC_LEER_LISTA:
                printf("Leer variable: %s\n", e->leer.variable);
                break;

            case ENUNC_ANUNCIAR:
                if (e->anunciar.esCadena) {
                    printf("Anunciar cadena: \"%s\"\n", e->anunciar.cadena);
                } else {
                    printf("Anunciar expresión: ");
                    imprimirExpresion(e->anunciar.expresion);
                    printf("\n");
                }
                break;

            case ENUNC_SI:
                printf("Estructura SI:\n  Condición:\n    ");
                imprimirValor(e->si.comp.izquierda);
                imprimirOp(e->si.comp);
                imprimirValor(e->si.comp.derecha);
                printf("\n  Entonces:\n");
                imprimirPrograma(e->si.entonces);
                if(e->si.si_no) {
                    printf(" Sino:\n");
                    imprimirPrograma(e->si.si_no);
                }
                break;

            case ENUNC_MIENTRAS:
                printf("Mientras:\n  Condición:\n    ");
                imprimirValor(e->mientras.comp.izquierda);
                printf(" <operador> ");
                imprimirValor(e->mientras.comp.derecha);
                printf("\n  Cuerpo:\n");
                //imprimirEnunciado(e->mientras.cuerpo);
                imprimirPrograma(e->mientras.cuerpo);
                break;

            default:
                printf("Enunciado no reconocido.\n");
                break;
        }
        e = e->siguiente;
    }
}
/**
 * @brief Imprimir Expresion
 * @brief funciones_imprimir
 *
 * Compara que tipo de expresion es y la imprime
 *
 * @param expr Expresion a imprimir.
 */
void imprimirExpresion(Expresion* expr) {
    if (!expr) return;
    if (expr->tipo == EXPR_VALOR) {
        imprimirValor(expr->valor);
    } else if (expr->tipo == EXPR_BINARIA) {
        printf("(");
        imprimirExpresion(expr->binaria.izquierda);
        printf(" %s ", expr->binaria.oparitmetico);
        imprimirExpresion(expr->binaria.derecha);
        printf(")");
    }
}
/**
 * @brief Imprimir Valor
 * @brief funciones_imprimir
 *
 * Compara que tipo de valor es y lo imprime
 *
 * @param val Valor a imprimir.
 */
void imprimirValor(Valor val) {
        switch (val.tipo) {
            case VAL_VARIABLE:
                printf("%s", val.nombre);
                break;
            case VAL_NUMERO:
                printf("%.2f", val.numero);
                break;
            case VAL_LISTA_INDEX:
                printf("%s[%d]", val.nombre, val.indice);
                break;
            case VAL_LISTA_INDEX_VAR:
                printf("%s[%s]", val.nombre, val.var);
                break;
            case VAL_LISTA_F:
                printf("%s.f",val.nombre);
                break;
            case VAL_LISTA_L:
                printf("%s.l",val.nombre);
                break;
            case VAL_LISTA_S:
                printf("%s.s",val.nombre);
                break;
            default:
                printf("Valor desconocido");
        }
}
/**
 * @brief Imprimir Operador Relacional
 * @brief funciones_imprimir
 *
 * Compara que tipo de operador relacional es y lo imprime
 *
 * @param comparacion Operador relacional a imprimir.
 */
void imprimirOp(Comparacion comparacion) {
        switch (comparacion.operador) {
            case OP_DIFERENTE:
                printf("!=");
                break;
            case OP_IGUAL:
                printf("==");
                break;
            case OP_MAYOR:
                printf(">");
                break;
            case OP_MAYOR_IGUAL:
                printf(">=");
                break;
            case OP_MENOR:
                printf("<");
                break;
            case OP_MENOR_IGUAL:
                printf("<=");
                break;
            default:
                printf("Valor desconocido");
        }
}
/**
 * @brief Imprimir Programa
 * @brief funciones_imprimir
 *
 * Imprime todos los enunciados almacenados en el parametro pasado.
 *
 * @param programa Lista de todos los enunciados actuales.
 */
void imprimirPrograma(Programa* programa) {
    printf("== Programa ==\n");
    imprimirEnunciado(programa->lista_enunciados);
}
/**
 * @brief liberarExpresion
 * @brief funciones_eliminar
 *
 * Libera la expresion depende el tipo.
 *
 * @param expr Expresion a liberar.
 */
void liberarExpresion(Expresion* expr) {
    if (!expr) return;

    if (expr->tipo == EXPR_BINARIA) {
        liberarExpresion(expr->binaria.izquierda);
        liberarExpresion(expr->binaria.derecha);
        free(expr->binaria.oparitmetico);
    }
    else if(expr->tipo == EXPR_VALOR){
        liberarValor(&expr->valor);
    }

    free(expr);
}
/**
 * @brief Liberar Valor
 * @brief funciones_eliminar
 * 
 * Compara que tipo de valor es y lo libera depende el caso.
 *
 * @param valor Valor a liberar.
 */
void liberarValor(Valor *valor)
{
    if(valor->tipo == VAL_VARIABLE || valor->tipo == VAL_LISTA_F || valor->tipo == VAL_LISTA_L || valor->tipo == VAL_LISTA_S || valor->tipo == VAL_LISTA_INDEX)
        free(valor->nombre);
    else if(valor->tipo == VAL_LISTA_INDEX_VAR){
        free(valor->var);
        free(valor->nombre);
    }
}
/**
 * @brief Eliminar Programa
 * @brief funciones_eliminar
 *
 * Pasa enunciado por enunciado dentro del programa y va liberando cada uno.
 *
 * @param programa Lista de todos los enunciados actuales.
 */
void eliminarPrograma(Programa* programa) {
    Enunciado* actual = programa->lista_enunciados;
    while (actual) {
        Enunciado* siguiente = actual->siguiente;

        switch (actual->tipo) {
            case ENUNC_DECLARACION:
            case ENUNC_DECLARACION_LISTA:
                for (int i = 0; i < actual->declaracion.cantidad; i++) {
                    free(actual->declaracion.variables[i]);
                }
                free(actual->declaracion.variables);
                break;

            case ENUNC_ASIGNACION:
            case ENUNC_ASIGNACION_LISTA:
                liberarValor(&actual->asignacion.variable);
                liberarExpresion(actual->asignacion.expresion);
                break;

            case ENUNC_LEER:
            case ENUNC_LEER_LISTA:
                free(actual->leer.variable);
                break;

            case ENUNC_ANUNCIAR:
                if (actual->anunciar.esCadena) {
                    free(actual->anunciar.cadena);
                } else {
                    liberarExpresion(actual->anunciar.expresion);
                }
                break;

            case ENUNC_SI:
                // Comparacion usa Valor que contiene un char* solo si tipo == VAL_VARIABLE
                liberarValor(&actual->si.comp.izquierda);
                liberarValor(&actual->si.comp.derecha);

                eliminarPrograma(actual->si.entonces);
                free(actual->si.entonces);
                if (actual->si.si_no)
                    eliminarPrograma(actual->si.si_no);
                    free(actual->si.si_no);
                break;

            case ENUNC_MIENTRAS:
                liberarValor(&actual->mientras.comp.izquierda);
                liberarValor(&actual->mientras.comp.derecha);

                eliminarPrograma(actual->mientras.cuerpo);
                free(actual->mientras.cuerpo);
                break;

            default:
                break;
        }

        free(actual);
        actual = siguiente;
    }

    programa->lista_enunciados = NULL;
    programa->ultimo_enunciado = NULL;
}
