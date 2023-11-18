#include <iostream>
#include "bookIndex.h"
#include "BMS.h"
#include "default/paragraph.h"
#include "default/BTree.h"

int main() {


    std::vector<Book> books = findBooks();
    findPages(books);

    for (Book& book : books) {
        processBook(book);
    }

    buildHashtable(books);

    std::vector<std::string> keywords = {"ordinary", "people", "extraordinary", "things"};

    cout << "Top 10 books: " << endl;

    std::vector<Book> top10 = findTop10Books(books, keywords);

    for (Book& book : top10) {
        // Buscar la palabra clave "perhap" en el arbol B del libro

        std::vector<Paragraph> paragraphs = book.searchPagesByKeyword("perhap");

        for (const auto& paragraph : paragraphs) {
            std::cout << "Book: " << book.title << std::endl;
            std::cout << "Page: " << paragraph.page << std::endl;
            std::cout << "ID: " << paragraph.id << std::endl;
            std::cout << "Paragraph: " << paragraph.paragraph << std::endl;
            std::cout << std::endl;
        }
    }
 
    return 0;
}
