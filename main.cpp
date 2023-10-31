#include <iostream>
#include "Book.h"
#include "Search.h"

int main() {

    vector<Book> library;
    
    // Agregar libros manualmente
    Book book1;
    book1.title = "Libro 1";
    book1.chaptersBySentiment["amor"] = {"Capítulo 1", "Capítulo 3"}; // Capítulos con sentimiento "amor"
    book1.chaptersBySentiment["miedo"] = {"Capítulo 2", "Capítulo 4"}; // Capítulos con sentimiento "miedo"
    library.push_back(book1);

    Book book2;
    book2.title = "Libro 2";
    book2.chaptersBySentiment["amor"] = {"Capítulo 1", "Capítulo 5"}; // Capítulos con sentimiento "amor"
    library.push_back(book2);

    return 0;
}