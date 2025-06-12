#include <stdio.h>
#include <stdlib.h>

typedef struct Lista
{
    float *arr;
    int ultimo;
    int size;
}Lista;

void inicializarArray(Lista* lista);
void pushLista(Lista* lista,float num);
float getUltimo(Lista* lista);
float getPrimero(Lista* lista);
float getSize(Lista* lista);