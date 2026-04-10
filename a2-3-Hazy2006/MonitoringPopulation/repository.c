#define _CRT_SECURE_NO_WARNINGS
#include "repository.h"
#include <string.h>
#include <stdlib.h>

/// Create a new repository with an initial capacity for countries
Repository createRepo()
{
	Repository repo;
	repo.countries = createVector(2);
	return repo;
}
/// Add a country to the repository
int repo_addCountry(Repository* repo, Country c)
{
	if (repo_findByName(repo, c.name) != NULL)
		return ERROR_DUPLICATE;
	return vectorAdd(&repo->countries, c);
}
/// Delete a country by name
int repo_deleteCountry(Repository* repo, char* name)
{
    int i, pos = -1;
    int size = vectorSize(&repo->countries);
    for (i = 0; i < size; i++)
        if (strcmp(vectorGet(&repo->countries, i).name, name) == 0)
            pos = i;
    if (pos == -1)
        return ERROR_NOT_FOUND;
    for (i = pos; i < size - 1; i++)
        vectorSet(&repo->countries, i, vectorGet(&repo->countries, i + 1));
    repo->countries.size--;
    return SUCCESS;
}
/// Update a country's continent and population by name
int repo_updateCountry(Repository* repo, char* name, char* newContinent, int newPopulation)
{
    int i, size = vectorSize(&repo->countries);
    for (i = 0; i < size; i++)
        if (strcmp(vectorGet(&repo->countries, i).name, name) == 0)
        {
            Country updated = vectorGet(&repo->countries, i);
            strcpy(updated.continent, newContinent);
            updated.population = newPopulation;
            vectorSet(&repo->countries, i, updated);
            return SUCCESS;
        }
    return ERROR_NOT_FOUND;
}

Country* repo_getAll(Repository* repo)
{
    return repo->countries.elems;
}
/// Find a country by name
Country* repo_findByName(Repository* repo, char* name)
{
    int i, size = vectorSize(&repo->countries);
    for (i = 0; i < size; i++)
        if (strcmp(vectorGet(&repo->countries, i).name, name) == 0)
            return &repo->countries.elems[i];
    return NULL;
}

int repo_getSize(Repository* repo)
{
    return vectorSize(&repo->countries);
}
/// <summary>
/// preventing memory leaks by destroying the vector of countries when the repository is destroyed.
/// This ensures that all dynamically allocated memory for 
/// the countries is properly freed when the repository is no longer needed.
/// </summary>
/// <param name="repo"></param>
void destroyRepo(Repository* repo)
{
	destroyVector(&repo->countries);
}

