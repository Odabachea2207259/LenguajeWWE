/**
 * @file Lexer.c
 * @brief Implementación de métodos del lexer
 * 
 * Este archivo contiene la implementación de los métodos necesarios
 * para el funcionamiento del lexer
 * 
 * @author Alexander Arellano odabachea
 */

#include "Lexer.h"

/**
 * @brief Inputs permitidos
 * 
 * Esta lista de palabras son los inputs permitidos.
 * Hace de simulación de un automata finito.
 */
const char *inputs[][20] = {
	{"r","s","n","a","j","m","h","o","-","<","p",ALFABETO,PUNTO,SIGNOS,NUMEROS,IGUAL,COMPARACION,OPARITMETICO,"\""}, //0 A
	{ALFABETO,NUMEROS},                                              //1 B NXT
	{ALFABETO,NUMEROS},                                              //2 C ANUNCIAR
	{ALFABETO,NUMEROS,"."},                                          //3 D ID poner para que despeus de esto ponga .size o []
	{NUMEROS},                                                       //4 E
	{PUNTO,NUMEROS},                                                 //5 F OPARITMETICO
	{"eE",NUMEROS,PUNTO},                                            //6 G NUM
	{"eE",NUMEROS},                                                  //7 H FLOAT
	{SIGNOS,NUMEROS},                                                //8 I
	{NUMEROS},                                                       //9 J
	{NUMEROS},                                                       //10K FLOAT
	{"a",ALFABETO,NUMEROS},                                          //11  VARIABLE
	{"w","n",ALFABETO,NUMEROS},                                      //12  VARIABLE
	{"n",ALFABETO,NUMEROS},                                          //13  VARIABLE
	{"d",ALFABETO,NUMEROS},                                          //14  VARIABLE
	{"y",ALFABETO,NUMEROS},                                          //15  VARIABLE
	{"r",ALFABETO,NUMEROS},                                          //16  VARIABLE
	{"t",ALFABETO,NUMEROS},                                          //17  VARIABLE
	{"o",ALFABETO,NUMEROS},                                          //18  VARIABLE
	{"n",ALFABETO,NUMEROS},                                          //19  VARIABLE
	{"m",ALFABETO,NUMEROS},                                          //20  VARIABLE
	{"a",ALFABETO,NUMEROS},                                          //21  VARIABLE
	{"c",ALFABETO,NUMEROS},                                          //22  VARIABLE
	{"k",ALFABETO,NUMEROS},                                          //23  VARIABLE
	{"d",ALFABETO,NUMEROS},                                          //24  VARIABLE
	{"o",ALFABETO,NUMEROS},                                          //25  VARIABLE
	{"w",ALFABETO,NUMEROS},                                          //26  VARIABLE
	{"n",ALFABETO,NUMEROS},                                          //27  VARIABLE
	{"e",ALFABETO,NUMEROS},                                          //28  VARIABLE
	{"f",ALFABETO,NUMEROS},                                          //29  VARIABLE
	{"f",ALFABETO,NUMEROS},                                          //30  VARIABLE
	{"a",ALFABETO,NUMEROS},                                          //31  VARIABLE
	{"t",ALFABETO,NUMEROS},                                          //32  VARIABLE
	{"t",ALFABETO,NUMEROS},                                          //33  VARIABLE
	{"a",ALFABETO,NUMEROS},                                          //34  VARIABLE
	{"r",ALFABETO,NUMEROS},                                          //35  VARIABLE
	{"d",ALFABETO,NUMEROS},                                          //36  VARIABLE
	{"y",ALFABETO,NUMEROS},                                          //37  VARIABLE
	{"n",ALFABETO,NUMEROS},                                          //38  VARIABLE
	{"u",ALFABETO,NUMEROS},                                          //39  VARIABLE
	{"n",ALFABETO,NUMEROS},                                          //40  VARIABLE
	{"c",ALFABETO,NUMEROS},                                          //41  VARIABLE
	{"i",ALFABETO,NUMEROS},                                          //42  VARIABLE
	{"a",ALFABETO,NUMEROS},                                          //43  VARIABLE
	{"r",ALFABETO,NUMEROS},                                          //44  VARIABLE
	{"x","e",ALFABETO,NUMEROS},                                      //45  VARIABLE AGREGAR PARA nexus
	{"t",ALFABETO,NUMEROS},                                          //46  VARIABLE
	{"="},                                                           //47 IGUAL      
	{ALFABETO,NUMEROS},                                              //48 RAW
	{ALFABETO,NUMEROS},                                              //49 SMACKDOWN
	{ALFABETO,NUMEROS},                                              //50 RANDY
	{ALFABETO,NUMEROS},                                              //51 ORTON
	{ALFABETO,NUMEROS},                                              //52 JEFF
	{ALFABETO,NUMEROS},                                              //53 MATT
	{ALFABETO,NUMEROS},                                              //54 HARDY
	{">",NUMEROS},                                                   //55 OPARITMETICO
	{"-","=",">"},                                                   //56 COMPARACION MENOR QUE
	{},                                                              //57 INICIO-ANUNCIAR
	{},                                                              //58 FIN-ANUNCIAR
	{"="},                                                           //59 COMPARACION
	{},               						 //60 OPARITMETICO
	{},                                                              //61 COMPARACION IGUAL QUE
	{"\"",ALFABETO,NUMEROS," ",SIGNOS,OPARITMETICO,COMPARACION,PUNTO,PUNTUACION,IGUAL,ESPECIALES},//62 
	{},               						 //63 STRING
	{"x",ALFABETO,NUMEROS},						 //64 VARIABLE
	{"u",ALFABETO,NUMEROS},             				 //65 VARIABLE
	{"s",ALFABETO,NUMEROS},                                          //66 VARIABLE
	{ALFABETO,NUMEROS},           					 //67 NEXUS
	{"s","l","f"},					                 //68
	{},								 //69 SIZE
	{},              						 //70 LAST
	{},								 //71 FIRST
	{"r",ALFABETO,NUMEROS},					         //72 VARIABLE
	{"o",ALFABETO,NUMEROS},						 //73 VARIABLE
	{"m",ALFABETO,NUMEROS},						 //74 VARIABLE
	{"o",ALFABETO,NUMEROS},						 //75 VARIABLE
	{}};								 //76 PROMO

	/**
	 * @brief Movimientos permitidos
	 * 
	 * Tiene la secuencia de caracteres permitida
	 */
