#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../service.h"
#include "../domain.h"
#include "test.h"

void testCreateCountry()
{
    Country c = createCountry("Romania", "Europe", 19);
    assert(strcmp(c.name, "Romania") == 0);
    assert(strcmp(c.continent, "Europe") == 0);
    assert(c.population == 19);
    printf("testCreateCountry passed.\n");
}
void test_addMultipleCountries()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    Country c2 = createCountry("Italy", "Europe", 60);
    assert(addCountry(&service, c1) == SUCCESS);
    assert(addCountry(&service, c2) == SUCCESS);
    assert(getSize(&service) == 2);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_addMultipleCountries passed.\n");
}

void test_deleteCountry()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    Country c2 = createCountry("Italy", "Europe", 60);
    addCountry(&service, c1);
    addCountry(&service, c2);
    assert(deleteCountry(&service, "Romania") == SUCCESS);
    assert(getSize(&service) == 1);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_deleteCountry passed.\n");
}

void test_repo_findByName()
{
    Repository repo = createRepo();
    Country c1 = createCountry("Romania", "Europe", 19);
    repo_addCountry(&repo, c1);
    Country* found = repo_findByName(&repo, "Romania");
    assert(found != NULL);
    assert(strcmp(found->name, "Romania") == 0);
    destroyRepo(&repo);
    printf("test_repo_findByName passed.\n");
}

void testGetByName()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    addCountry(&service, c1);
    int count;
    Country* results = getByName(&service, "Romania", &count);
    assert(count == 1);
    assert(strcmp(results[0].name, "Romania") == 0);
    free(results);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("testGetByName passed.\n");
}

void testGetByContinent()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    Country c2 = createCountry("Italy", "Europe", 60);
    addCountry(&service, c1);
    addCountry(&service, c2);
    int count;
    int threshold = 20;
    Country* countries = getByContinent(&service, "Europe", &threshold, &count);
    assert(count == 1);
    assert(strcmp(countries[0].name, "Italy") == 0);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("testGetByContinent passed.\n");
}

void test_updateCountry()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    addCountry(&service, c1);
    assert(updateCountry(&service, "Romania", "Europe", 20) == SUCCESS);
    int count;
    Country* results = getByName(&service, "Romania", &count);
    assert(results[0].population == 20);
    free(results);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_updateCountry passed.\n");
}

void test_migrate()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 30);
    Country c2 = createCountry("Italy", "Europe", 60);
    addCountry(&service, c1);
    addCountry(&service, c2);
    assert(migrate(&service, "Romania", "Italy", 5) == SUCCESS);
    int count;
    Country* romania = getByName(&service, "Romania", &count);
    Country* italy = getByName(&service, "Italy", &count);
    assert(romania[0].population == 25);
    assert(italy[0].population == 65);
    free(romania);
    free(italy);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_migrate passed.\n");
}

void test_undoRedo()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    pushTop(&service.undoStack, &service.repo->countries);
    Country c1 = createCountry("Romania", "Europe", 19);
    addCountry(&service, c1);
    assert(getSize(&service) == 1);
    undoOperation(&service);
    assert(getSize(&service) == 0);
    redoOperation(&service);
    assert(getSize(&service) == 1);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_undoRedo passed.\n");
}

void test_duplicateCountry()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    Country c1 = createCountry("Romania", "Europe", 19);
    assert(addCountry(&service, c1) == SUCCESS);
    assert(addCountry(&service, c1) == ERROR_DUPLICATE);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_duplicateCountry passed.\n");
}

void test_deleteNonExistent()
{
    Repository repo = createRepo();
    Service service = createService(&repo);
    assert(deleteCountry(&service, "NonExistent") == ERROR_NOT_FOUND);
    destroyUndoStack(&service.undoStack);
    destroyRepo(&repo);
    printf("test_deleteNonExistent passed.\n");
}

void runAllTests()
{
    printf("\n=== Running All Tests ===\n\n");
    testCreateCountry();
    test_addMultipleCountries();
    test_deleteCountry();
    test_updateCountry();
    test_repo_findByName();
    testGetByName();
    testGetByContinent();
    test_migrate();
    test_undoRedo();
    test_duplicateCountry();
    test_deleteNonExistent();
    printf("\n=== All Tests Passed! ===\n\n");
}