#include <iostream>
#include "bookIndex.h"

int main() {

    Book book;

    std::vector<Book> books = findBooks();  // Almacena el resultado de findBooks en una variable
    findPages(books);  // Pasa la variable a findPages

    std::vector<std::string> keywords = findKeywords(books[0].pages[0]);  // Pasa la variable a findKeywords

    // Imprimer las palabras clave del primer libro

    return 0;
    
}