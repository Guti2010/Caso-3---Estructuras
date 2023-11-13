#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <algorithm>

vector<string> Tokenize(const string& input) {
        vector<string> tokens;
        stringstream ss(input);
        string token;
        while (ss >> token) {
            // Eliminar comas de las palabras
            token.erase(remove_if(token.begin(), token.end(), ::ispunct), token.end());
            tokens.push_back(token);
        }
        return tokens;
    }

vector<string> RemoveCommonWords(const string& input) {
    vector<string> words = Tokenize(input);
    vector<string> filteredWords;
    for (const string& word : words) {
        // Aquí puedes agregar lógica para filtrar palabras comunes si lo deseas.
        filteredWords.push_back(word);
    }
    return filteredWords;
}


