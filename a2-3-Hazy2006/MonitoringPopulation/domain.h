#pragma once
#ifndef DOMAIN_H
#define DOMAIN_H

typedef struct
{
	char name[100];
	char continent[20];
	int population;

}Country;

Country createCountry(char* name, char* continent, int population);


#endif // !DOMAIN.h
