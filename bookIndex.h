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
#include <string>
#include <sstream>
#include "default/fragment.h"

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

std::string lemmatize(const std::string& word) {
    // Implementación simple: solo trata con plurales en inglés
    if (word.size() > 1 && word.back() == 's') {
        // Elimina la 's' al final para convertir a singular
        return word.substr(0, word.size() - 1);
    }
    return word;
}

// Función para dividir una cadena en palabras clave
std::vector<std::string> tokenizeParagraph(const std::string& paragraph) {
    // Lista de stopwords, incluyendo pronombres, preposiciones, conjunciones, y palabras comunes
    std::unordered_set<std::string> stopwords = {
        "a", "an", "the", "and", "or", "but", "so", "i", "you", "he", "she", "it", "we", "they", "is", "are", "am", "have", "has", "do", "does", "in", "on", "at", "by", "with", "for", "of", "to", "from", "as", "not", "if", "this", "that", "these", "those", "be", "been", "was", "were", "being", "will", "would", "can", "could", "should", "must", "my", "your", "his", "her", "its", "our", "their"
    };

    // Utiliza un stringstream para dividir el párrafo en palabras
    std::vector<std::string> keywords;

    std::stringstream ss(paragraph);
    std::string word;

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

        // Lematiza la palabra
        std::string lemmatizedWord = lemmatize(word);

        // Verifica si la palabra no es una stopword
        if (!lemmatizedWord.empty()) {
            keywords.push_back(lemmatizedWord);
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

// Función para procesar un libro y actualizar el árbol B con palabras clave lematizadas
void processBook(Book& book) {
    std::string filePath = "data/BTrees/" + book.title + ".bin";
    
    // Comprobar si el archivo ya existe
    std::ifstream file(filePath);
    if (file.good()) {
        // El archivo ya existe, no hacer nada
        return;
    }
    
    cout << "Procesando libro: " << book.title << endl;

    int id = 0;
    for (int i = 0; i < book.pages.size(); i++) {
        std::stringstream ss(book.pages[i]);
        std::string line;

        while (std::getline(ss, line)) {
            // Si la línea está vacía o contiene solo espacios, ignórala
            if (isLineEmpty(line)) {
                continue;
            }

            // Acumula las líneas hasta que encuentres un espacio en blanco
            std::string paragraph;
            paragraph = line + "\n";
            while (!line.empty() && !isLineEmpty(line)) {
                paragraph += line + "\n";
                std::getline(ss, line);
            }

            std::vector<std::string> keywords = tokenizeParagraph(paragraph);

            id++;

            for (const std::string& keyword : keywords) {
                book.insertKeyword(keyword, id, i + 1, paragraph);
            }
        }
    }

    // Guardar el arbol en un archivo binario
    book.btree.save(filePath);

    // Destruir el árbol para liberar memoria
    book.btree.~BTree();
}

std::vector<Paragraph> commonKeywords(const std::vector<std::string>& keywords, Book& book, int minFrequency) {
    
    book.btree.load("data/BTrees/" + book.title + ".bin");

    // Vector de párrafos comunes
    std::vector<Paragraph> commonParagraphs; // Vector de párrafos comunes

    for(std::string keyword : keywords) {
        std::vector<Paragraph> paragraphs = book.searchPagesByKeyword(keyword); // Vector de párrafos de la primera keyword
        
        for(Paragraph& paragraph : paragraphs) {
            // Cut the first line of the paragraph
            size_t pos = paragraph.paragraph.find('\n');
            if (pos != std::string::npos) {
                paragraph.paragraph = paragraph.paragraph.substr(pos + 1);
            }
            
            commonParagraphs.push_back(paragraph);
        }
    }

    // Mapa para rastrear la frecuencia de cada página e ID
    std::unordered_map<int, std::unordered_map<int, int>> pageIdFrequency;

    // Iterar sobre los párrafos en el vector común
    for (Paragraph& paragraph : commonParagraphs) {
        int page = paragraph.page;
        int id = paragraph.id;

        // Verificar si la página está en el mapa
        auto pageIt = pageIdFrequency.find(page);
        if (pageIt != pageIdFrequency.end()) {
            // Verificar si el ID está en el mapa de la página
            auto idIt = pageIt->second.find(id);
            if (idIt != pageIt->second.end()) {
                // Incrementar la frecuencia del párrafo y eliminar los demás con el mismo ID y página
                paragraph.frequency += idIt->second;
                idIt->second = 0; // Establecer la frecuencia a cero en lugar de eliminar para mantener el tamaño del mapa
            } else {
                // Si el ID no está en el mapa, agregarlo con frecuencia 1
                pageIt->second[id] = 1;
            }
        } else {
            // Si la página no está en el mapa, agregarla con el ID y frecuencia 1
            pageIdFrequency[page][id] = 1;
        }
    }

    // Ordenar el vector por ID y página antes de eliminar duplicados
    std::sort(commonParagraphs.begin(), commonParagraphs.end(),
        [](const Paragraph& a, const Paragraph& b) {
            return std::tie(a.id, a.page) < std::tie(b.id, b.page);
        }
    );

    // Eliminar duplicados consecutivos
    commonParagraphs.erase(
        std::unique(commonParagraphs.begin(), commonParagraphs.end(),
            [](const Paragraph& a, const Paragraph& b) {
                return std::tie(a.id, a.page) == std::tie(b.id, b.page);
            }),
        commonParagraphs.end()
    );

    // Eliminar los párrafos con frecuencia 0
    commonParagraphs.erase(
        std::remove_if(commonParagraphs.begin(), commonParagraphs.end(),
            [](const Paragraph& paragraph) {
                return paragraph.frequency <= 0;
            }),
        commonParagraphs.end()
    );

    // Eliminar los párrafos con frecuencia mayor a 50
    commonParagraphs.erase(
        std::remove_if(commonParagraphs.begin(), commonParagraphs.end(),
            [](const Paragraph& p) {
                return p.frequency > 50;
            }),
        commonParagraphs.end()
    );

    // Ordenar el vector por frecuencia de mayor a menor
    std::sort(commonParagraphs.begin(), commonParagraphs.end(),
        [](const Paragraph& a, const Paragraph& b) {
            return a.frequency > b.frequency;
        }
    );

    return commonParagraphs;
}

std::vector<Fragment> search(const std::string& response, const std::vector<std::string>& keywords, const std::vector<Book>& library) {
    
    Chat chat;
    
    std::vector<Fragment> usefulFragments;

    // Obtener los 10 libros principales
    std::vector<Book> top10 = findTop10Books(library, keywords);

    for (Book& book : top10) {
        // Obtener párrafos relacionados
        std::vector<Paragraph> paragraphs = commonKeywords(keywords, book, 3);

        // Filtrar y agregar hasta 3 párrafos útiles
        for (const Paragraph& paragraph : paragraphs) {

            std::string result = chat.getCompletion("Assess the coherence and relevance of the following phrase: " + response + "with some sentence of this paragraph: " + paragraph.paragraph + " If it does, return exactly the same as the next format without spaces between '|': True|only the sentence with what it makes sense, if it doesn't, return False|none|only the sentiment of the sentence in one word");

            std::istringstream iss(result);

            std::string array[3];

            std::getline(iss, array[0], '|');
            std::getline(iss, array[1], '|');
            std::getline(iss, array[2]);

            if (array[0] == "True" && usefulFragments.size() < 3) {

                Fragment finalParagraph(book.title, paragraph.page, array[2] , array[1]);

                // Agregar el párrafo al vector de párrafos útiles
                usefulFragments.push_back(finalParagraph);
            }
        }

        // Si ya tenemos 3 párrafos útiles, salir del bucle
        if (usefulFragments.size() >= 3) {
            break;
        }
    }

    return usefulFragments;
}

#endif
