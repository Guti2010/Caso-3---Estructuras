#include <iostream>
#include <string>
#include <curl/curl.h>
#include "lib/json.hpp"
#include <sstream>
#include <algorithm>
#include "lib/GPTapi.cpp"
#include "lib/GPT.h"
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
    
    string keywords = chat.getCompletion("Obtener palabras clave de la siguiente frase separados por coma: "+ prompt);
    vector<string> keywordsVector = chat.Tokenize(keywords);

    // for(int i = 0; i < chat.Tokenize(keywords).size(); i++){
    //     cout << chat.Tokenize(keywords)[i] << endl;
    // }
    
    return 0;
}