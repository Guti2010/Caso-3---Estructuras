#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
struct Book {
    std::string title;
    std::string filename;
    std::vector<std::string> pages;
};

#endif // BOOK_H