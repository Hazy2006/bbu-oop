#pragma once
#ifndef UNDO_H
#define UNDO_H

#include "vector.h"

typedef struct {
	Vector history[100];
	int current;
	int size;
} UndoStack;

UndoStack createUndoStack();
void pushTop(UndoStack* stack, Vector* current);
int undo(UndoStack* stack, Vector* current);
int redo(UndoStack* stack, Vector* current);
void destroyUndoStack(UndoStack* stack);

#endif