const int mov2[][20] = {
	{11,20,45,38,28,31,34,16,55,56,72,3,68,5,6,47,59,60,62}, //A 0
	{3,3},                                //B 1
	{3,3},                                //C 2
	{3,3,68},                             //D 3
	{7},                                  //E 4
	{4,6},                                //F 5
	{8,6,7},                              //G 6
	{8,7},                                //H 7
	{9,10},                               //I 8
	{10},                                 //J 9
	{10},                                 //K 10
	{12,3,3},                             //  11
	{48,14,3,3},                          //  12
	{3,3,3},                              //  13
	{15,3,3},                             //  14
	{50,3,3},                             //  15
        {17,3,3},                             //  16
	{18,3,3},                             //  17
	{19,3,3},                             //  18
	{51,3,3},                             //  19
	{21,3,3},                             //  20
	{22,3,3},                             //  21
	{23,3,3},                             //  22
	{24,3,3},                             //  23
	{25,3,3},                             //  24
	{26,3,3},                             //  25
	{27,3,3},                             //  26
	{49,3,3},                             //  27
	{29,3,3},                             //  28
	{30,3,3},                             //  29
	{52,3,3},                             //  30
	{32,3,3},                             //  31
	{33,3,3},                             //  32
	{53,3,3},                             //  33
	{35,3,3},                             //  34
	{36,3,3},                             //  35
	{37,3,3},                             //  36
	{54,3,3},                             //  37
	{39,3,3},                             //  38
	{40,3,3},                             //  39
	{41,3,3},                             //  40
	{42,3,3},                             //  41
	{43,3,3},                             //  42
	{44,3,3},                             //  43
	{2,3,3},                              //  44
	{46,64,3,3},                          //  45
	{1,3,3},                              //  46
	{61},                                 //  47
	{3,3},                                //  48
	{3,3},                                //  49
	{3,3},                                //  50
	{3,3},                                //  51
	{3,3},                                //  52
	{3,3},                                //  53
	{3,3},                                //  54
	{57,6},                               //  55
	{58,61,61},                           //  56
	{},                                   //  57
	{},                                   //  58
	{61},                                 //  59
	{},                                   //  60
	{},                                   //  61
	{63,62,62,62,62,62,62,62,62,62,62},   //  62
	{},                                   //  63
	{65,3,3},                             //  64
	{66,3,3}, 			      //  65
	{67,3,3},                             //  66
	{3,3},   			      //  67
	{69,70,71},		              //  68
	{},        			      //  69
	{},				      //  70
	{},				      //  71
	{73,3,3},                             //  72
	{74,3,3},			      //  73
	{75,3,3},          	   	      //  74
	{76,3,3},			      //  75
	{}};				      //  76


/**
 * @brief Siguiente estado
 * @ingroup tokenizer
 * 
 * Este método Revisa el estado actual y el input recibido y retorna el siguiente estado.
 * 
 * @param currentState Estado actual de los inputs
 * @param input Caracter recibido
 * 
 * @return int Del estado siguiente
 */
int nextState(int currentState, char input) {
	int nextState = currentState;
	int found = 0;

	for(int i = 0;*(inputs[currentState][i]) != '\0'; i++) {
		if(correctInput(inputs[currentState][i],input))
		{
			nextState = mov2[currentState][i];
			found = 1;
			break;
		}
	}
	
	if(!found)
		return -1;

	return nextState;
}

