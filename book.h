#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

struct Book {
    string title;
    string filename;
    std::unordered_map<std::string, std::vector<std::string>> chaptersBySentiment; // Hashtable para capítulos agrupados por sentimiento
};

#endif // BOOK_H