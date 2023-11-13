#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Función para dividir una cadena en palabras clave (tokens)
std::vector<std::string> tokenize(const std::string& paragraph) {
    std::istringstream iss(paragraph);
    std::vector<std::string> tokens;

    std::string token;
    while (iss >> token) {
        // Convierte a minúsculas y elimina puntuación
        token.erase(std::remove_if(token.begin(), token.end(), ispunct), token.end());
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);

        // Puedes agregar lógica adicional, como eliminar stop words o aplicar stemming/lematización aquí

        tokens.push_back(token);
    }

    return tokens;
}

