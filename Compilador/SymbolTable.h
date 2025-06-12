/**
 * @file SymbolTable.h
 * @brief Métodos de la tabla de simbolos
 * 
 * Archivo que contiene los protipos de las funciones para el correcto
 * funcionamiento de la tabla de símbolos y las estructuras necesarias
 * 
 * @author Alexander Arellano Odabachea
 */

#pragma once
#include "Lexer.h"

/**
 * @brief Estructura que representa una variable
 */
typedef struct variable
{
	Token token; /**< Valor de la variable */
	char *nombre; /**< Nombre de la variable */
	struct variable* next; /**< Apuntador a la siguiente variable */
}Variable;

/**
 * @brief Tabla hash que representa un scope
 */
typedef struct hashmap
{
	int numOfElements,capacity;
	Variable** arr;
}HashMap;

/**
 * @brief Nodo de la pila
 */
typedef struct Node
{
	HashMap *scope; /**< Tabla hash que representa un scope*/
	struct Node* next; /**< Apuntador al siguiente nodo */
}NodoPila;

/**
 * @brief Pila
 */
typedef struct stack
{
	NodoPila* head; /**< Scope actual */
}Pila;

extern Pila copia;

/**
 * @defgroup funciones_pila Funciones de la tabla de símbolos
 * @{
 */
NodoPila* crearNodoPila(HashMap *scope);
void inicializarPila(Pila *pila);
int estaVacia(Pila *pila);
void push(Pila *pila, HashMap *scope); //En vez de data irá una tabla hash
void pop(Pila *pila);
void popCompleto(Pila *pila);
void vaciarPila(Pila *pila);
void vaciarPilaCompleto(Pila *pila);
void borrarValor(Pila *pila, char* nombre);
void asignarValor(Pila *pila, char* nombre, Token token);
Token* regresarValor(Pila *pila, char* nombre);
void printTabla();
int buscarPila(Pila* pila, char* nombre);
int intermedia(Pila* scopes, char* nombre, Token token, int declaracion);
//int peek(Pila *pila); //Aqui no va a regresar un int, sino una tabla hash
/**@} */

/**
 * @defgroup funciones_hashtable Funciones de cada scope
 * @{
 */
Variable* setVariable(char* value);
void inicializarHashMap(HashMap* hashmap);
int hashFunction(HashMap* hashmap, char* nombre);
void insert(HashMap* hashmap, char* nombre);
int buscar(HashMap* hashmap,char* nombre);
int asignarValorHash(HashMap* hashmap, char* nombre, Token token);
int declararLista(HashMap* hashmap, char* nombre);
Token* regresar(HashMap* hashmap, char* nombre);
int borrarValorHash(HashMap* hashmap, char* nombre);
void printVariable(HashMap* hashmap, char* nombre);
void borrarHash(HashMap* hashmap);
/**@} */

/**
 * @defgroup funciones_compilador Funciones principales del compilador
 * @{
 */
int parser(char *filename, Tokens *tokens);
/**@} */