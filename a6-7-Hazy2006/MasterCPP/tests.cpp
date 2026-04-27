#include <iostream>
#include <cassert>
#include <fstream>
#include "repository.h"
#include "service.h"
#include "tests.h"

static void clearFile() { std::ofstream("test.txt", std::ofstream::trunc); }

void testAddValidations() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    try { s.add("", "Cherno", 10, 30, 100, "http://test.com"); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Title cannot be empty!") != std::string::npos); }
    try { s.add("T", "", 10, 30, 100, "http://test.com"); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Presenter cannot be empty!") != std::string::npos); }
    try { s.add("T", "P", -1, 30, 100, "http://test.com"); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Minutes cannot be negative!") != std::string::npos); }
    try { s.add("T", "P", 10, 60, 100, "http://test.com"); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Seconds must be between 0 and 59!") != std::string::npos); }
    try { s.add("T", "P", 10, 30, -1, "http://test.com"); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Likes cannot be negative!") != std::string::npos); }
    try { s.add("T", "P", 10, 30, 0, ""); }
    catch (std::exception& e) { assert(std::string(e.what()).find("Link cannot be empty!") != std::string::npos); }
    std::cout << "testAddValidations passed!\n";
}

void testAdd() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    assert(s.length() == 1);
    try { s.add("LearnCPP", "Cherno", 5, 0, 50, "http://other.com"); }
    catch (std::exception& e) { assert(std::string(e.what()) == "Tutorial already exists!"); }
    s.add("TestTutorial", "TestPresenter", 0, 0, 0, "http://valid.com");
    assert(s.length() == 2);
    std::cout << "testAdd passed!\n";
}

void testRemove() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    s.remove("LearnCPP", "Cherno");
    assert(s.length() == 0);
    try { s.remove("LearnCPP", "Cherno"); }
    catch (std::exception& e) { assert(std::string(e.what()) == "Tutorial not found!"); }
    std::cout << "testRemove passed!\n";
}

void testUpdate() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    s.update("LearnCPP", "Cherno", "LearnC++", "Chernobog", 15, 20, "http://test2.com");
    std::vector<Tutorial> all = s.get_all();
    assert(all[0].getTitle() == "LearnC++");
    assert(all[0].getLikes() == 100);
    std::cout << "testUpdate passed!\n";
}

void testGetAll() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    assert(s.get_all().size() == 0);
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test1.com");
    s.add("PointersCPP", "CodeBeauty", 15, 45, 250, "http://test2.com");
    s.add("STL_Guide", "BoQian", 20, 15, 500, "http://test3.com");
    std::vector<Tutorial> all = s.get_all();
    assert(all.size() == 3);
    assert(all[0].getTitle() == "LearnCPP");
    assert(all[1].getTitle() == "PointersCPP");
    assert(all[2].getTitle() == "STL_Guide");
    std::cout << "testGetAll passed!\n";
}

void testDomainClasses() {
    Duration d(10, 30);
    assert(d.getMinutes() == 10 && d.getSeconds() == 30);
    Tutorial t1("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    assert(t1.getTitle() == "LearnCPP" && t1.getLikes() == 100);
    t1.incrementLikes();
    assert(t1.getLikes() == 101);
    Tutorial t2("LearnCPP", "Cherno", 5, 0, 50, "http://other.com");
    assert(t1 == t2);
    std::cout << "testDomainClasses passed!\n";
}

void testGetByPresenter() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 1500, "http://1.com");
    s.add("SmartPointers", "Cherno", 18, 35, 2700, "http://2.com");
    s.add("STL", "BoQian", 22, 0, 3400, "http://3.com");
    std::vector<Tutorial> result = s.getByPresenter("Cherno");
    assert(result.size() == 2);
    std::vector<Tutorial> all = s.getByPresenter("");
    assert(all.size() == 3);
    std::cout << "testGetByPresenter passed!\n";
}

void testIncrementLikes() {
    clearFile(); Repo r("test.txt"); Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 1500, "http://1.com");
    s.incrementLikes("LearnCPP", "Cherno");
    assert(s.get_all()[0].getLikes() == 1501);
    std::cout << "testIncrementLikes passed!\n";
}

void runAllTests() {
    std::cout << "\n=== Running All Tests ===\n\n";
    testAddValidations();
    testAdd();
    testRemove();
    testUpdate();
    testGetAll();
    testDomainClasses();
    testGetByPresenter();
    testIncrementLikes();
    std::cout << "\n=== All tests passed! ===\n\n";
}
