#define _CRT_SECURE_NO_WARNINGS
#include "service.h"
#include "repository.h"
#include <string.h>
#include <stdlib.h>


Service createService(Repository* repo)
{
	Service service;
	service.repo = repo;
    service.undoStack = createUndoStack();
	return service;
}


/// Add a country to the service
int addCountry(Service* service, Country c)
{
    if (c.population <= 0)
        return ERROR_INVALID_POPULATION;
    int result = repo_addCountry(service->repo, c);
    if (result == SUCCESS)
        pushTop(&service->undoStack, &service->repo->countries);
    return result;
}
/// Delete a country by name
int deleteCountry(Service* service, char* name)
{
    int result = repo_deleteCountry(service->repo, name);
    if (result == SUCCESS)
        pushTop(&service->undoStack, &service->repo->countries);
    return result;
}
/// Update a country's continent and population by name
int updateCountry(Service* service, char* name, char* newContinent, int newPopulation)
{
    if (newPopulation <= 0)
        return ERROR_INVALID_POPULATION;
    int result = repo_updateCountry(service->repo, name, newContinent, newPopulation);
    if (result == SUCCESS)
        pushTop(&service->undoStack, &service->repo->countries);
    return result;
}

/// Get countries by name
/// @param service The service containing the repository and undo stack
/// @param name The name to search for
/// @param count Pointer to store the number of countries found
/// @return Array of countries matching the name
Country* getByName(Service* service, char* name, int* count)
{
    Country* countries = repo_getAll(service->repo);
    int size = service->repo->countries.size;
    int localCount = 0;

    for (int i = 0; i < size; i++)
        if (strstr(countries[i].name, name) != NULL)
            localCount++;

    if (count != NULL)
        *count = localCount;

    if (localCount == 0)
        return NULL;

    Country* results = malloc(localCount * sizeof(Country));
    int idx = 0;
    for (int i = 0; i < size; i++)
        if (strstr(countries[i].name, name) != NULL)
            results[idx++] = countries[i];

    return results;
}

/// <summary>
/// Operation for migration
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
int compareByPopulation(const void* a, const void* b)
{
    Country* countryA = (Country*)a;
	Country* countryB = (Country*)b;
    return countryA->population - countryB->population;
}

/// Get countries by continent and population threshold
/// @param service The service containing the repository and undo stack
/// @param continent The continent to filter by (use "all" for no filtering)
/// @param threshold The population threshold
/// @param count Pointer to store the number of countries found
/// @return Array of countries matching the criteria
Country* getByContinent(Service* service, char* continent, int* threshold, int* count)
{
	static Country results[100];
	*count = 0;
	Country* countries = repo_getAll(service->repo);
	int size = service->repo->countries.size;
    for (int i = 0; i < size; i++)
    {
        if ((strlen(continent) == 0 || strcmp(continent, "all") == 0 || strcmp(countries[i].continent, continent) == 0)
            && countries[i].population > *threshold)
        {
            results[*count] = countries[i];
            (*count)++;
		}
    }
	qsort(results, *count, sizeof(Country), compareByPopulation);
    return results;
}

/// Migrate population from one country to another
/// @param service The service containing the repository and undo stack
/// @param from The name of the country to migrate from
/// @param to The name of the country to migrate to
/// @param amount The amount of population to migrate (in millions)
/// @return SUCCESS if migration was successful, ERROR_NOT_FOUND if either country doesn't exist or if the source country doesn't have enough population
/// This function performs a migration of population from one country to another. It first checks if both countries exist and if the source country has enough population to migrate. If any of these conditions fail, it returns an error code. If the migration is valid, it updates the 
/// populations of both countries accordingly and pushes the new state onto the undo stack for potential rollback. 

int migrate(Service* service, char* from, char* to, int amount)
{
    Country* fromCountry = repo_findByName(service->repo, from);
    Country* toCountry = repo_findByName(service->repo, to);

    if (fromCountry == NULL || toCountry == NULL)
        return ERROR_NOT_FOUND;

    if (fromCountry->population < amount)
        return ERROR_NOT_FOUND;

    // snapshot before any mutation
    char fromContinent[50], toContinent[50];
    int fromPop = fromCountry->population;
    int toPop = toCountry->population;
    strcpy(fromContinent, fromCountry->continent);
    strcpy(toContinent, toCountry->continent);

    repo_updateCountry(service->repo, from, fromContinent, fromPop - amount);
    repo_updateCountry(service->repo, to, toContinent, toPop + amount);
    pushTop(&service->undoStack, &service->repo->countries);

    return SUCCESS;
}
int getSize(Service* service)
{
    return repo_getSize(service->repo);
}

Country* getAllCountries(Service* service)
{
    return repo_getAll(service->repo);
}
void undoOperation(Service* service)
{
    undo(&service->undoStack, &service->repo->countries);
}

void redoOperation(Service* service)
{
    redo(&service->undoStack, &service->repo->countries);
}

