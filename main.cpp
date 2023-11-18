#include <iostream>
#include "bookIndex.h"
#include "BMS.h"
#include "default/paragraph.h"
#include "default/BTree.h"


int main() {
    //GPT gpt;
    //Chat chat;

    std::vector<Book> books = findBooks();
    findPages(books);

    for (Book& book : books) {
        processBook(book);
    }

    buildHashtable(books);
    /*
    std::string prompt;

    cout << "Enter the phrase: ";
    cin >> prompt;

    std::string response = chat.getCompletion("Dame unicamente las palabras claves de la siguiente frase, todas en minuscula y su forma base, separadas por coma: "+prompt);
    std::vector<std::string> keywords = Tokenize(response);

*/
    std::vector<std::string> keywords = {"very", "worthy", "person", "lover"};

    std::vector<Book> top10 = findTop10Books(books, keywords);

    for (Book& book : top10) {

        cout << "Book: " << book.title << endl;

        std::vector<Paragraph> paragraphs = commonKeywords(keywords, book, 1);

        for (Paragraph& paragraph : paragraphs) {
            std::cout << "Book: " << book.title << std::endl;
            std::cout << "Page: " << paragraph.page << std::endl;
            std::cout << "Paragraph: " << paragraph.paragraph << std::endl;
            std::cout << "Frecuency: " << paragraph.frequency << std::endl;
            std::cout << std::endl;
        }
    }


 
    return 0;
}
