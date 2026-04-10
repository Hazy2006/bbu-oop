#include <iostream>
#include "service.h"
#include "ui.h"
#include "tests.h"

int main()
{
	Repo r;
	Service s{ r };
	UI ui{ s };
	s.add("LearnCPP", "Cherno", 10, 30, 1500, "http://youtube.com/1");
	s.add("PointersCPP", "CodeBeauty", 15, 42, 2300, "http://youtube.com/2");
	s.add("STL_Containers", "BoQian", 22, 18, 3400, "http://youtube.com/3");
	s.add("Templates_Advanced", "CppCon", 8, 55, 950, "http://youtube.com/4");
	s.add("RAII_Explained", "JasonTurner", 12, 20, 1800, "http://youtube.com/5");
	s.add("SmartPointers", "Cherno", 18, 35, 2700, "http://youtube.com/6");
	s.add("Lambdas_Deep_Dive", "MikeShah", 9, 48, 1100, "http://youtube.com/7");
	s.add("Multithreading", "CodeBeauty", 25, 12, 4200, "http://youtube.com/8");
	s.add("Move_Semantics", "BoQian", 14, 28, 2100, "http://youtube.com/9");
	s.add("Const_Correctness", "JasonTurner", 11, 16, 1650, "http://youtube.com/10");
	runAllTests();
	ui.run();
	return 0;
}