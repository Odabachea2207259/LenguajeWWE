/**
 * @file Parser.c
 * @brief Archivo que contiene todo lo necesario para el parser del programa.
 * 
 * En este archivo se realizan todos los pasos del parser, checa la lista del tokens generada
 * por el lexer y revisa su correcta implementación.
 * 
 * @author Alexander Arellano Odabachea
 */

#include "Lexer.h"
#include "SymbolTable.h"

/**@cond */
int analizar(Tokens* tokens, Pila *scopes);
int programa(Tokens* tokens, Pila *scopes);
int enunciados(Tokens *tokens, Pila *scopes);
int enunciado(Tokens *tokens, Pila *scopes);
int variables(Tokens *tokens, Pila *scopes);
int asignacion(Tokens *tokens, Pila *scopes);
int lista(Tokens *tokens, Pila *scopes);
int expresion(Tokens *tokens, Pila *scopes,char* nombreVar);
int expresionLista(Tokens *tokens, Pila *scopes,char* nombreVar);
int anunciar(Tokens *tokens, Pila *scopes);
int si(Tokens *tokens, Pila *scopes);
int mientras(Tokens *tokens, Pila *scopes);
int comparacion(Tokens *tokens, Pila *scopes);
int valor(Tokens* tokens, Pila *scopes, char* nombreVar);
int oprelacional(Tokens *tokens, Pila *scopes);

void term(Tokens* tokens, Pila *scopes);
void borrarTodo(Pila *scopes);
/**@endcond */

/**
 * @brief Booleano global para saber si entre a una declaracion de variables, una declaracion de variables lista o no
 */
int declaracion = 0;
/**
 * @brief Contador global para saber el total de líneas de mi documento
 */
int linea = 1;

HashMap* crearScope();

/**
 * @brief Copia global que representa una copia de mi tabla de simbolos.
 */
Pila copia;

/**
 * @brief Parser
 * @ingroup funciones_compilador
 * 
 * Método que genera una tabla de símbolos y analiza la lista de tokens.
 * 
 * @param filename Nombre del archivo a revisar
 * @param tokens Lista de tokens.
 */
int parser(char *filename, Tokens *tokens)
{
	Pila scopes;
	inicializarPila(&scopes);
	inicializarPila(&copia);
	push(&scopes,crearScope());

	if(analizar(tokens,&scopes))
	{
		//borrarTodo(&scopes);
		push(&copia,scopes.head->scope);
		pop(&scopes);
		return 1;
	}

	//borrarTodo(&scopes);

	term(tokens,&scopes);
	
	return 0;
}

/**
 * @brief Analizar
 * 
 * Analiza la lista de tokens y para si encuentra algo incorrecto
 * 
 * @param tokens Lista de tokens.
 * @param scopes Tabla de símbolos.
 * 
 * @return int Si esta todo correcto.
 */
int analizar(Tokens* tokens, Pila *scopes)
{
	if(programa(tokens,scopes))
		return 1;
	else
		return 0;
}

