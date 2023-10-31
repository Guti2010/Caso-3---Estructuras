#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <algorithm>
#include "GPTapi.cpp"
#include "book.h"
#include "generateLibrary.h"
using namespace std;
using json = nlohmann::json;

// Main entry into the application
int main() {

    // Library of books
    vector<Book> library = generateLibrary();

    cout << "Ingresar frase: ";
    string prompt;
    getline(cin, prompt);
    Chat chat;
    
    string response = chat.getCompletion("Obtener palabras clave de la siguiente frase: "+ prompt);

    vector<string> filteredWords = chat.RemoveCommonWords(response);

    cout << "Palabras clave de la frase: ";
    for (const string& word : filteredWords) {
        cout << word << endl;
    }

    std::string sentimentToSearch = "Aventura"; // Cambie esta por el sentimiento que se obtiene de la frase
    std::vector<Book> top10BooksForSentiment = findTop10BooksForSentiment(library, sentimentToSearch);
    
    
    // Dar respuesta al usuario
    if (top10BooksForSentiment.empty()) {
        std::cout << "No se encontraron libros con capitulos para el sentimiento '" << sentimentToSearch << "'." << std::endl;
    } else {
        std::cout << "Los 10 primeros libros con capitulos para el sentimiento '" << sentimentToSearch << "' son:" << std::endl;
        for (const auto& book : top10BooksForSentiment) {
            std::cout << " - " << book.title << std::endl;
        }
    }
    
    return 0;
}