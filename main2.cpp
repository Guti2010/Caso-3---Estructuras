#include <iostream>
#include "default/bTree.h"

int main() {
    BTree btree;

    // Insertar datos de prueba
    btree.insert("apple", 1, 10, "This is an apple.");
    btree.insert("banana", 2, 5, "I like bananas.");
    btree.insert("orange", 3, 15, "Oranges are juicy.");
    btree.insert("apple", 4, 8, "Another apple paragraph.");

    // Medir el tiempo de búsqueda
    std::vector<Paragraph> result = btree.search("apple");


    // Mostrar resultados y tiempo de búsqueda
    std::cout << "Búsqueda de 'apple':\n";
    for (const auto& paragraph : result) {
        std::cout << "   Página: " << paragraph.page << "\n";
        std::cout << "   Párrafo: " << paragraph.paragraph << "\n";
    }

    return 0;
}