/**
 * @brief Programa
 * 
 * Analiza si el programa inicializa y termina con los tokens
 * correctos
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Programa> -> raw <Enunciados> smackdown
int programa(Tokens* tokens, Pila *scopes)
{
	if(!strcmp(tokens->actual->token->tipoToken,"INICIO-PROGRAMA"))
	{
		push(scopes,crearScope());

		tokens->actual = tokens->actual->next;
		if(enunciados(tokens,scopes)){}
		while(tokens->actual && !strcmp(tokens->actual->token->tipoToken,"FIN-LINEA")){
			linea++;
			tokens->actual = tokens->actual->next;
		}
		if(!strcmp(tokens->actual->token->tipoToken,"FIN-PROGRAMA"))
		{
			push(&copia,scopes->head->scope);
			pop(scopes);
			return 1;
		}
		else
		{
			printf("Se esperaba \"smackdown\", se encontro %s\n",tokens->actual->token->cadena);
			return 0;
		}
	}
	else
		printf("Se esperaba \"raw\", se encontro: %s\n",tokens->actual->token->cadena);
	
	return 0;
}

/**
 * @brief Enunciados
 * 
 * Analiza cada enunciado
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Enunciados> -> <Enunciado> <Enunciados> | <Enunciado>
int enunciados(Tokens *tokens, Pila *scopes)
{
	if(enunciado(tokens,scopes))
	{
		while(enunciado(tokens,scopes));
		return 1;
	}

	return 0;
}

/**
 * @brief Enunciado
 * 
 * A diferencia de Enunciados, analiza cada enunciado uno por uno
 * y llama al método correspondiente
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Enunciado> -> <Declaracion> | <Asignacion> | <Anunciar> | <Si> | <Mientras> 
int enunciado(Tokens *tokens, Pila *scopes)
{
	//printf("Analizando token: %s con lexeme: %s\n",tokens->actual->token->tipoToken,tokens->actual->token->cadena);
	declaracion = 0;
	Nodo *prev = tokens->actual;

	if(!strcmp(tokens->actual->token->tipoToken,"FIN-LINEA"))
	{
		linea++;
		tokens->actual = tokens->actual->next;
		return 1;
	}

	//printf("Token actual: %s\n",tokens->actual->token->cadena);

	if(!strcmp(tokens->actual->token->tipoToken,"INICIALIZAR"))
	{
		//printf("INICIALIZAR\n");
		declaracion = 1;
		tokens->actual = tokens->actual->next;
		if(variables(tokens,scopes))
		{
			return 1;
		}
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"NEXUS"))
	{
		//printf("INICIALIZAR LISTAS\n");
		declaracion = 2;
		tokens->actual = tokens->actual->next;
		if(variables(tokens,scopes))
		{
			return 1;
		}
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE")) //AQUI DEBO BUSCAR SI LA VAR ESTA DECLARADA
	{
		//printf("ASIGNACION\n");

		if(!intermedia(scopes,tokens->actual->token->cadena,*tokens->actual->token,declaracion))
			term(tokens,scopes);
		tokens->actual = tokens->actual->next;
		if(asignacion(tokens,scopes))
			return 1;
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"ANUNCIAR"))
	{
		//printf("ANUNCIAR\n");
		tokens->actual = tokens->actual->next;
		if(anunciar(tokens,scopes))
			return 1;
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"SI"))
	{
		//printf("SI\n");
		push(scopes,crearScope());
		//push(&copia,crearScope());

		tokens->actual = tokens->actual->next;
		if(si(tokens,scopes))
			return 1;
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"MIENTRAS"))
	{
		//printf("MIENTRAS\n");
		push(scopes,crearScope());
		//push(&copia,crearScope());

		tokens->actual = tokens->actual->next;
		if(mientras(tokens,scopes))
			return 1;
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"NOT_TOKEN"))
	{
		printf("La palabra %s no es una palabra reservada valida\n",tokens->actual->token->cadena);
		term(tokens,scopes);
		return 0;
	}

	tokens->actual = prev;

	if(!strcmp(tokens->actual->token->tipoToken,"NUM") || !strcmp(tokens->actual->token->tipoToken,"FLOAT") || !strcmp(tokens->actual->token->tipoToken,"OPARITMETICO") || !strcmp(tokens->actual->token->tipoToken,"OPRELACIONAL"))
	{
		//printf("Token mal: %s\n",tokens->actual->token->cadena);
		printf("Formato no permitido\n");
		term(tokens,scopes);
		return 0;
	}

	return 0;
}

/**
 * @brief Asignacion
 * 
 * Método para revisar si una asignación está correcta
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Asignacion> -> VARIABLE = <Expresion>
int asignacion(Tokens *tokens, Pila *scopes)
{
	Nodo* temp = tokens->actual->prev;

	Token* token = regresarValor(scopes,tokens->actual->prev->token->cadena);

	if(!strcmp(tokens->actual->token->tipoToken,"IGUAL"))
	{
		if(!strcmp(token->tipoToken,"LISTA")){
			printf("Se esperaba un indice valido de la lista\n");
			term(tokens,scopes);
		}
		borrarValor(scopes,temp->token->cadena);
		tokens->actual = tokens->actual->next;
		if(expresion(tokens,scopes,temp->token->cadena)) //AQUI PODRIA PONER LOS VALORES
			return 1;
		else if(!strcmp(tokens->actual->token->tipoToken,"PROMO"))
		{
			tokens->actual = tokens->actual->next;
			return 1;
		}
	}
	else if(lista(tokens,scopes))
	{
		if(!strcmp(tokens->actual->token->tipoToken,"IGUAL"))
		{
			tokens->actual = tokens->actual->next;
			if(expresionLista(tokens,scopes,temp->token->cadena))
				return 1;
			else if(!strcmp(tokens->actual->token->tipoToken,"PROMO"))
			{
				tokens->actual = tokens->actual->next;
				return 1;
			}
			else{
				printf("Se esperaba una expresion o \"promo\", se encontro: %s\n",tokens->actual->token->cadena);
				term(tokens,scopes);
			}
		}
		else {
			printf("Se esperaba \"=\", se encontro: %s\n",tokens->actual->token->cadena);
			term(tokens,scopes);
		}
	}
	else if(!strcmp(token->tipoToken,"LISTA"))
	{
		if(!strcmp(tokens->actual->token->tipoToken,"FIN-ANUNCIAR"))
		{
			tokens->actual = tokens->actual->next;
			if(expresionLista(tokens,scopes,temp->token->cadena))
				return 1;
			else if(!strcmp(tokens->actual->token->tipoToken,"PROMO"))
			{
				tokens->actual = tokens->actual->next;
				return 1;
			}
		}
		printf("Se esperaba \"<-\", se encontro: %s\n", tokens->actual->token->cadena);
		term(tokens,scopes);
	}
	else {
		printf("Se esperaba \"=\" o \"[\", se encontro: %s\n",tokens->actual->token->cadena);
		term(tokens,scopes);
	}
	
	return 0;
}

/**
 * @brief Lista
 * 
 * Método que revisa si mi token anterior es del tipo lista con indice
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si mi token es una lista con indice
 */
