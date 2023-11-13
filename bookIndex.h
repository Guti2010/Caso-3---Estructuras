#ifndef BOOKINDEX_H
#define BOOKINDEX_H

#include <iostream>
#include <fstream>
#include "default/book.h"
#include <filesystem>
#include <curl/curl.h>
#include "lib/json.hpp"
#include "lib/GPTapi.cpp"
#include "lib/GPT.h"

namespace fs = std::filesystem;

std::vector<Book> findBooks() {

    // Vector de libros
    std::vector<Book> library;

    try {
        for (const auto& entry : fs::directory_iterator("Libros/")) 
        {
            std::string filename = entry.path().filename().string();

            Book book;
            book.filename = filename;
            
            size_t lastDotPos = filename.find_last_of('.');

            if (lastDotPos != std::string::npos) 
            {
                std::string filenameWithoutExtension = filename.substr(0, lastDotPos);
                book.title = filenameWithoutExtension;
                cout << "Registrando libro: " << book.title << endl;
            } 
            else 
            {
                book.title = filename;
            }

            library.push_back(book);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error al acceder a la carpeta: " << e.what() << std::endl;
    }

    return library;
}

void findPages(std::vector<Book>& library) {
    for (Book& book : library) { // Por cada libro en la biblioteca
        std::ifstream file("Libros/" + book.filename); // Abre el archivo de texto para lectura

        if (file.is_open()) { // Si el archivo se abrió correctamente
            std::cout << "REGISTRANDO PÁGINAS DE: " << book.title << std::endl;
            std::string line;
            std::string page = ""; // Página actual
            int lineNumber = 0;  // Número de línea actual

            while (getline(file, line)) {
                // Almacena la línea en el vector de páginas del libro
                page += line + "\n";

                // Incrementa el número de línea
                lineNumber++;

                // Si se alcanza el límite de líneas por página, inicia una nueva página
                if (lineNumber >= 55) {
                    book.pages.push_back(page);
                    lineNumber = 1;
                }
            }

            file.close();
        } else {
            std::cerr << "No se pudo abrir el archivo: " << book.filename << std::endl;
        }
    }
}



#endif