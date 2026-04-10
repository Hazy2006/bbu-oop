#pragma once
#ifndef SERVICE_H
#define SERVICE_H

#include "repository.h"
#include "undo.h"

typedef struct {
	Repository* repo;
	UndoStack undoStack;
}Service;

int addCountry(Service* service, Country c);
int deleteCountry(Service* service, char* name);
int updateCountry(Service* service, char* name, char* newContinent, int newPopulation);
Service createService(Repository* repo);
Country* getByName(Service* service, char* name, int* count);
int migrate(Service* service, char* from, char* to, int amount);
int getSize(Service* service);
Country* getAllCountries(Service* service);
Country* getByContinent(Service* service, char* continent, int* threshold, int* count);
int compareByPopulation(const void* a, const void* b);
void undoOperation(Service* service);
void redoOperation(Service* service);
#endif