int lista(Tokens *tokens, Pila *scopes)
{
	Nodo *prev = tokens->actual;
	if(!strcmp(tokens->actual->token->tipoToken,"LBRACKET"))
	{
		tokens->actual = tokens->actual->next;
		if(valor(tokens,scopes,tokens->actual->token->cadena))
		{
			if(!strcmp(tokens->actual->token->tipoToken,"RBRACKET"))
			{
				tokens->actual = tokens->actual->next;
				return 1;
			}
			else
			{
				printf("Se esperaba \"]\", se encontro: %s\n",tokens->actual->token->cadena);
				term(tokens,scopes);
			}
		}
		else {
			printf("Se esperaba un valor valido, se encontro: %s\n",tokens->actual->token->cadena);
			term(tokens,scopes);
		}
	}
	
	tokens->actual = prev;
	return 0;
}

/**
 * @brief Expresion
 * 
 * Analiza si se encontro una expresion correcta
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * @param nombreVar Nombre de la variable para asignarle el valor en la tabla de simbolos
 * 
 * @return int Si esta todo correcto
 */

//<Expresion> -> <Valor> <OPAritmetico> <Expresion> | <Valor> | CADENA
int expresion(Tokens *tokens, Pila *scopes,char *nombreVar) //Le debo pasar el nombre de la variable
{
	Nodo* prev = tokens->actual;
	if(valor(tokens,scopes,nombreVar))
	{
		if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
		{
			if(!strcmp(tokens->actual->prev->token->tipoToken,"VARIABLE"))
			{
				Token* token = regresarValor(scopes,tokens->actual->prev->token->cadena);
				asignarValor(scopes,nombreVar,*token);
			}
			else
				asignarValor(scopes,nombreVar,*tokens->actual->prev->token);

			asignarValor(scopes,nombreVar,*tokens->actual->token);

			tokens->actual = tokens->actual->next;
			if(expresion(tokens,scopes,nombreVar))
				return 1;
			else
			{
				printf("Se esperaba una expresion, se encontro: %s\n",tokens->actual->token->cadena);
				term(tokens,scopes);
			}
		}
	}
	
	tokens->actual = prev;

	if(valor(tokens,scopes,nombreVar))
	{
		if(!strcmp(tokens->actual->prev->token->tipoToken,"VARIABLE"))
		{
			Token* token = regresarValor(scopes,tokens->actual->prev->token->cadena);
			asignarValor(scopes,nombreVar,*token);
		}
		else if(!strcmp(tokens->actual->prev->token->tipoToken,"NUM") || !strcmp(tokens->actual->prev->token->tipoToken,"FLOAT")){
			asignarValor(scopes,nombreVar,*tokens->actual->prev->token);
			//printf("Token actual: %s\n",tokens->actual->token->cadena);
			//printf("Token prev: %s\n",tokens->actual->prev->token->cadena);
		}
		return 1;
	}

	if(!strcmp(tokens->actual->token->tipoToken,"STRING"))
	{
		asignarValor(scopes,nombreVar,*tokens->actual->token);

		tokens->actual = tokens->actual->next;
		return 1;
	}
	
	return 0;
}

/**
 * @brief Expresion Lista
 * 
 * Analiza si se encontro una expresion correcta para una lista
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * @param nombreVar Nombre de la variable para asignarle el valor en la tabla de simbolos
 * 
 * @return int Si esta todo correcto
 */
