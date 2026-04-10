#define _CRT_SECURE_NO_WARNINGS
#include "vector.h"
#include <string.h>
#include <stdlib.h>

Vector createVector(int capacity)
{
    Vector v;
    v.elems = (Country*)malloc(capacity * sizeof(Country));
    if (v.elems == NULL)
    {
        v.size = 0;
        v.capacity = 0;
        return v;
    }
    v.size = 0;
    v.capacity = capacity;
    return v;
}

void destroyVector(Vector* v)
{
    free(v->elems);
    v->size = 0;
    v->capacity = 0;
}

int vectorAdd(Vector* v, Country c)
{
    if (v->size >= v->capacity)
    {
        v->capacity *= 2;
        v->elems = (Country*)realloc(v->elems, v->capacity * sizeof(Country));
    }
    v->elems[v->size] = c;
    v->size++;
    return SUCCESS;
}

int vectorSize(Vector* v)
{
    return v->size;
}

Country vectorGet(Vector* v, int index)
{
    return v->elems[index];
}

void vectorSet(Vector* v, int index, Country c)
{
    v->elems[index] = c;
}

