#define _CRT_SECURE_NO_WARNINGS
#include "domain.h"
#include <string.h>

/// Create a Country struct with the given name, continent, and population
Country createCountry(char* name, char* continent, int population)
{
	Country c;
	strcpy(c.name, name);
	strcpy(c.continent, continent);
	c.population = population;
	return c;
}