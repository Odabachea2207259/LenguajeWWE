/**
 * @file Enunciados.h
 * @brief Declaraciones de estructuras, enumeraciones y funciones.
 *
 * Este archivo tiene las declaraciones de cada elemento
 * necesario para la creación de mi programa de enunciados.
 *
 * @author Alexander Arellano Odabachea
 */

 /**
  * @brief Estructura que representa un programa(adelanto).
  */
typedef struct Programa Programa;

/**
 * @brief Tipos de enunciados que almacenare.
 */
typedef enum {
    ENUNC_DECLARACION, /**< Enunciado de declaracion */
    ENUNC_DECLARACION_LISTA, /**< Enunciado de declaracion de listas */
    ENUNC_ASIGNACION, /**< Enunciado de asignacion */
    ENUNC_ASIGNACION_LISTA, /**< Enunciado de asignacion de listas */
    ENUNC_LEER, /**< Enunciado de leer */
    ENUNC_LEER_LISTA, /**< Enunciado de leer una lista */
    ENUNC_ANUNCIAR, /**< Enunciado de anunciar o imprimir */
    ENUNC_SI, /**< Enunciado de si */
    ENUNC_MIENTRAS /**< Enunciado de mientras */
} TipoEnunciado;

/**
 * @brief Tipos de expresiones
 */
typedef enum {
    EXPR_VALOR, /**< Expresion que contiene un solo valor */
    EXPR_BINARIA /**< Expresion que contiene un operador y otra expresion */
} TipoExpresion;

/**
 * @brief Tipos de valores del lenguaje
 */
typedef enum {
    VAL_VARIABLE, /**< Valor tipo variable */
    VAL_NUMERO, /**< Valor tupo numero(float) */
    VAL_LISTA_INDEX, /**< Valor tipo indice de una lista */
    VAL_LISTA_INDEX_VAR, /**< Valor tipo variable que representa el indice de una lista */
    VAL_LISTA_F, /**< Valor tipo PRIMERO de una lista */
    VAL_LISTA_L, /**< Valor tipo ULTIMO de una lista */
    VAL_LISTA_S /**< Valor tipo TAMAÑO de una lista */
} TipoValor;
/**
 * @brief Tipos de operadores
 */
typedef enum {
    OP_SUMA, OP_RESTA, OP_MULT, OP_DIV
} OPAritmetico;

/**
 * @brief Tipos de operadores relaciones
 */
typedef enum {
    OP_MENOR, OP_MAYOR, OP_MENOR_IGUAL, OP_MAYOR_IGUAL,
    OP_DIFERENTE, OP_IGUAL
} OPRelacional;

/**
 * @brief Estructura que contiene todo lo de un valor
 */
typedef struct Valor {
    TipoValor tipo; /**< Tipo de valor que es */
    char* nombre; /**< Nombre de la variable si es de tipo variable */
    float numero; /**< Numero del valor */
    int indice;  /**< Indice si es tipo indice */
    char* var; /**< Nombre de la variable */
} Valor;

/**
 * @brief Estructura que contiene todo lo de una expresion
 */
typedef struct Expresion {
    TipoExpresion tipo; /**< Tipo de expresion */
    /**
     * @brief Almacena el valor de la expresion depende el tipo
     */
    union {
        Valor valor;  /**< Almacena un valor simple si es tipo valor */
        /**
         * Estructura que almacena los valores de una expresion binaria
         */
        struct {
            struct Expresion* izquierda; /**< Expresion izquierda */
            char *oparitmetico; /** Operador aritmetico */
            struct Expresion* derecha; /**< Expresion derecha */
        } binaria; 
    };
} Expresion;

/**
 * @brief Estructura que representa una comparación
 */
typedef struct {
    Valor izquierda; /**< Valor de la izquierda */
    OPRelacional operador; /**< Operador relacional */
    Valor derecha; /**< Valor de la derecha */
} Comparacion;

typedef struct Enunciado Enunciado;

/**
 * @brief Estructura del tipo inicializacion
 */
