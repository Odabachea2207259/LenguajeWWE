/**
 * @file Lexer.h
 * @brief Prototipos y estructuras necesarias para el lexer
 * 
 * Este archivo contiene todas las estructuras necesarias para 
 * el correcto funcionamiento del lexer
 * 
 * @author Alexander Arellano Odabachea
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @defgroup caracteres_permitidos Caracteres permitidos del lenguaje WWE
 * @{
 */
/** Caracterer del alfabeto permitidos para el nombre de variables y las palabras reservadas */
#define ALFABETO "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

/** Caracteres permitidos numericos */
#define NUMEROS "0123456789"

/** Símbolo de igual */
#define IGUAL "="

/** Símbolo de punto */
#define PUNTO "."

/** Signos permitidos para los números */
#define SIGNOS "+-"

/** Símbolo en caso de un caso de un error */
#define ERROR 'x'

/** Caracteres permitidos de operadores relacionales */
#define COMPARACION "<>"

/** Caracteres permitidos para los operadores aritmeticos */
#define OPARITMETICO "*/%"

/** Caracteres permitidos de puntuacion para los strings */
#define PUNTUACION "!@#$&()?¿¡[]{};:\\\'"

/** Caracteres especiales */
#define ESPECIALES "\n\t"

/**@} */
/**
 * @brief Estructura token
 * 
 * Esta estructura representa un token. 
 */
typedef struct token
{
	char* tipoToken; /**< Nombre del tipo de token que es */
	char* cadena; /**< Palabra almacenada del token */
}Token;

/**
 * @brief Estructura nodo
 * 
 * Esta estructura representa un nodo para la lista de tokens
 */
typedef struct nodo
{
	Token *token; /**< Token almacenado */
	struct nodo *next; /**< Apuntador al token siguiente */
	struct nodo *prev; /**< Apuntador al token anterior */
}Nodo;

/**
 * @brief Estructura de tokens
 * 
 * Esta estructura representa una lista enlazada de tokens
 */
typedef struct tokens
{
	Nodo *inicio; /**< Inicio de la lista */
	Nodo *fin; /**< Fin de la lista */
	Nodo *actual; /**< Token actual */
	int cant; /**< Cantidad de tokens almacenados */
}Tokens;

//Inputs que acepta cada estado
extern const char *inputs[][20];
extern const int mov2[][20]; 

//char nextState(char currentState, char input);
/**
 * @defgroup tokenizer Tokenizer
 * Estos métodos se encargan de revisar una palabra y retornar el tipo de token que es
 * @{
 */
int nextState(int currentState, char input);
int correctInput(const char *goodInputs, char input); 

//void isToken(char finalState);
Token* isToken(int finalState);
Token* lexer(char *str);
/**@} */

/**
 * @defgroup lexer Elementos del lexer
 * @{
 */
void incializarTokens(Tokens *tokens);
Nodo* crearNodo();
void borrarLista(Tokens *tokens);
void lexerAnalysis(char *filename,Tokens *tokens);
void inicializarTokens(Tokens *tokens);
/**@} */
