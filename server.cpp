#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "httplib.h"
#include "json.hpp"
#include <sstream>
#include <algorithm>
#include "GPTapi.cpp"
#include "book.h"
#include "generateLibrary.h"

using json = nlohmann::json;
using namespace httplib;
using namespace std;

int main() {
    Server svr;
    svr.
    svr.Post("/Mostrar_libros", [](const Request& req, Response& res) {
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
      
    

        json respuesta;
        respuesta["Libros"] = top10BooksForSentiment;
        
      
        res.set_content(respuesta.dump(), "application/json");
    });

    svr.listen("0.0.0.0", 8080);

    return 0;
}