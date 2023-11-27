#ifndef BMS
#define BMS

#include <iostream>
#include <fstream>
#include "default/book.h"
#include <filesystem>

void saveBooks(std::vector<Book>& library)
{
    for(const Book& book : library) 
    {
        std::ofstream outputFile("data/books/" + book.title + ".bin", std::ios::binary);
        if (outputFile.is_open()) {
            serializeBook(book, outputFile);
            outputFile.close();
            std::cout << "Libro guardado exitosamente.\n";
        } else {
            std::cerr << "No se pudo abrir el archivo para guardar el libro.\n";
        }
    }
}

std::vector<Book> loadBooks() {
    std::vector<Book> loadedBooks;

    const std::string dataFolder = "data/books/";

    // Itera sobre los archivos en el directorio "data"
    for (const auto& entry : std::filesystem::directory_iterator(dataFolder)) {
        if (entry.is_regular_file()) {
            std::ifstream inputFile(entry.path(), std::ios::binary);
            if (inputFile.is_open()) {
                Book loadedBook;
                deserializeBook(loadedBook, inputFile);
                inputFile.close();
                loadedBooks.push_back(loadedBook);
                std::cout << "Libro cargado exitosamente: " << loadedBook.title << "\n";
            } else {
                std::cerr << "No se pudo abrir el archivo para cargar el libro: " << entry.path().filename() << "\n";
            }
        }
    }

    return loadedBooks;
}

#endif 
