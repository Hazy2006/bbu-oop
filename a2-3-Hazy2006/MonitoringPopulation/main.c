#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include "service.h"
#include "ui.h"
#include "test.h"
#include <crtdbg.h>
int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Repository repo = createRepo();
    Service service = createService(&repo);

    // 10 hardcoded countries
    addCountry(&service, createCountry("Romania", "Europe", 19));
    addCountry(&service, createCountry("France", "Europe", 68));
    addCountry(&service, createCountry("Germany", "Europe", 84));
    addCountry(&service, createCountry("United States", "North America", 331));
    addCountry(&service, createCountry("China", "Asia", 1412));
    addCountry(&service, createCountry("India", "Asia", 1408));
    addCountry(&service, createCountry("Brazil", "South America", 215));
    addCountry(&service, createCountry("Australia", "Oceania", 26));
    addCountry(&service, createCountry("Japan", "Asia", 125));
    addCountry(&service, createCountry("Egypt", "Africa", 104));
    pushTop(&service.undoStack, &service.repo->countries);
    

	runAllTests();
	run(&service);
	destroyUndoStack(&service.undoStack);
	destroyRepo(&repo);
	return 0;
}