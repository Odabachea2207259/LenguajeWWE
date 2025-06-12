/**
 * @file SymbolTable.c
 * @brief Métodos de la tabla de simbolos
 * 
 * Archivo que contiene las implementaciones de las funciones para el correcto
 * funcionamiento de la tabla de símbolos
 * 
 * @author Alexander Arellano Odabachea
 */

#include "SymbolTable.h"

//Pila copia;

/**
 * @brief SetVariable
 * @ingroup funciones_hashtable
 * 
 * Método que inicializa un valor tipo Variable para la tabla de simbolos
 * 
 * @param nombre Nombre de la variable nueva
 * 
 * @return Variable* Apuntador a la nueva variable
 */
Variable* setVariable(char* nombre)
{
	Variable* var = (Variable*)malloc(sizeof(Variable));
	var->nombre = strdup(nombre);
	var->next = NULL;
	var->token.tipoToken = (char*)calloc(sizeof(char),10);
	var->token.cadena = (char*)calloc(sizeof(char),20);

	return var;
}

/**
 * @brief Inicializar hashmap
 * @ingroup funciones_hashtable
 * 
 * Inicializa una nueva tabla hash
 * 
 * @param hashmap Nueva tabla hash a inicializar
 */
void inicializarHashMap(HashMap* hashmap)
{
	hashmap->capacity = 100;
	hashmap->numOfElements = 0;

	hashmap->arr = (Variable**)calloc(hashmap->capacity,sizeof(Variable*));
}

/**
 * @brief Funcion hash
 * @ingroup funciones_hashtable
 * 
 * Función hash que utilizo para el valor del índice en base al nombre de la variable
 * 
 * @param hashmap Tabla hash donde se pondrá la variable
 * @param nombre Nombre de la variable
 * 
 * @return int Indice de la nueva variable
 */
int hashFunction(HashMap* hashmap, char *nombre)
{
	int bucketIndex;
	int sum = 0, factor = 31;
	for(int i = 0; i < strlen(nombre); i++)
	{
		sum = ((sum % hashmap->capacity) + (((int)nombre[i]) * factor) % hashmap->capacity);

		factor = ((factor % __INT16_MAX__)
			 * (31 % __INT16_MAX__))
			 % __INT16_MAX__;
	}

	bucketIndex = sum % hashmap->capacity;
	return bucketIndex;
}

/**
 * @brief Insert a la tabla hash
 * @ingroup funciones_hashtable
 * 
 * Método que añade una nueva variable a una tabla hash
 * 
 * @param hashmap Tabla hash donde se pondrá la variable
 * @param nombre Variable a añadir
 */
void insert(HashMap* hashmap, char* nombre)
{
	Variable* var = setVariable(nombre);
	int bucketIndex = hashFunction(hashmap,nombre);
	
	if(hashmap->arr[bucketIndex] == NULL)
	{
		hashmap->arr[bucketIndex] = var;
	}
	else
	{
		var->next = hashmap->arr[bucketIndex];
		hashmap->arr[bucketIndex] = var;
	}

	hashmap->numOfElements++;
}

/**
 * @brief Busca de variable
 * @ingroup funciones_hashtable
 * 
 * Método que busca si una variable ya está en la tabla hash
 * 
 * @param hashmap Tabla hash donde se buscará la variable
 * @param nombre Variable a buscar
 * 
 * @return int Si se encontró la variable en la tabla
 */
int buscar(HashMap* hashmap, char* nombre)
{
	int bucketIndex = hashFunction(hashmap,nombre);

	Variable* bucketHead = hashmap->arr[bucketIndex];

	while(bucketHead != NULL)
	{
		if(!strcmp(bucketHead->nombre,nombre))
			return 1;

		bucketHead = bucketHead->next;
	}

	return 0;
}

/**
 * @brief Regresar variable
 * @ingroup funciones_hashtable
 * 
 * Regresa el tipo de valor de la variable encontrada, si la variable tenia un float retorna un token tipo FLOAT
 * 
 * @param hashmap Tabla hash a buscar la variable
 * @param nombre Variable a buscar
 * 
 * @return Token* apuntador al tipo de token que es la variable
 */
Token* regresar(HashMap* hashmap, char* nombre)
{
	int bucketIndex = hashFunction(hashmap,nombre);

	Variable* bucketHead = hashmap->arr[bucketIndex];

	while(bucketHead != NULL)
	{
		if(!strcmp(bucketHead->nombre,nombre))
			return &bucketHead->token;

		bucketHead = bucketHead->next;
	}

	return NULL;
}

