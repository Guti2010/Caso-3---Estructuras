#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <fstream>
#include "default/book.h"

// Esto se hace hasta despues


void indexBook(Book& book) {
    ifstream file(book.filename);
    if (file.is_open()) {
        cout << "Indexando libro: " << book.title << endl;
        string chapter, line1, line2, sentiment;

        //string::npos

        int i = 0;
        while (getline(file, line1)) {
            
        }
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << book.filename << endl;
    }
}

/*
void searchSentiment(const vector<Book>& library, const string& sentiment) {
    for (const Book& book : library) {
        if (book.chapters.find(sentiment) != book.chapters.end()) {
            cout << "El sentimiento '" << sentiment << "' se encuentra en el libro: " << book.title << endl;
        }
    }
}

void showChapters(const vector<Book>& library) {
    for (const Book& book : library) {
        for (const auto& chapter : book.chapters) {
            cout << chapter.first << endl;
            for (const string& line : chapter.second) {
                cout << line << endl;
            }
        }
    }
}
*/

#endif