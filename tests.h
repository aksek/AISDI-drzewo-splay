#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>
#include <cstdlib>

using namespace std;

void unit_test() {
    TreeMap<int,int> dict;

    // slownik jest pusty
    assert(dict.isEmpty() == true);
    assert(dict.size() == 0);
    assert(dict.contains(0) == false);

    // dodanie elementu do slownika
    dict.insert(0, 1);
    assert(dict.isEmpty() == false);
    assert(dict.size() == 1);
    assert(dict.contains(0) == true);
    assert(dict.value(0) == 1);

    // dodanie elementu do slownika jako pary
    dict.insert(pair<int,int>(1, 2));
    assert(dict.size() == 2);
    assert(dict.contains(1) == true);
    assert(dict.value(0) == 1);
    assert(dict.value(1) == 2);

    // operator []
    assert(dict[0] == 1);
    assert(dict[1] == 2);

    // operator [] tworzy nowy element
    dict[2] = 3;
    assert(dict.value(2) == 3);
    assert(dict.size() == 3);


    // nadpisanie wartosci dla istniejacego elementu
    dict.insert(2, 4);
    assert(dict.size() == 3);
    assert(dict.value(2) == 4);

    dict.insert(pair<int,int>(5, 6));
    assert(dict.size() == 4);
    assert(dict.contains(5) == true);
    assert(dict.value(5) == 6);

    dict.insert(pair<int,int>(100, 150));
    assert(dict.size() == 5);
    assert(dict.contains(100) == true);
    assert(dict.value(100) == 150);

    dict.insert(pair<int,int>(75, 75));
    assert(dict.size() == 6);
    assert(dict.contains(75) == true);
    assert(dict.value(75) == 75);

    dict.insert(pair<int,int>(80, 80));
    assert(dict.size() == 7);
    assert(dict.contains(80) == true);
    assert(dict.value(80) == 80);

    dict.insert(pair<int,int>(80, 100));
    assert(dict.size() == 7);
    assert(dict.contains(80) == true);
    assert(dict.value(80) == 100);


    dict.insert(pair<int,int>(1000, 2000));
    assert(dict.size() == 8);
    assert(dict.contains(1) == true);
    assert(dict.value(1000) == 2000);

    dict.insert(pair<int,int>(-1, -1));
    assert(dict.size() == 9);
    assert(dict.contains(-1) == true);
    assert(dict.value(-1) == -1);

    dict.insert(pair<int,int>(10, 20));
    assert(dict.size() == 10);
    assert(dict.contains(10) == true);
    assert(dict.value(10) == 20);

    dict.insert(pair<int,int>(1234, 412));
    assert(dict.size() == 11);
    assert(dict.contains(1234) == true);
    assert(dict.value(1234) == 412);

    dict.insert(pair<int,int>(41, 123));
    assert(dict.size() == 12);
    assert(dict.contains(41) == true);
    assert(dict.value(41) == 123);
}