typedef struct {
    char** variables; /**< Variables a inicializar */
    int cantidad; /** Cantidad de variables */
} Declaracion;

/**
 * @brief Estructura del tipo asignacion
 */
typedef struct {
    Valor variable; /**< Variable a asignar un valor */
    Expresion* expresion; /**< Expresion a asignar */
} Asignacion;

/**
 * @brief Estructura del tipo leer
 */
typedef struct {
    char* variable; /**< Variable a leer desde el teclado */
} Leer;

/**
 * @brief Estructura del tipo anunciar o imprimir
 */
typedef struct {
    int esCadena; /**< Booleano para saber si es una cadena o una expresion */
    union {
        char* cadena;
        Expresion* expresion;
    };
} Anunciar;

/**
 * @brief Estructura del tipo si
 */
typedef struct {
    Comparacion comp; /**< Comparador */
    Programa* entonces; /**< Enunciados dentro del si */
    Programa* si_no; /**< Enunciados dentro del si_no */
} Si;

/**
 * @brief Estructura del tipo mientras
 */
typedef struct {
    Comparacion comp; /**< Comparador */
    Programa* cuerpo; /**< Enunciados del cuerpo de mientras */
} Mientras;

/**
 * @brief Estructura del tipo enunciado que contiene el tipo de enunciado
 */
struct Enunciado {
    TipoEnunciado tipo; /**< Tipo del enunciado */
    union {
        Declaracion declaracion;
        Asignacion asignacion;
        Leer leer;
        Anunciar anunciar;
        Si si;
        Mientras mientras;
    };
    Enunciado* siguiente; /**< Apuntador al enunciado siguiente */
};

/**
 * @brief Estructura del tipo programa.
 * 
 * Contiene una lista de enunciados.
 */
struct Programa{
    Enunciado* lista_enunciados; /**< Lista completa de los enunciados */
    Enunciado* ultimo_enunciado; /**< Apuntador al último enunciado */
    int listas; /**< Booleano para saber si hubo listas inicializadas */
    char **variables; /**< Lista de variables tipo lista inicializadad*/
    int cantidad; /**< Cantidad de listas inicializadas */
};

/**
 * @defgroup funciones_enunciados Funciones del traductor
 * @{
 */

void crearEnunciados(Programa* programa, Tokens* tokens);
int esEnunciado(Programa* programa, Tokens* tokens);
int inicializar(Programa* programa, Tokens* tokens);
int inicializarLista(Programa* programa, Tokens* tokens);
int leer(Programa* programa, Tokens* tokens);
int asignacion_trad(Programa* programa, Tokens* tokens);
int asignacion_lista(Programa* programa, Tokens* tokens);
int si_trad(Programa *programa,Tokens *tokens);
int mientras_trad(Programa *programa,Tokens *tokens);
int anunciar_trad(Programa *programa,Tokens *tokens);
/** @} */

/**
 * @defgroup funciones_tipo Funciones de tipo
 * Funciones que revisan que tipo de valor, operador o expresion es.
 * @{
 */
void expDerecha(Programa* programa, Tokens* tokens, Expresion* derecha);
Valor esValor(Programa* programa, Tokens* tokens);
OPRelacional tipoOperador(char* operador);
/** @} */

/**
 * @defgroup funciones_lista Funciones de la lista enlazada Programa
 * @{
 */
void pushEnunciado(Programa* programa, Enunciado* enunciado);
/** @} */

/**
 * @defgroup funciones_eliminar Funciones para eliminar la lista de enunciados
 * @{
 */
void liberarExpresion(Expresion* expresion);
void liberarValor(Valor *valor);
void eliminarPrograma(Programa* programa);
/** @} */

/**
 * @defgroup funciones_imprimir Funciones para imprimir la lista de enunciados
 * @{
 */
void imprimirExpresion(Expresion* expr);
void imprimirValor(Valor val);
void imprimirEnunciado(Enunciado* e);
void imprimirPrograma(Programa* programa);
void imprimirOp(Comparacion comparacion);
/**@} */