/**
 * @brief Asignar valor
 * @ingroup funciones_hashtable
 * 
 * Método que asigna un valor a una variable ya declarada
 * 
 * @param hashmap Tabla hash donde se buscará la variable
 * @param nombre Variable a buscar y la cual se le añadirá el valor
 * @param token Tipo de valor de la variable
 * 
 * @return int Si se encontró la variable en esa tabla
 */
int asignarValorHash(HashMap* hashmap, char* nombre, Token token)
{
	//printf("Iniciando asignacion de %s a %s\n",token.cadena,nombre);
	int bucketIndex = hashFunction(hashmap,nombre);

	Variable* bucketHead = hashmap->arr[bucketIndex];

	while(bucketHead != NULL)
	{
		if(!strcmp(bucketHead->nombre,nombre))
		{
			if(!strcmp(token.tipoToken,"FLOAT") || !strcmp(token.tipoToken,"NUM"))
				strcpy(bucketHead->token.tipoToken,"FLOAT");
			else if(!strcmp(token.tipoToken,"STRING"))
				strcpy(bucketHead->token.tipoToken,token.tipoToken);
			else if(!strcmp(token.tipoToken,"LISTA"))
				strcpy(bucketHead->token.tipoToken,token.tipoToken);
			
			char temp[50];
			strcpy(temp,bucketHead->token.cadena);
			strcat(temp,token.cadena);
			strcpy(bucketHead->token.cadena,temp);
			//strcat(bucketHead->token.cadena,token.cadena);

			//printf("Valor %s asignado a %s\n",bucketHead->token.cadena,bucketHead->nombre);
			return 1;
		}

		bucketHead = bucketHead->next;
	}

	return 0;
}

/**
 * @brief Declaracion lista
 * @ingroup funciones_hashtable
 * 
 * Método que declara una variable del tipo lista
 * 
 * @param hashmap Tabla hash donde se añadirá la variable
 * @param nombre Nombre de la lista
 * 
 * @return int Si todo salio bien
 */
int declararLista(HashMap* hashmap, char* nombre)
{
	printf("Declarando %s\n",nombre);
	if(buscar(hashmap,nombre))
	{
		printf("Lista ya declarada\n");
		return 0;
	}
	insert(hashmap,nombre);
	Token token;
	token.tipoToken = "LISTA";
	token.cadena = "";
	asignarValorHash(hashmap,nombre,token);
	
	return 1;
}

/**
 * @brief Borrar valor
 * @ingroup funciones_hashtable
 * 
 * Método que busca una variable en una tabla hash y borra el tipo de valor que contiene
 * 
 * @param hashmap Tabla hash donde se buscará la variable
 * @param nombre Variable a la cual se le quitará el valor
 * 
 * @return Si se encontró la variable en la tabla hash
 */
int borrarValorHash(HashMap* hashmap, char* nombre)
{
	int bucketIndex = hashFunction(hashmap,nombre);

	Variable* bucketHead = hashmap->arr[bucketIndex];

	while(bucketHead != NULL)
	{
		if(!strcmp(bucketHead->nombre,nombre))
		{
			strcpy(bucketHead->token.tipoToken,"");
			strcpy(bucketHead->token.cadena,"");

			//printf("Valor de %s borrado\n",bucketHead->nombre);
			return 1;
		}

		bucketHead = bucketHead->next;
	}

	return 0;
}

/**
 * @brief Imprimir variable
 * @ingroup funciones_hashtable
 * 
 * Método para imprimir una variable
 * 
 * @param hashmap Tabla hash donde se buscará la variable
 * @param nombre Variable a buscar
 */
void printVariable(HashMap* hashmap, char* nombre)
{
	int bucketIndex = hashFunction(hashmap,nombre);

	Variable* bucketHead = hashmap->arr[bucketIndex];

	while(bucketHead != NULL)
	{
		if(!strcmp(bucketHead->nombre,nombre))
		{
			printf("Nombre: %s\nTipoToken: %s\nValor: %s\n",bucketHead->nombre,bucketHead->token.tipoToken,bucketHead->token.cadena);
			return;
		}

		bucketHead = bucketHead->next;
	}

	//printf("\nVariable no declarada\n");
}