/**
 * @brief Input correcto
 * @ingroup tokenizer
 * 
 * Este método revisa si el input que se ingreso esta dentro de lo permitido
 * 
 * @param goodInputs Lista de palabras que están permitidas
 * @param input Caracter recibido
 * 
 * @return int Si es un input correcto
 */
int correctInput(const char *goodInputs, char input) {
	int boolean = 0;

	for(int i = 0; i < strlen(goodInputs); i++) {
		if(input == goodInputs[i]) boolean = 1;
	}

	return boolean;
}

/**
 * @brief Retornar token
 * @ingroup tokenizer
 * 
 * Este método revisa el último estado donde se encuentra
 * y depende de este será el tipo de token que se regresará
 * 
 * @param finalState Estado final donde se quedó la palabra
 * 
 * @return Token* Apuntador al token creado
 */
Token* isToken(int finalState) {
	
	Token *token = (Token*)malloc(sizeof(Token));
	
	if((finalState >= 11 && finalState <= 46) || (finalState >= 64 && finalState <= 66) || (finalState >= 72 && finalState <= 75))
	{
		//printf("Token: VARIABLE\n");
		token->tipoToken = strdup("VARIABLE");
		return token;
	}

	char *cadena;

	switch(finalState) {
		case 1:
			//printf("Token: INICIALIZAR\n");
			cadena = strdup("INICIALIZAR");
			break;
		case 2:
			//printf("Token: ANUNCIAR\n");
			cadena = strdup("ANUNCIAR");
			break;
		case 3:
			//printf("Token: VARIABLE\n");
			cadena = strdup("VARIABLE");
			break;
		case 5:
		case 55:
		case 60:
			//printf("Token: OPARITMETICO\n");
			cadena = strdup("OPARITMETICO");
			break;
		case 6:
			//printf("Token: NUM\n");
			cadena = strdup("NUM");
			break;
		case 7:
		case 10:
			//printf("Token: FLOAT\n");
			cadena = strdup("FLOAT");
			break;

		case 47:
			//printf("Token: IGUAL\n");
			cadena = strdup("IGUAL");
			break;
		case 48:
			//printf("Token: INICIO-PROGRAMA\n");
			cadena = strdup("INICIO-PROGRAMA");
			break;
		case 49:
			//printf("Token: FIN-PROGRAMA\n");
			cadena = strdup("FIN-PROGRAMA");
			break;
		case 50:
			//printf("Token: MIENTRAS\n");
			cadena = strdup("MIENTRAS");
			break;
		case 51:
			//printf("Token: FIN-MIENTRAS\n");
			cadena = strdup("FIN-MIENTRAS");
			break;
		case 52:
			//printf("Token: SI\n");
			cadena = strdup("SI");
			break;
		case 53:
			//printf("Token: SINO\n");
			cadena = strdup("SINO");
			break;
		case 54:
			//printf("Token: FIN-SI\n");
			cadena = strdup("FIN-SI");
			break;
		case 56:
		case 59:
		case 61:
			//printf("Token: COMPARACION\n");
			cadena = strdup("COMPARACION");
			break;
		case 57:
			//printf("Token: INICIO-ANUNCIAR\n");
			cadena = strdup("INICIO-ANUNCIAR");
			break;
		case 58:
			//printf("Token: FIN-ANUNCIAR\n");
			cadena = strdup("FIN-ANUNCIAR");
			break;
		case 63:
			//printf("Token: STRING\n");
			cadena = strdup("STRING");
			break;
		case 67:
			//printf("Token: NEXUS\n");
			cadena = strdup("NEXUS");
			break;
		case 69:
			//printf("Token: SIZE\n");
			cadena = strdup("SIZE");
			break;
		case 70:
			cadena = strdup("LAST");
			break;
		case 71:
			cadena = strdup("FIRST");
			break;
		case 76:
			cadena = strdup("PROMO");
			break;
		default:
			//printf("Token: NOT_TOKEN\n");
			cadena = strdup("NOT_TOKEN");
			break;
	}

	token->tipoToken = cadena;

	return token;
}

/**
 * @brief Tokenizer
 * @ingroup tokenizer
 * 
 * Este método revisa una palabra y retorna el token que es
 * 
 * @param str Palabra a revisar
 * 
 * @return Token* Apuntador al nuevo token
 */
Token* lexer(char *str)
{
	//char states[] = "ABCDEFGHIJK";
	int states[] = {0,1,2,3,4,5,6,7,8,9,10};
	
	//char currentState = 'A';
	int currentState = 0;

	for(int i = 0; i < strlen(str); i++) {
		//printf("Current state: %d\n",currentState);

		//PASAR EL ESTADO ACTUAL Y EL INPUT(EL CHAR EN EL QUE ESTAMOS)
		currentState = nextState(currentState,str[i]);

		//if(currentState == ERROR) {
		if(currentState == -1) {
			printf("Token: NOT_TOKEN\n");
			break;
		}

	}

	return isToken(currentState);
}
