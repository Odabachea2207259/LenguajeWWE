/**
 * @file LexicalAnalysis.c
 * 
 * @brief Métodos para revisar todo un programa y retornar una lista de tokens
 * 
 * Este archivo contiene todo lo necesario para crear una lista de tokens 
 * del archivo que se indique
 * 
 * @author Alexander Arellano Odabachea
 */

#include "Lexer.h"
#include <stdlib.h>
#include <ctype.h>

/**Caracteres especiales permitidos */
#define SIGNOS1 "%:=+-*/_.,<>#!\"\\\'"
#define true 1
#define false 0

int encontrado(char c, char* lista);
int esValido(char c,int *string);
Nodo* crearNodo(Token *token);
void insertarNodo(Tokens *tokens, Nodo *nodo);
void printTokens(Tokens *tokens);

/**
 * @brief encontrado
 * @ingroup lexer
 * 
 * Método que revisa si un carácter está en la palabra o lista enviada
 * 
 * @param c Caracter a buscar
 * @param lista Lista de caracteres donde se buscará
 * 
 * @return int Si se encontro la palabra
 */
int encontrado(char c, char* lista)
{
	for(int i = 0; i < strlen(lista); i++)
	{
		if(c == lista[i])
			return 1;
	}

	return 0;
}

/**
 * @brief Es valido
 * @ingroup lexer
 * 
 * Este método revisa si un carácter ingresado es válido
 * Si se manda true como parametro, quiere decir que se está analizando un string, por
 * lo que debe añadir más caracteres que pueden ingresarse
 * 
 * @param c Caracter a revisar
 * @param string Booleano para revisar si es un string 
 * 
 * @return int Si es valido o no
 */
int esValido(char c,int *string)
{	
	if(*string == true) {
		if(c == '"'){
			*string = false;
		}

		if(isalnum(c) || encontrado(c, SIGNOS1) || c == ' ')
			return 1;
	}
	else if(*string == false){
		if(isalnum(c) || encontrado(c, SIGNOS1))
			return 1;
	}

	//if(isalnum(c) || encontrado(c, SIGNOS))
	//		return 1;
	return 0;
}

/**
 * @brief Análisis Léxico
 * @ingroup lexer
 * 
 * Este método revisa todo un archivo y pasa cada palabra que encuentre
 * al tokenizer para ir haciendo la lista de tokens
 * Ignora los comentarios
 * 
 * @param filename Nombre del archivo a revisar
 * @param tokens Lista de tokens donde se agregaran los tokens encontrados
 */
void lexerAnalysis(char *filename,Tokens *tokens)
{
	int cantLineas = 0;
	FILE *file;

	char word[100];
	int i;
	char c,prev;
	int comentario = 0, string = false, variable = 0;

	file = fopen(filename,"r");
	if(file == NULL)
	{
		printf("Error opening file.\n");
		return;
	}

	while((c = fgetc(file)) != EOF)
	{

		i = 0;
		variable = 0;
		if(c == '#')
			comentario = 1;

		if(c == '\n' || c == '\t')
			comentario = 0;

		if(c == '\n')
		{
			cantLineas++;
			comentario = 0;
			Token *token = (Token*)malloc(sizeof(Token));
			token->tipoToken = strdup("FIN-LINEA");
			token->cadena = strdup(" ");
			Nodo *nuevoNodo = crearNodo(token);
			insertarNodo(tokens,nuevoNodo);
			continue;
		}
		
		if(c == '"')
		{
			string = true;
			word[i++] = c;
			c = fgetc(file);
		}

		while(esValido(c,&string))
		{
			prev = c;
			word[i++] = c;
			c = fgetc(file);
			if(c == '.' && !isdigit(prev))
			{
				variable = 1;
				break;
			}
		}
		word[i] = '\0';
		if(i > 0)
		{
			if(!comentario)
			{
				//printf("%s\t", word);
				Token *token = lexer(word);
				token->cadena = (char*)calloc(i+1,sizeof(char));
				strcpy(token->cadena,word);
				Nodo* nuevoNodo = crearNodo(token);
				insertarNodo(tokens,nuevoNodo);
			}
		}	
		i = 0;
		if(variable)
		{
			while(esValido(c,&string))
			{
				word[i++] = c;
				c = fgetc(file);
			}
			word[i] = '\0';
			if(i > 0)
			{
				Token *token = lexer(word);
				token->cadena = (char*)calloc(i+1,sizeof(char));
				strcpy(token->cadena,word);
				Nodo* nuevoNodo = crearNodo(token);
				insertarNodo(tokens,nuevoNodo);
			}
			variable = 0;
		}

		if(c == '[')
		{
			comentario = 0;
			Token *token = (Token*)malloc(sizeof(Token));
			token->tipoToken = strdup("LBRACKET");
			token->cadena = strdup("[");
			Nodo *nuevoNodo = crearNodo(token);
			insertarNodo(tokens,nuevoNodo);
			continue;
		}
	

		if(c == ']')
		{
			comentario = 0;
			Token *token = (Token*)malloc(sizeof(Token));
			token->tipoToken = strdup("RBRACKET");
			token->cadena = strdup("]");
			Nodo *nuevoNodo = crearNodo(token);
			insertarNodo(tokens,nuevoNodo);
			continue;
		}

		if(c == '\n')
		{
			Token *token = (Token*)malloc(sizeof(Token));
			token->tipoToken = strdup("FIN-LINEA");
			token->cadena = strdup(" ");
			Nodo *nuevoNodo = crearNodo(token);
			insertarNodo(tokens,nuevoNodo);
			cantLineas++;
			//break;
		}
	}
	//printTokens(tokens);
	//borrarLista(tokens);
	fclose(file);
}

