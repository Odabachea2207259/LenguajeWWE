#include "Listas.h"

void inicializarArray(Lista* lista)
{
    lista->size = 10;
    lista->ultimo = 0;
    lista->arr = (float*)calloc(lista->size,sizeof(float));
}

void pushLista(Lista* lista,float num)
{
    lista->arr[lista->ultimo] = num;
    lista->ultimo++;

    if(lista->ultimo >= lista->size)
    {
        lista->size *= 2;
        lista->arr = (float*)realloc(lista->arr,lista->size*sizeof(float));
        for(int i = lista->ultimo; i < lista->size; i++)
        {
            lista->arr[i] = 0;
        }
    }
}

float getUltimo(Lista* lista)
{
    return lista->arr[lista->ultimo - 1];
}

float getPrimero(Lista* lista)
{
    return lista->arr[0];
}

float getSize(Lista* lista)
{
    return lista->ultimo;
}