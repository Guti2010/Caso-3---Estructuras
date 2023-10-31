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
    std::unordered_map<std::string, std::vector<std::string>> chaptersBySentiment; // Hashtable para capítulos agrupados por sentimiento
};

// Función para buscar el libro con más capítulos para un sentimiento dado
vector<Book> findTop10BooksForSentiment(const std::vector<Book>& library, const std::string& sentiment) {
    std::vector<Book> top10Books;
    int count = 0;

    for (const auto& book : library) {
        if (book.chaptersBySentiment.find(sentiment) != book.chaptersBySentiment.end()) {
            top10Books.push_back(book);
            count++;

            if (count >= 10) {
                break; // Detener la búsqueda después de encontrar los primeros 10 libros
            }
        }
    }

    return top10Books;
}

#endif // BOOK_H