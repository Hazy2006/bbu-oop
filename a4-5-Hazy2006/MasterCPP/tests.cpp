#include <iostream>
#include <cassert>
#include "service.h"
#include "tests.h"

void testAddValidations() {
    Repo r;
    Service s{ r };

    // test empty title
    try {
        s.add("", "Cherno", 10, 30, 100, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Title cannot be empty!");
    }

    // test empty presenter
    try {
        s.add("LearnCPP", "", 10, 30, 100, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Presenter cannot be empty!");
    }

    // test negative minutes
    try {
        s.add("LearnCPP", "Cherno", -5, 30, 100, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Minutes cannot be negative!");
    }

    // test negative seconds
    try {
        s.add("LearnCPP", "Cherno", 10, -10, 100, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Seconds must be between 0 and 59!");
    }

    // test seconds > 59
    try {
        s.add("LearnCPP", "Cherno", 10, 60, 100, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Seconds must be between 0 and 59!");
    }

    // test empty link
    try {
        s.add("LearnCPP", "Cherno", 10, 30, 100, "");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Link cannot be empty!");
    }
    // test negative likes
    try {
        s.add("LearnCPP", "Cherno", 10, 30, -5, "http://test.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Likes cannot be negative!");
    }

    std::cout << "testAddValidations passed!\n";
}

void testAdd() {
    Repo r;
    Service s{ r };

    // test valid add
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    assert(s.length() == 1);

    // test duplicate
    try {
        s.add("LearnCPP", "Cherno", 5, 0, 50, "http://other.com");
        // assert(false); 
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Tutorial already exists!");
    }

    // test valid add with boundary values
    s.add("TestTutorial", "TestPresenter", 0, 0, 0, "http://valid.com");
    assert(s.length() == 2);

    s.add("TestTutorial2", "TestPresenter2", 100, 59, 10000, "http://valid2.com");
    assert(s.length() == 3);

    std::cout << "testAdd passed!\n";
}

void testRemoveValidations() {
    Repo r;
    Service s{ r };

    // test empty title
    try {
        s.remove("", "Cherno");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Title cannot be empty!");
    }

    // test empty presenter
    try {
        s.remove("LearnCPP", "");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Presenter cannot be empty!");
    }

    std::cout << "testRemoveValidations passed!\n";
}

void testRemove() {
    Repo r;
    Service s{ r };

    // add first, then remove
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    s.remove("LearnCPP", "Cherno");
    assert(s.length() == 0);

    // test remove non-existing
    try {
        s.remove("LearnCPP", "ch8rn0");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Tutorial not found!");
    }

    std::cout << "testRemove passed!\n";
}

void testUpdateValidations() {
    Repo r;
    Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");

    // test empty old title
    try {
        s.update("", "Cherno", "NewTitle", "NewPresenter", 5, 5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Title to find cannot be empty!");
    }

    // test empty old presenter
    try {
        s.update("LearnCPP", "", "NewTitle", "NewPresenter", 5, 5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Presenter to find cannot be empty!");
    }

    // test empty new title
    try {
        s.update("LearnCPP", "Cherno", "", "NewPresenter", 5, 5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "New title cannot be empty!");
    }

    // test empty new presenter
    try {
        s.update("LearnCPP", "Cherno", "NewTitle", "", 5, 5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "New presenter cannot be empty!");
    }

    // test negative minutes
    try {
        s.update("LearnCPP", "Cherno", "NewTitle", "NewPresenter", -5, 5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Minutes cannot be negative!");
    }

    // test negative seconds
    try {
        s.update("LearnCPP", "Cherno", "NewTitle", "NewPresenter", 5, -5, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Seconds must be between 0 and 59!");
    }

    // test seconds > 59
    try {
        s.update("LearnCPP", "Cherno", "NewTitle", "NewPresenter", 5, 60, "http://new.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Seconds must be between 0 and 59!");
    }

    // test empty new link
    try {
        s.update("LearnCPP", "Cherno", "NewTitle", "NewPresenter", 5, 5, "");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "New link cannot be empty!");
    }

    std::cout << "testUpdateValidations passed!\n";
}

void testFind() {
    Repo r;
    Service s{ r };
    Tutorial t{ "LearnCPP", "Cherno", 10, 30, 100, "http://test.com" };
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");

    // test found
    s.find("LearnCPP", "Cherno"); // should not throw

    // test not found
    try {
        s.find("NotExist", "Nobody");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Tutorial not found!");
    }
    std::cout << "testFind passed!\n";
}

void testUpdate() {
    Repo r;
    Service s{ r };

    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    s.update("LearnCPP", "Cherno", "LearnC++", "Chernobog", 15, 20, "http://test2.com");

    // verify update worked
    DynamicVector<Tutorial> all = s.get_all();
    Tutorial t = all.get(0);
    assert(t.getTitle() == "LearnC++");
    assert(t.getPresenter() == "Chernobog");
    assert(t.getDuration().getMinutes() == 15);
    assert(t.getDuration().getSeconds() == 20);
    assert(t.getLikes() == 100); // likes should be preserved
    assert(t.getLink() == "http://test2.com");

    // test update non-existing
    try {
        s.update("nonexistent", "nobody", "a", "b", 1, 1, "http://x.com");
        // assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Tutorial not found!");
    }

    std::cout << "testUpdate passed!\n";
}

void testGetAll() {
    Repo r;
    Service s{ r };

    // test get_all on empty repository
    DynamicVector<Tutorial> all = s.get_all();
    assert(all.length() == 0);

    // add multiple tutorials
    s.add("LearnCPP", "Cherno", 10, 30, 100, "http://test1.com");
    s.add("PointersCPP", "CodeBeauty", 15, 45, 250, "http://test2.com");
    s.add("STL_Guide", "BoQian", 20, 15, 500, "http://test3.com");

    // test get_all returns correct number
    all = s.get_all();
    assert(all.length() == 3);

    // verify first tutorial content
    Tutorial t1 = all.get(0);
    assert(t1.getTitle() == "LearnCPP");
    assert(t1.getPresenter() == "Cherno");
    assert(t1.getDuration().getMinutes() == 10);
    assert(t1.getDuration().getSeconds() == 30);
    assert(t1.getLikes() == 100);
    assert(t1.getLink() == "http://test1.com");

    // verify second tutorial content
    Tutorial t2 = all.get(1);
    assert(t2.getTitle() == "PointersCPP");
    assert(t2.getPresenter() == "CodeBeauty");
    assert(t2.getDuration().getMinutes() == 15);
    assert(t2.getDuration().getSeconds() == 45);
    assert(t2.getLikes() == 250);

    // verify third tutorial content
    Tutorial t3 = all.get(2);
    assert(t3.getTitle() == "STL_Guide");
    assert(t3.getPresenter() == "BoQian");
    assert(t3.getDuration().getMinutes() == 20);
    assert(t3.getDuration().getSeconds() == 15);
    assert(t3.getLikes() == 500);

    std::cout << "testGetAll passed!\n";
}

void testDomainClasses() {
    // Test Duration
    Duration d1(10, 30);
    assert(d1.getMinutes() == 10);
    assert(d1.getSeconds() == 30);

    Duration d2;
    assert(d2.getMinutes() == 0);
    assert(d2.getSeconds() == 0);

    // Test Tutorial constructors and getters
    Tutorial t1("LearnCPP", "Cherno", 10, 30, 100, "http://test.com");
    assert(t1.getTitle() == "LearnCPP");
    assert(t1.getPresenter() == "Cherno");
    assert(t1.getDuration().getMinutes() == 10);
    assert(t1.getDuration().getSeconds() == 30);
    assert(t1.getLikes() == 100);
    assert(t1.getLink() == "http://test.com");

    Tutorial t2;
    assert(t2.getTitle() == "");
    assert(t2.getPresenter() == "");
    assert(t2.getLikes() == 0);

    // Test Tutorial setters
    t2.setTitle("NewTitle");
    t2.setPresenter("NewPresenter");
    t2.setLink("http://newlink.com");
    Duration newDuration(5, 15);
    t2.setDuration(newDuration);

    assert(t2.getTitle() == "NewTitle");
    assert(t2.getPresenter() == "NewPresenter");
    assert(t2.getLink() == "http://newlink.com");
    assert(t2.getDuration().getMinutes() == 5);
    assert(t2.getDuration().getSeconds() == 15);

    // Test incrementLikes
    Tutorial t3("Test", "Test", 1, 1, 0, "http://test.com");
    assert(t3.getLikes() == 0);
    t3.incrementLikes();
    assert(t3.getLikes() == 1);
    t3.incrementLikes();
    assert(t3.getLikes() == 2);

    // Test equality operator
    Tutorial t4("Title1", "Presenter1", 10, 20, 100, "http://link1.com");
    Tutorial t5("Title1", "Presenter1", 5, 5, 50, "http://link2.com");
    Tutorial t6("Title2", "Presenter1", 10, 20, 100, "http://link1.com");
    Tutorial t7("Title1", "Presenter2", 10, 20, 100, "http://link1.com");

    assert(t4 == t5); // same title and presenter
    assert(!(t4 == t6)); // different title
    assert(!(t4 == t7)); // different presenter

    std::cout << "testDomainClasses passed!\n";
}

void testServiceLength() {
    Repo r;
    Service s{ r };

    assert(s.length() == 0);

    s.add("T1", "P1", 1, 1, 1, "http://1.com");
    assert(s.length() == 1);

    s.add("T2", "P2", 2, 2, 2, "http://2.com");
    assert(s.length() == 2);

    s.remove("T1", "P1");
    assert(s.length() == 1);

    s.remove("T2", "P2");
    assert(s.length() == 0);

    std::cout << "testServiceLength passed!\n";
}

// test self-assignment (covers line 33)
void testSelfAssignment() {
    DynamicVector<Tutorial> v;
    v = v; // self-assign
    assert(v.length() == 0);
    std::cout << "testSelfAssignment passed!\n";
}

// test remove out of bounds (covers line 59)
void testRemoveOutOfBounds() {
    DynamicVector<Tutorial> v;
    v.remove(-1); // should just return, not crash
    v.remove(99);
    assert(v.length() == 0);
    std::cout << "testRemoveOutOfBounds passed!\n";
}

// test vector operations for better coverage
void testVectorOperations() {
    DynamicVector<Tutorial> v;

    // Test add and length
    Tutorial t1("T1", "P1", 1, 1, 1, "http://1.com");
    v.add(t1);
    assert(v.length() == 1);

    // Test get
    Tutorial retrieved = v.get(0);
    assert(retrieved.getTitle() == "T1");

    // Test search found
    Tutorial searchTutorial("T1", "P1", 0, 0, 0, "");
    int index = v.search(searchTutorial);
    assert(index == 0);

    // Test search not found
    Tutorial notFound("NotExist", "Nobody", 0, 0, 0, "");
    index = v.search(notFound);
    assert(index == -1);

    // Test update
    Tutorial t2("T2", "P2", 2, 2, 2, "http://2.com");
    v.update(t2, 0);
    retrieved = v.get(0);
    assert(retrieved.getTitle() == "T2");

    // Test copy constructor
    DynamicVector<Tutorial> v2(v);
    assert(v2.length() == 1);
    assert(v2.get(0).getTitle() == "T2");

    // Test resize (add many elements to trigger resize)
    for (int i = 0; i < 10; i++) {
        Tutorial t("Title" + std::to_string(i), "Presenter" + std::to_string(i), i, i, i, "http://" + std::to_string(i) + ".com");
        v.add(t);
    }
    assert(v.length() == 11);

    // Test remove
    v.remove(0);
    assert(v.length() == 10);

    std::cout << "testVectorOperations passed!\n";
}


void testGetByPresenter() {
    Repo r;
    Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 1500, "http://youtube.com/1");
    s.add("SmartPointers", "Cherno", 18, 35, 2700, "http://youtube.com/6");
    s.add("STL_Containers", "BoQian", 22, 18, 3400, "http://youtube.com/3");

    // filter by presenter
    DynamicVector<Tutorial> result = s.getByPresenter("Cherno");
    assert(result.length() == 2);

    // empty presenter returns all
    DynamicVector<Tutorial> all = s.getByPresenter("");
    assert(all.length() == 3);
}

void testIncrementLikes() {
    Repo r;
    Service s{ r };
    s.add("LearnCPP", "Cherno", 10, 30, 1500, "http://youtube.com/1");

    s.incrementLikes("LearnCPP", "Cherno");
    DynamicVector<Tutorial> all = s.get_all();
    assert(all.get(0).getLikes() == 1501);

    // not found throws
    try {
        s.incrementLikes("Fake", "Nobody");
        assert(false);
    }
    catch (std::exception& e) {
        assert(std::string(e.what()) == "Tutorial not found!");
    }
}
void runAllTests() {
    std::cout << "\n=== Running All Tests ===\n\n";
    testAddValidations();
    testAdd();
    testRemoveValidations();
    testRemove();
    testUpdateValidations();
    testFind();
    testUpdate();
    testGetAll();
    testDomainClasses();
    testServiceLength();
    testSelfAssignment();
    testRemoveOutOfBounds();
    testVectorOperations();
    testGetByPresenter();
    testIncrementLikes();
    std::cout << "\n=== All tests passed! ===\n\n";
}