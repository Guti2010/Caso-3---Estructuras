#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <algorithm>
#include "GPTapi.cpp"
using namespace std;
using json = nlohmann::json;

// Main entry into the application
int main() {
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
    

    return 0;
}