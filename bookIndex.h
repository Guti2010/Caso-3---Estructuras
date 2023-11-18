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
#include <cctype>
#include <unordered_set>
#include "BMS.h"

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
                std::cout << "Registrando libro: " << book.title << std::endl;
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

// Función para dividir una cadena en palabras clave
std::vector<std::string> tokenizeParagraph(const std::string& paragraph) {
    // Lista de stopwords, incluyendo pronombres, preposiciones, conjunciones, y palabras comunes
    std::unordered_set<std::string> stopwords = {
        "a", "an", "the", "and", "or", "but", "so", "i", "you", "he", "she", "it", "we", "they", "is", "are", "am", "have", "has", "do", "does", "in", "on", "at", "by", "with", "for", "of", "to", "from", "as", "not", "if", "this", "that", "these", "those", "be", "been", "was", "were", "being", "will", "would", "can", "could", "should", "must", "my", "your", "his", "her", "its", "our", "their"
    };

    // Utiliza un stringstream para dividir el párrafo en palabras
    std::stringstream ss(paragraph);
    std::string word;
    std::vector<std::string> keywords;

    while (ss >> word) {
        // Convierte la palabra a minúsculas
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        // Elimina signos de puntuación alrededor de la palabra
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());

        // Elimina comillas alrededor de la palabra
        if (!word.empty() && (word.front() == '\"' || word.front() == '“')) {
            word.erase(word.begin());
        }
        if (!word.empty() && (word.back() == '\"' || word.back() == '”')) {
            word.pop_back();
        }

        // Verifica si la palabra no es una stopword
        if (stopwords.find(word) == stopwords.end() && !word.empty()) {
            keywords.push_back(word);
        }
    }

    return keywords;
}

std::vector<std::string> findKeywords(const std::string& page) {
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
        std::vector<std::string> paragraphKeywords = tokenizeParagraph(paragraph);

        // Agrega las palabras clave del párrafo al vector general
        keywordsVector.insert(keywordsVector.end(), paragraphKeywords.begin(), paragraphKeywords.end());
    }

    return keywordsVector;
}

void buildHashtable(std::vector<Book>& library) {
    for (Book& book : library) {
        std::string filePath = "datas/" + book.title + ".bin";

        // Verifica si el archivo ya existe
        if (std::filesystem::exists(filePath)) {
            std::cout << "SE OMITE EL PROCESO PARA: " << book.title << std::endl;
        } else {
            std::cout << "CONSTRUYENDO HASHTABLE PARA: " << book.title << std::endl;

            for (int i = 0; i < book.pages.size(); ++i) {
                // Supongamos que la función findKeywords retorna un vector con las palabras clave encontradas en la página
                std::vector<std::string> keywords = findKeywords(book.pages[i]);

                // Almacena el número de página en la hashtable para cada palabra clave
                for (const std::string& keyword : keywords) {
                    // Verifica si la palabra clave ya existe en la hashtable
                    auto keywordIt = book.keywordPageMap.find(keyword);

                    if (keywordIt == book.keywordPageMap.end()) {
                        // Si la palabra clave no existe, agrega la página
                        book.keywordPageMap[keyword].insert(i);
                    } else {
                        // Si la palabra clave ya existe, verifica si la página ya está registrada
                        auto& pages = keywordIt->second;
                        if (pages.find(i) == pages.end()) {
                            // Si la página no está registrada, agrégala
                            pages.insert(i);
                        }
                    }
                }
            }
        }
    }
    // Guarda el libro en un archivo binario después de construir la hashtable
    saveBooks(library);
}

// Función para encontrar el top 10 de libros con más páginas asociadas a las keywords deseadas
std::vector<Book> findTop10Books(const std::vector<Book>& library, const std::vector<std::string>& keywords) {
    // Crear un vector de pares (libro, cantidad de páginas)
    std::vector<std::pair<Book, int>> bookPageCounts;

    // Calcular la cantidad de páginas para cada libro
    for (const auto& book : library) {
        int pageCount = book.countPagesForKeywords(keywords);
        bookPageCounts.emplace_back(book, pageCount);
    }

    // Ordenar el vector en orden descendente por la cantidad de páginas
    std::sort(bookPageCounts.begin(), bookPageCounts.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Tomar los primeros 10 libros
    std::vector<Book> top10Books;
    for (size_t i = 0; i < std::min(bookPageCounts.size(), static_cast<size_t>(10)); ++i) {
        top10Books.push_back(bookPageCounts[i].first);
    }

    return top10Books;
}

#endif
