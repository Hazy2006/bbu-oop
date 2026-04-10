#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include "../domain.h"

#define SUCCESS 0
#define ERROR_NOT_FOUND -1
#define ERROR_ARRAY_FULL -2
#define ERROR_DUPLICATE -3
#define ERROR_INVALID_POPULATION -4

typedef struct {
    Country* elems;
    int size;
    int capacity;
} Vector;

Vector createVector(int capacity);
void destroyVector(Vector* v);
int vectorAdd(Vector* v, Country c);
int vectorSize(Vector* v);
Country vectorGet(Vector* v, int index);
void vectorSet(Vector* v, int index, Country c);
#endif