/**
 * @brief Crear nodo
 * @ingroup lexer
 * 
 * Este método crea un token para la lista enlazada de tokens
 * 
 * @param token Token a agregar
 * 
 * @return Nodo* Apuntador al nuevo nodo
 */
Nodo* crearNodo(Token *token)
{
	Nodo *nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
	nuevoNodo->token = token;
	nuevoNodo->next = NULL;
	nuevoNodo->next = NULL;

	return nuevoNodo;
}

/**
 * @brief Inicializar tokens
 * @ingroup lexer
 * 
 * Este método inicializa la lista de tokens
 * 
 * @param tokens Lista a inicializar
 */
void inicializarTokens(Tokens *tokens)
{
	tokens->cant = 0;
	tokens->inicio = NULL;
	tokens->fin = NULL;
	tokens->actual = NULL;
}

/**
 * @brief Insertar nodo
 * @ingroup lexer
 * 
 * Este método agrega un nuevo nodo a la lista
 * 
 * @param tokens Lista enlazada de tokens
 * @param nodo Nodo con el token a agregar
 */
void insertarNodo(Tokens *tokens, Nodo *nodo)
{
	if(tokens->inicio == NULL)
	{
		tokens->inicio = nodo;
		tokens->fin = nodo;
		tokens->actual = nodo;
	}

	tokens->fin->next = nodo;
	nodo->prev = tokens->fin;
	tokens->fin = nodo;
	tokens->cant++;
}

/**
 * @brief Borrar Lista
 * @ingroup lexer
 * 
 * Este método borra la lista que se pasa
 * 
 * @param tokens Lista enlazada de tokens
 */
void borrarLista(Tokens *tokens)
{
	Nodo *nodo = tokens->fin;
	for(int i = tokens->cant; i > 0; i--)
	{
		if (nodo->token->tipoToken != NULL) {
    		free(nodo->token->tipoToken); // Liberar solo si no es NULL
    		nodo->token->tipoToken = NULL; // Asegurar que no se libere dos veces
		}

		if(nodo->token->cadena != NULL){
			free(nodo->token->cadena);
			nodo->token->cadena = NULL;
		}

		if(nodo->token != NULL){
			free(nodo->token);
			nodo->token = NULL;
		}

		tokens->fin = tokens->fin->prev;
		free(nodo);
		nodo = tokens->fin;
	}
}

/**
 * @brief Imprimir tokens
 * @ingroup lexer
 * 
 * Este método imprime la lista de tokens que se pasa
 * 
 * @param tokens Lista de tokens a imprimir
 */
void printTokens(Tokens *tokens)
{
	tokens->actual = tokens->inicio;
	Nodo *nodo = tokens->actual;

	for(int i = 0; i < tokens->cant; i++)
	{
		printf("%s -> %s\n",nodo->token->cadena,nodo->token->tipoToken);
		nodo = nodo->next;
	}
	tokens->actual = tokens->inicio;
}