/**
 * @brief Intermedia entre métodos
 * @ingroup funciones_pila
 * 
 * Método que manda a llamar una declaración o búsqueda depende el parámetro declaracion
 * 
 * @param scopes Tabla de simbolos
 * @param nombre Variable
 * @param token Valor
 * @param declaracion Tipo de declaración que se hará
 * 
 * @return Si todo salio bien
 */
int intermedia(Pila *scopes, char* nombre, Token token, int declaracion)
{
	if(declaracion == 1)
	{
		if(buscar(scopes->head->scope,nombre))
		{
			printf("Variable ya declarada\n");
			return 0;
		}
		insert(scopes->head->scope,nombre);
		//insert(copia.head->scope,nombre);
	}
	else
	{
		return buscarPila(scopes,nombre);
		//asignarValor(hashmap,nombre,token);
	}
}

/**
 * @brief Borrar hash
 * @ingroup funciones_hashtable
 * 
 * Método que borra toda una tabla hash
 * 
 * @param hashmap Tabla hash a borrar
 */
void borrarHash(HashMap* hashmap)
{
	if(!hashmap) return;

	for(int i = 0; i < hashmap->capacity; i++)
	{
		Variable* actual = hashmap->arr[i];
		while(actual != NULL)
		{
			Variable* temp = actual;
			actual = actual->next;

			free(temp->nombre);
			temp->nombre = NULL;
			free(temp->token.tipoToken);
			temp->token.tipoToken = NULL;
			free(temp->token.cadena);
			temp->token.cadena = NULL;
			free(temp);
		}
	}

	free(hashmap->arr);
	free(hashmap);
}

/**
 * @brief Crear nodo pila
 * @ingroup funciones_pila
 * 
 * Método que crea un nodo para la pila-tabla de símbolos
 * 
 * @param scope Tabla hash que se añadirá al nodo
 * 
 * @return NodoPila* Apuntador al nodo pila creado
 */
NodoPila* crearNodoPila(HashMap *scope)
{
	NodoPila* nuevoNodo = (NodoPila*)calloc(sizeof(NodoPila),1);
	nuevoNodo->scope = scope;
	nuevoNodo->next = NULL;

	return nuevoNodo;
}

/**
 * @brief Inicializar pila
 * @ingroup funciones_pila
 * 
 * Método para inicializar una pila
 * 
 * @param pila Pila a inicializar
 */
void inicializarPila(Pila *pila)
{
	pila->head = NULL;
}

/**
 * @brief Esta vacia
 * @ingroup funciones_pila
 * 
 * Método que revisa si una pila-tabla de símbolos está vacía
 * 
 * @param pila Pila a revisar
 * 
 * @return int Si esta vacía o no
 */
int estaVacia(Pila *pila)
{
	return pila->head == NULL;
}

/**
 * @brief Push
 * @ingroup funciones_pila
 * 
 * Método para realizar un push de una tabla hash a una pila-tabla de símbolos
 * 
 * @param pila Pila donde se hará el push
 * @param scope Tabla hash que se añadirá
 */
void push(Pila *pila, HashMap *scope)
{
	NodoPila *nuevo = crearNodoPila(scope);

	if(!nuevo)
	{
		printf("\nStack overflow");
		return;
	}

	nuevo->next = pila->head;
	pila->head = nuevo;
}

/**
 * @brief Pop
 * @ingroup funciones_pila
 * 
 * Método que realiza un pop sin borrar la cima de la pila
 * 
 * @param pila Pila que se hará el pop
 */
void pop(Pila *pila)
{
	if(estaVacia(pila))
	{
		printf("\nStack overflow");
		return;
	}
	else
	{
		NodoPila* temp = pila->head;
		pila->head = pila->head->next;

		//if(temp->scope != NULL)
			//borrarHash(temp->scope);
		free(temp);
	}
}

/**
 * @brief Pop completo
 * @ingroup funciones_pila
 * 
 * Método que realiza un pop que si borra la cima de la pila
 * 
 * @param pila Pila que se hará el pop
 */
void popCompleto(Pila *pila)
{
	if(estaVacia(pila))
	{
		printf("\nStack overflow");
		return;
	}
	else
	{
		NodoPila* temp = pila->head;
		pila->head = pila->head->next;

		if(temp->scope != NULL)
			borrarHash(temp->scope);
		free(temp);
	}
}

