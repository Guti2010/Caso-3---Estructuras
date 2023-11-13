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
#include <cctype> // Necesario para std::isspace

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

bool isLineEmpty(const std::string& line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}

std::vector<std::string> findKeywords(const std::string& page) {
    Chat chat;

    // Utiliza un stringstream para dividir el texto en líneas
    std::stringstream ss(page);
    std::string line;
    std::vector<std::string> keywordsVector;

    while (std::getline(ss, line)) {
        // Si la línea está vacía o contiene solo espacios, ignórala
        if (isLineEmpty(line)) {
            continue;
        }

        // Acumula las líneas hasta que encuentres un espacio en blanco
        std::string paragraph;
        while (!line.empty() && !isLineEmpty(line)) {
            paragraph += line + "\n";
            std::getline(ss, line);
        }

        // Procesa el párrafo para obtener las palabras clave
        std::string keywords = chat.getCompletion("Obtener unicamente palabras clave del siguiente párrafo separadas por coma:\n" + paragraph);
        std::vector<std::string> paragraphKeywords = Tokenize(keywords);

        // Agrega las palabras clave del párrafo al vector general
        keywordsVector.insert(keywordsVector.end(), paragraphKeywords.begin(), paragraphKeywords.end());
    }

    return keywordsVector;
}

void buildHashtable(Book& book) {
    for (int i = 0; i < book.pages.size(); ++i) {
        // Supongamos que la función findKeywords retorna un vector con las palabras clave encontradas en la página
        std::vector<std::string> keywords = findKeywords(book.pages[i]);

        // Almacena el número de página en la hashtable para cada palabra clave
        for (const std::string& keyword : keywords) {
            // Si ya existe la palabra clave, no la sobrescribe
            if (book.keywordPageMap.find(keyword) == book.keywordPageMap.end()) {
                book.keywordPageMap[keyword] = i; // Las paginas van a comenzar en 0 para agilizar los algoritmos
            }
        }
    }
}

std::vector<Book> rankBooks(const std::vector<Book>& library, const std::vector<std::string>& keywords) {
    // Contenedor para el resultado del ranking
    std::vector<Book> rankedBooks = library;

    // Inicializar el contador de cada libro a cero
    for (Book& book : rankedBooks) {
        for (const std::string& keyword : keywords) {
            book.keywordCount[keyword] = 0;
        }
    }

    // Incrementar el contador para cada keyword que aparece en cada libro
    for (const Book& book : library) {
        for (const std::string& keyword : keywords) {
            if (book.keywordCount.find(keyword) != book.keywordCount.end()) {
                // Si la keyword está en el libro, incrementar el contador
                rankedBooks[&book - &library[0]].keywordCount[keyword] += book.keywordCount.at(keyword);
            }
        }
    }

    // Ordenar los libros en función de los conteos de las keywords
    std::sort(rankedBooks.begin(), rankedBooks.end(), [](const Book& a, const Book& b) {
        int countA = 0, countB = 0;
        for (const auto& pair : a.keywordCount) {
            countA += pair.second;
        }
        for (const auto& pair : b.keywordCount) {
            countB += pair.second;
        }
        return countA > countB;
    });

    // Devolver los 10 primeros libros del ranking (o menos si no hay 10 libros)
    return std::vector<Book>(rankedBooks.begin(), rankedBooks.begin() + std::min(10, static_cast<int>(rankedBooks.size())));
}

#endif