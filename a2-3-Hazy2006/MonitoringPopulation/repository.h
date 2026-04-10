#pragma once
#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "vector.h"

typedef struct {
    Vector countries;
} Repository;

int repo_addCountry(Repository* repo, Country c);
int repo_deleteCountry(Repository* repo, char* name);
int repo_updateCountry(Repository* repo, char* name, char* newContinent, int newPopulation);
Repository createRepo();
Country* repo_findByName(Repository* repo, char* name);
Country* repo_getAll(Repository* repo);
int repo_getSize(Repository* repo);
void destroyRepo(Repository* repo);

#endif