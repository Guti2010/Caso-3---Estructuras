#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include "paragraph.h"
#include "btree.h"

using namespace std;

struct Book {
    std::string title;
    std::string author;
    std::string filename;
    std::vector<std::string> pages;
    std::unordered_map<std::string, std::unordered_set<int>> keywordPageMap; // Hashtable para cada libro
    BTree btree; // Árbol B para cada libro

    // Función para insertar una palabra clave en el libro
    void insertKeyword(const std::string& keyword, int id, int page, const std::string& paragraph) {
        // Actualizar el árbol B
        btree.insert(keyword, id, page, paragraph);
    }

    // Función para buscar páginas por palabra clave
    std::vector<Paragraph> searchPagesByKeyword(const std::string& keyword) {
        return btree.search(keyword);
    }

    // Función para contar la cantidad total de páginas asociadas a un conjunto de keywords
    int countPagesForKeywords(const std::vector<std::string>& keywords) const {
        int totalCount = 0;
        for (const auto& keyword : keywords) {
            auto it = keywordPageMap.find(keyword);
            if (it != keywordPageMap.end()) {
                totalCount += it->second.size();
            }
        }
        return totalCount;
    }

    // Función para serializar el libro excepto el BTree y guardarlo en data/books/ + title + .bin
    void serializeBook() const {
        std::string filePath = "data/books/" + title + ".bin";
        std::ofstream outputFile(filePath, std::ios::binary);
        if (outputFile.is_open()) {
            // Escribir el título del libro
            outputFile.write(title.c_str(), title.size() + 1);

            // Escribir el autor del libro
            outputFile.write(author.c_str(), author.size() + 1);

            // Escribir el nombre del archivo
            outputFile.write(filename.c_str(), filename.size() + 1);

            // Escribir las páginas del libro
            int numPages = pages.size();
            outputFile.write(reinterpret_cast<const char*>(&numPages), sizeof(int));
            for (const auto& page : pages) {
                int pageSize = page.size();
                outputFile.write(reinterpret_cast<const char*>(&pageSize), sizeof(int));
                outputFile.write(page.c_str(), pageSize);
            }

            // Escribir el mapa de palabras clave y páginas
            int numKeywords = keywordPageMap.size();
            outputFile.write(reinterpret_cast<const char*>(&numKeywords), sizeof(int));
            for (const auto& entry : keywordPageMap) {
                const std::string& keyword = entry.first;
                const std::unordered_set<int>& pages = entry.second;

                // Escribir la palabra clave
                int keywordSize = keyword.size();
                outputFile.write(reinterpret_cast<const char*>(&keywordSize), sizeof(int));
                outputFile.write(keyword.c_str(), keywordSize);

                // Escribir la cantidad de páginas asociadas a la palabra clave
                int numPages = pages.size();
                outputFile.write(reinterpret_cast<const char*>(&numPages), sizeof(int));

                // Escribir las páginas asociadas a la palabra clave
                for (const auto& page : pages) {
                    outputFile.write(reinterpret_cast<const char*>(&page), sizeof(int));
                }
            }

            outputFile.close();
        }
    }

    // Función para deserializar el libro desde un archivo binario
    void deserializeBook() {
        std::ifstream inputFile("data/books/" + title + ".bin", std::ios::binary);
        if (inputFile.is_open()) {
            // Leer el título del libro
            std::string deserializedTitle;
            std::getline(inputFile, deserializedTitle, '\0');

            // Leer el autor del libro
            std::string deserializedAuthor;
            std::getline(inputFile, deserializedAuthor, '\0');

            // Leer el nombre del archivo
            std::string deserializedFilename;
            std::getline(inputFile, deserializedFilename, '\0');

            // Leer las páginas del libro
            int numPages;
            inputFile.read(reinterpret_cast<char*>(&numPages), sizeof(int));
            std::vector<std::string> deserializedPages(numPages);
            for (int i = 0; i < numPages; i++) {
                int pageSize;
                inputFile.read(reinterpret_cast<char*>(&pageSize), sizeof(int));
                std::string page(pageSize, '\0');
                inputFile.read(&page[0], pageSize);
                deserializedPages[i] = page;
            }

            // Leer el mapa de palabras clave y páginas
            int numKeywords;
            inputFile.read(reinterpret_cast<char*>(&numKeywords), sizeof(int));
            std::unordered_map<std::string, std::unordered_set<int>> deserializedKeywordPageMap;
            for (int i = 0; i < numKeywords; i++) {
                int keywordSize;
                inputFile.read(reinterpret_cast<char*>(&keywordSize), sizeof(int));
                std::string keyword(keywordSize, '\0');
                inputFile.read(&keyword[0], keywordSize);

                int numPages;
                inputFile.read(reinterpret_cast<char*>(&numPages), sizeof(int));
                std::unordered_set<int> deserializedPages(numPages);
                for (int j = 0; j < numPages; j++) {
                    int page;
                    inputFile.read(reinterpret_cast<char*>(&page), sizeof(int));
                    deserializedPages.insert(page);
                }

                deserializedKeywordPageMap[keyword] = deserializedPages;
            }

            // Actualizar los datos del libro
            title = deserializedTitle;
            author = deserializedAuthor;
            filename = deserializedFilename;
            pages = deserializedPages;
            keywordPageMap = deserializedKeywordPageMap;

            inputFile.close();
        }
    }
};




#endif // BOOK_H