int expresionLista(Tokens *tokens, Pila *scopes,char* nombreVar)
{
	Nodo* prev = tokens->actual;
	if(valor(tokens,scopes,nombreVar))
	{
		if(!strcmp(tokens->actual->token->tipoToken,"OPARITMETICO"))
		{
			tokens->actual = tokens->actual->next;
			if(expresion(tokens,scopes,nombreVar))
				return 1;
			else
			{
				printf("Se esperaba una expresion, se encontro: %s\n",tokens->actual->token->cadena);
				term(tokens,scopes);
			}
		}
	}
	
	tokens->actual = prev;

	if(valor(tokens,scopes,nombreVar))
	{
		return 1;
	}
	
	return 0;
}
/**
 * @brief Anunciar
 * 
 * Analiza si encontro un enunciado anunciar y si está correcta su implementación
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Anunciar> -> -> CADENA <- | -> <Variables> <-
int anunciar(Tokens *tokens, Pila *scopes)
{
	if(!strcmp(tokens->actual->token->tipoToken,"INICIO-ANUNCIAR"))
	{
		tokens->actual = tokens->actual->next;
		if(!strcmp(tokens->actual->token->tipoToken,"STRING"))
		{
			tokens->actual = tokens->actual->next;
		}
		else if(expresion(tokens,scopes,tokens->actual->token->cadena))
		{
			//tokens->actual = tokens->actual->next;
			//while(variables(tokens));
		}
		else if(variables(tokens,scopes))
		{
		}

		if(!strcmp(tokens->actual->token->tipoToken,"FIN-ANUNCIAR"))
		{
			tokens->actual = tokens->actual->next;

			return 1;
		}
		else
		{
			printf("Se esperaba \"<-\", se encontro: %s\n",tokens->actual->token->cadena);
			term(tokens,scopes);
		}

	} else
	{
		printf("Se esperaba \"->\", se encontro: %s\n",tokens->actual->token->cadena);
		term(tokens,scopes);
	}

	return 0;
}

/**
 * @brief Si
 * 
 * Analiza si encontro un enunciado si y si está implementado correctamente
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Si> -> jeff <Comparacion> <Enunciados> hardy | jeff <Comparacion> <Enunciados> matt <Enunciados> hardy
int si(Tokens *tokens, Pila *scopes)
{
	Nodo *prev = tokens->actual;
		
	if(comparacion(tokens,scopes))
	{
		if(enunciados(tokens,scopes))
		{
		}

		if(!strcmp(tokens->actual->token->tipoToken,"SINO"))
		{
			tokens->actual = tokens->actual->next;
			if(enunciados(tokens,scopes)){}
		}

		if(!strcmp(tokens->actual->token->tipoToken,"FIN-SI"))
		{
			push(&copia,scopes->head->scope);
			pop(scopes);
			tokens->actual = tokens->actual->next;
			return 1;
		}
		else
		{
			printf("Se esperaba \"hardy\", se encontro: %s\n",tokens->actual->token->cadena);
			term(tokens,scopes);
		}
	}
	else
	{
		printf("Se esperaba una comparacion, se encontro: %s\n",tokens->actual->token->cadena);
		term(tokens,scopes);
	}

	return 0;
}

/**
 * @brief Mientras
 * 
 * Analiza si encontro un enunciado mientras y si está implementado correctamente
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Mientras> -> randy <Comparacion> <Enunciados> orton
int mientras(Tokens *tokens, Pila *scopes)
{
	if(comparacion(tokens,scopes))
	{
		if(enunciados(tokens,scopes))
		{
		}

		if(!strcmp(tokens->actual->token->tipoToken,"FIN-MIENTRAS"))
		{
			push(&copia,scopes->head->scope);
			pop(scopes);
			tokens->actual = tokens->actual->next;
			return 1;
		}
		else
		{
			printf("Se esperaba \"orton\", se encontro: %s\n",tokens->actual->token->cadena);
			term(tokens,scopes);
		}
	}
	else
	{
		printf("Se esperaba una comparacion, se encontro: %s\n",tokens->actual->token->cadena);
		term(tokens,scopes);
	}

	return 0;
}

/**
 * @brief Variables
 * 
 * Analiza la variable o variables, depende la variable global declaracion, las va a declarar en la tabla de simbolos
 * o buscará si está inicializada antes de usarla.
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Variables> -> VARIABLE <Variables> | VARIABLE
int variables(Tokens *tokens, Pila *scopes)
{
	if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE")) //AQUI SE METERA CADA VARIABLE A LA HASHTABLE
	{
		if(declaracion == 2)
		{
			if(!declararLista(scopes->head->scope,tokens->actual->token->cadena))
				term(tokens,scopes);
		}
		else
		{
			if(!intermedia(scopes,tokens->actual->token->cadena,*tokens->actual->token,declaracion))
				term(tokens,scopes);
		}
		
		tokens->actual = tokens->actual->next;

		if(variables(tokens,scopes))
		{
			while(variables(tokens,scopes));
		}

		return 1;
	}

	return 0;
}

/**
 * @brief Comparacion
 * 
 * Analiza si la comparacion realizada está correctamente implementada
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<Comparacion> -> <Valor> <OPRelacional> <Valor>
int comparacion(Tokens *tokens, Pila *scopes)
{
	Nodo *prev = tokens->actual;

	if(valor(tokens,scopes," "))
	{
		if(oprelacional(tokens,scopes))
		{
			if(valor(tokens,scopes," "))
				return 1;
		}
	}

	return 0;
}

/**
 * @brief Valor
 * 
 * Analiza el tipo de valor que es el token actual
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * @param nombreVar Nombre de la variable a buscar
 * 
 * @return int Si esta todo correcto
 */
