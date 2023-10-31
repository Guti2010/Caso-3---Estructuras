#include <iostream>
#include "Book.h"
#include "Search.h"
#include "GenerateLibrary.h"

int main() {

    vector<Book> library = generateLibrary();

    std::string sentimentToSearch = "Aventura";
    std::vector<Book> top10BooksForSentiment = findTop10BooksForSentiment(library, sentimentToSearch);

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