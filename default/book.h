#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include "BTree.h"
#include "paragraph.h"

using namespace std;

struct Book {
    std::string title;
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
        std::vector<Paragraph> result = btree.search(keyword);

        // Remove duplicate paragraphs
        std::unordered_set<std::string> uniqueParagraphs;
        result.erase(std::remove_if(result.begin(), result.end(), [&](const Paragraph& p) {
            if (uniqueParagraphs.count(p.paragraph) > 0) {
                return true; // Remove duplicate paragraph
            } else {
                uniqueParagraphs.insert(p.paragraph);
                return false;
            }
        }), result.end());

        return result;
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
};

void serializeBook(const Book& book, std::ostream& os) {
    // Serializar el título
    os << book.title << '\n';

    // Serializar el nombre de archivo
    os << book.filename << '\n';

    // Serializar el número de páginas y las páginas
    size_t numPages = book.pages.size();
    os.write(reinterpret_cast<const char*>(&numPages), sizeof(numPages));
    for (const std::string& page : book.pages) {
        // Serializar el tamaño de la página y la página
        size_t pageSize = page.size();
        os.write(reinterpret_cast<const char*>(&pageSize), sizeof(pageSize));
        os.write(page.c_str(), pageSize);
    }

    // Serializar el tamaño del hashtable y el hashtable
    size_t numKeywords = book.keywordPageMap.size();
    os.write(reinterpret_cast<const char*>(&numKeywords), sizeof(numKeywords));
    for (const auto& entry : book.keywordPageMap) {
        // Serializar la keyword
        os << entry.first << '\n';

        // Serializar el valor asociado a la keyword (un conjunto de páginas)
        size_t numPages = entry.second.size();
        os.write(reinterpret_cast<const char*>(&numPages), sizeof(numPages));
        for (int page : entry.second) {
            os.write(reinterpret_cast<const char*>(&page), sizeof(page));
        }
    }
}

void deserializeBook(Book& book, std::istream& is) {
    // Deserializar el título
    std::getline(is, book.title);

    // Deserializar el nombre de archivo
    std::getline(is, book.filename);

    // Deserializar el número de páginas y las páginas
    size_t numPages;
    is.read(reinterpret_cast<char*>(&numPages), sizeof(numPages));
    book.pages.resize(numPages);
    for (size_t i = 0; i < numPages; ++i) {
        // Deserializar el tamaño de la página y la página
        size_t pageSize;
        is.read(reinterpret_cast<char*>(&pageSize), sizeof(pageSize));
        char* pageBuffer = new char[pageSize + 1];
        is.read(pageBuffer, pageSize);
        pageBuffer[pageSize] = '\0';
        book.pages[i] = pageBuffer;
        delete[] pageBuffer;
    }

    // Deserializar el tamaño del hashtable y el hashtable
    size_t numKeywords;
    is.read(reinterpret_cast<char*>(&numKeywords), sizeof(numKeywords));
    for (size_t i = 0; i < numKeywords; ++i) {
        // Deserializar la keyword
        std::string keyword;
        std::getline(is, keyword);

        // Deserializar el valor asociado a la keyword (un conjunto de páginas)
        size_t numPages;
        is.read(reinterpret_cast<char*>(&numPages), sizeof(numPages));
        for (size_t j = 0; j < numPages; ++j) {
            int page;
            is.read(reinterpret_cast<char*>(&page), sizeof(page));
            book.keywordPageMap[keyword].insert(page);
        }
    }
}


#endif // BOOK_H