//<Valor> -> VARIABLE | NUMERO
int valor(Tokens* tokens, Pila *scopes, char *nombreVar)
{
	Nodo *prev = tokens->actual;

	if(!strcmp(tokens->actual->token->tipoToken,"VARIABLE"))
	{
		if(!intermedia(scopes,tokens->actual->token->cadena,*tokens->actual->token,declaracion))
			term(tokens,scopes);

		tokens->actual = tokens->actual->next;

		if(lista(tokens,scopes))
			return 1;

		if(!strcmp(tokens->actual->token->tipoToken,"LAST"))
		{
			tokens->actual = tokens->actual->next;
			return 1;
		}

		if(!strcmp(tokens->actual->token->tipoToken,"FIRST"))
		{
			tokens->actual = tokens->actual->next;
			return 1;
		}

		if(!strcmp(tokens->actual->token->tipoToken,"SIZE"))
		{
			tokens->actual = tokens->actual->next;
			return 1;
		}
		return 1;
	}
	
	if(!strcmp(tokens->actual->token->tipoToken,"NUM"))
	{
		tokens->actual = tokens->actual->next;
		return 1;
	}
	
	if(!strcmp(tokens->actual->token->tipoToken,"FLOAT"))
	{
		tokens->actual = tokens->actual->next;
		return 1;
	}

	if(!strcmp(tokens->actual->token->tipoToken,"LAST"))
	{
		tokens->actual = tokens->actual->next;
		return 1;
	}

	if(!strcmp(tokens->actual->token->tipoToken,"FIRST"))
	{
		tokens->actual = tokens->actual->next;
		return 1;
	}
	
	return 0;
}	

/**
 * @brief OpRelacional
 * 
 * Analiza que si el operador relacional esta permitido
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 * @return int Si esta todo correcto
 */
//<OPRelacional> -> < | > | <= | >= | <> | ==
int oprelacional(Tokens *tokens, Pila *scopes)
{
	if(!strcmp(tokens->actual->token->tipoToken,"COMPARACION"))
	{
		tokens->actual = tokens->actual->next;
		return 1;
	}

	return 0;
}

/**
 * @brief Terminacion de programa ante un error
 * 
 * Método que borra todo para no dejar fugas de memoria, imprime en que línea se encontró el error y termina el programa.
 * 
 * @param tokens Lista de tokens
 * @param scopes Tabla de símbolos
 * 
 */
void term(Tokens *tokens, Pila *scopes)
{
	push(&copia,scopes->head->scope);
	pop(scopes);
	borrarLista(tokens);
	vaciarPilaCompleto(scopes);
	vaciarPilaCompleto(&copia);
	printf("Error en lína %d\n",linea);
	exit(1);
}

/**
 * @brief Borrar pila
 * 
 * Borra la pila ya que no se usará más
 * 
 * @param scopes Tabla de simbolos
 */
void borrarTodo(Pila *scopes)
{
	//borrarLista(tokens);
	vaciarPila(scopes);
	//vaciarPila(&copia);
}

/**
 * @brief Crear scope
 * 
 * Crea un nuevo scope o tabla hash, para la pila, cuando entra a un nuevo scope
 * 
 * @return HashMap* Una hashtable para la pila.
 */
HashMap* crearScope()
{
	HashMap *scope = (HashMap*)malloc(sizeof(HashMap));
	inicializarHashMap(scope);

	return scope;
}