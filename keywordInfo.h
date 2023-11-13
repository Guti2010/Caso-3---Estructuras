#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Estructura para almacenar información de distancia y orden
struct DistanceOrderInfo {
    int distance; // Distancia entre palabras clave
    int order;    // Orden entre palabras clave
    // Puedes agregar más información según sea necesario
};

// Función para construir un árbol B para distancias y órdenes
std::unordered_map<std::string, DistanceOrderInfo> buildBTreeForDistanceAndOrder(const std::vector<std::vector<std::string>>& paragraphs) {
    std::unordered_map<std::string, DistanceOrderInfo> distanceOrderIndex;

    for (const auto& paragraph : paragraphs) {
        for (int i = 0; i < paragraph.size(); ++i) {
            for (int j = i + 1; j < paragraph.size(); ++j) {
                // Construir clave para la combinación de palabras clave
                std::string key = paragraph[i] + "_" + paragraph[j];

                // Calcular distancia y orden entre palabras clave
                int distance = j - i;
                int order = (paragraph[i] < paragraph[j]) ? 1 : -1; // Puedes ajustar la lógica según tus necesidades

                // Actualizar la información en el índice
                distanceOrderIndex[key] = {distance, order};
            }
        }
    }

    return distanceOrderIndex;
}

// Función para buscar párrafos relacionados y generar un ranking
std::vector<std::pair<int, double>> searchAndRankParagraphs(
    const std::vector<std::vector<std::string>>& paragraphs,
    const std::vector<std::string>& query,
    const std::unordered_map<std::string, DistanceOrderInfo>& distanceOrderIndex,
    const std::unordered_map<std::string, std::vector<int>>& keywordIndex
) {
    std::vector<std::pair<int, double>> ranking; // (Índice de párrafo, Puntuación)

    for (int i = 0; i < paragraphs.size(); ++i) {
        double score = 0.0;

        // Buscar cada palabra clave en el índice de palabras clave
        for (const auto& word : query) {
            auto keywordIt = keywordIndex.find(word);
            if (keywordIt != keywordIndex.end()) {
                // Incrementar la puntuación por la presencia de la palabra clave
                score += 1.0;

                // Puedes agregar lógica adicional para considerar la distancia y el orden aquí
            }
        }

        // Puedes ajustar la fórmula de puntuación según tus necesidades específicas

        ranking.push_back({i, score});
    }

    // Ordenar el ranking en orden descendente según la puntuación
    std::sort(ranking.begin(), ranking.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    return ranking;
}