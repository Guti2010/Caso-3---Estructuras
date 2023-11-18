#include <iostream>
#include "bookIndex.h"
#include "BMS.h"
#include "default/paragraph.h"
#include "default/BTree.h"

int main() {


    std::vector<Book> books = findBooks();
    findPages(books);
    buildHashtable(books);

    std::vector<std::string> keywords = {"ordinary", "people", "extraordinary", "things"};

    cout << "Top 10 books: " << endl;

    std::vector<Book> top10 = findTop10Books(books, keywords);

    for (const auto& book : top10) {
        cout << book.title << endl;
    }
 
    return 0;
}