/**
 * @brief Vaciar pila
 * @ingroup funciones_pila
 * 
 * Método para vaciar la pila sin borrar nada
 * 
 * @param pila Pila a vaciar
 */
void vaciarPila(Pila *pila)
{
	if(!estaVacia(pila))
	{
		while(pila->head != NULL)
		{
			pop(pila);
		}
	}

	//free(pila);
}
/**
 * @brief Vaciar pila completo
 * @ingroup funciones_pila
 * 
 * Método para vaciar la pila que si borrar todo
 * 
 * @param pila Pila a vaciar
 */
void vaciarPilaCompleto(Pila *pila)
{
	if(!estaVacia(pila))
	{
		while(pila->head != NULL)
		{
			popCompleto(pila);
		}
	}

	//free(pila);
}
	
/**
 * @brief Buscar variable en pila
 * @ingroup funciones_pila
 * 
 * Método que busca dentro de toda la pila y en cada tabla hash de esta una variable
 * 
 * @param pila Pila-Tabla de símbolos donde se buscará
 * @param nombre Variable a buscar
 * 
 * @return int Si todo está bien
 */
int buscarPila(Pila* pila, char* nombre)
{
	//printf("Iniciando busqueda en pila\n");
	NodoPila *temp = pila->head;
	while(temp != NULL)
	{
		if(buscar(temp->scope,nombre))
			return 1;

		temp = temp->next;
	}

	printf("La variable %s no está declarada en este scope\n",nombre);
	return 0;
}

/**
 * @brief Borrar valor pila
 * @ingroup funciones_pila
 * 
 * Método que borra el valor de una variable y la busca en toda la pila
 * 
 * @param pila Pila-Tabla de símbolos donde se buscará
 * @param nombre Variable que valor se borrará
 */
void borrarValor(Pila *pila, char* nombre)
{
	NodoPila *temp = pila->head;
	while(temp != NULL)
	{
		if(borrarValorHash(temp->scope,nombre))
			return;

		temp = temp->next;
	}

	printf("La variable %s no está declarada en este scope\n",nombre);
}

/**
 * @brief Asignar valor pila
 * @ingroup funciones_pila
 * 
 * Método que asigna un valor a una variable y la busca en toda la pila
 * 
 * @param pila Pila-Tabla de símbolos donde se buscará
 * @param nombre Variable a buscar
 */
void asignarValor(Pila *pila, char* nombre,Token token)
{
	NodoPila *temp = pila->head;
	while(temp != NULL)
	{
		if(asignarValorHash(temp->scope,nombre,token))
			return;

		temp = temp->next;
	}

	printf("La variable %s no está declarada en este scope\n",nombre);
}

/**
 * @brief Regresar valor
 * @ingroup funciones_pila
 * 
 * Método que busca una variable en la tabla de símbolos y regresa el tipo de variable que es
 * 
 * @param pila Tabla de simbolos donde se buscará
 * @param nombre Variable a buscar
 * 
 * @return Token* Apuntador al tipo de valor de la variable
 */
Token* regresarValor(Pila *pila, char* nombre)
{

	//printf("Iniciando busqueda en pila\n");
	NodoPila *temp = pila->head;
	Token* token;
	while(temp != NULL)
	{
		token = regresar(temp->scope,nombre);

		if(token != NULL)
			return token;

		temp = temp->next;
	}

	printf("La variable %s no está declarada en este scope\n",nombre);
	return NULL;
}

/**
 * @brief Imprimir tabla completa
 * @ingroup funciones_pila
 * 
 * Método que imprime la copia de la tabla de símbolos
 */
void printTabla()
{
	if(copia.head == NULL)
	{
		printf("La tabla esta vacia\n");
		return;
	}

	NodoPila* actual = copia.head;
	int numHashMap = 1;

	while(actual)
	{
		printf("========Scope %d==========\n",numHashMap);
		HashMap* mapa = actual->scope;

		if(mapa->numOfElements > 0)
		{
			for(int i = 0; i < mapa->capacity;i++)
			{
				Variable* var = mapa->arr[i];
				while(var)
				{
					printf("Nombre: %s\n",var->nombre);
					printf("\tTipoDato: %s\n",var->token.tipoToken);
					printf("\tValor: %s\n",var->token.cadena);
					var = var->next;
				}
			}
		} else {
			printf("HashMap vacio\n");
		}

		actual = actual->next;
		numHashMap++;
	}

	printf("===============FIN============\n");
}
