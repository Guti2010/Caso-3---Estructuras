#include <iostream>
#include <unordered_map>
#include <vector>

// Estructura para almacenar las ubicaciones de una palabra clave
struct KeywordInfo {
    std::vector<int> locations; // Posiciones de párrafos donde aparece la palabra clave
    // Puedes agregar más información según sea necesario
};

// Función para construir árboles B para cada palabra clave
std::unordered_map<std::string, KeywordInfo> buildBTreeForEachKeyword(const std::vector<std::vector<std::string>>& paragraphs) {
    std::unordered_map<std::string, KeywordInfo> keywordIndex;

    for (int paragraphIndex = 0; paragraphIndex < paragraphs.size(); ++paragraphIndex) {
        const auto& paragraph = paragraphs[paragraphIndex];

        for (const auto& word : paragraph) {
            // Actualiza la información de la palabra clave en el índice
            keywordIndex[word].locations.push_back(paragraphIndex);
            // Puedes agregar más información aquí, como la distancia entre apariciones
        }
    }

    return keywordIndex;
}