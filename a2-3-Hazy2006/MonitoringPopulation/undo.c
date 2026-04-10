#define _CRT_SECURE_NO_WARNINGS
#include "undo.h"
#include "vector.h"
UndoStack createUndoStack()
{
	UndoStack stack;
	stack.current = 0;
	stack.size = 0;
	return stack;
}

void pushTop(UndoStack* stack, Vector* current)
{
	if (stack->current < stack->size)
	{
		for (int i = stack->current; i < stack->size; i++)
			destroyVector(&stack->history[i]);
		stack->size = stack->current;
	}
	stack->history[stack->size] = createVector(current->capacity);
	for (int i = 0; i < current->size; i++)
		vectorAdd(&stack->history[stack->size], vectorGet(current, i));
	stack->size++;
	stack->current = stack->size;
}

int undo(UndoStack* stack, Vector* current)
{
	if (stack->current > 1)
	{
		stack->current--;
		destroyVector(current);
		*current = createVector(stack->history[stack->current - 1].capacity);
		for (int i = 0; i < stack->history[stack->current - 1].size; i++)
			vectorAdd(current, vectorGet(&stack->history[stack->current - 1], i));
		return SUCCESS;
	}
	return ERROR_NOT_FOUND;
}

int redo(UndoStack* stack, Vector* current)
{
	if (stack->current < stack->size)
	{
		destroyVector(current);
		*current = createVector(stack->history[stack->current].capacity);
		for (int i = 0; i < stack->history[stack->current].size; i++)
			vectorAdd(current, vectorGet(&stack->history[stack->current], i));
		stack->current++;
		return SUCCESS;
	}
	return ERROR_NOT_FOUND;
}

void destroyUndoStack(UndoStack* stack)
{
	for (int i = 0; i < stack->size; i++)
		destroyVector(&stack->history[i]);
	stack->size = 0;
	stack->current = 0;
}