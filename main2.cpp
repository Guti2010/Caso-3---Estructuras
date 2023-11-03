#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <algorithm>
#include "GPTapi.cpp"
#include "Book.h"
#include "GenerateLibrary.h"

using namespace std;
using json = nlohmann::json;

// Main entry into the application
int main() {
    cout << "Ingresar frase: ";
    string prompt;
    getline(cin, prompt);
    Chat chat;
    
    // *** Descomente esto para ya obtener en keywords las keywords
    //string keywords = chat.getCompletion("Obtener palabras clave de la siguiente frase separados por coma: "+ prompt);
    string sentiment = chat.getCompletion("Obtener 10 sinónimos comunes al sentimiento de la siguiente frase, separados por coma, en sustantivo, con la primera letra de cada palabra en mayúscula: Ejemplo: Aventura, Terror, Desilusión, Aspiración, Guerra, Suspenso."+ prompt);

    // *** Palabras clave - Aqui estan las keywords para que las imprima en pantalla

    // for(int i = 0; i < chat.Tokenize(keywords).size(); i++){
    //     cout << chat.Tokenize(keywords)[i] << endl;
    // }

    vector<Book> library = generateLibrary();

    // Sentimientos
    std::vector<Book> top10BooksForSentiment = findTop10BooksForSentiment(library, chat.Tokenize(sentiment));
    if (!top10BooksForSentiment.empty()) {

        // *** De aqui el vector top10BooksForSentiment es el que tiene los libros que se deben mostrar en pantalla
        std::cout << "Los 10 primeros libros son para la frase: " + prompt + " son: " << std::endl;
        for (int i = 0; i < top10BooksForSentiment.size(); i++) {
            std::cout << " - " << top10BooksForSentiment[i].title << "#" << i+1 << std::endl;
        }
    } 
    else 
    {
        cout << "No se encontraron libros para la frase dada" << endl;
    }
    
    return 0;
}