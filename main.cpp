#include <iostream>
#include "bookIndex.h"
#include "BMS.h"
#include "default/paragraph.h"
#include "default/BTree.h"


int main() {
    //GPT gpt;
    Chat chat;

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

    cout << "Ingresar frase: ";
    std::string prompt;
    getline(cin, prompt);

    std::string response = chat.getCompletion("Give the keywords of next phrase separated by coma, lower case and singular in its base form: "+ prompt);

    cout << "Response: " << response << endl;

    std::vector<std::string> keywords = RemoveCommonWords(response);

    std::vector<Fragment> finalFragments = search(response, keywords, books);

    for (Fragment& Fragment : finalFragments) {
        std::cout << "Book: " << Fragment.book << std::endl;
        std::cout << "Page: " << Fragment.page << std::endl;
        std::cout << "Sentiment: " << Fragment.sentiment << std::endl;
        std::cout << "Paragraph: " << Fragment.paragraph << std::endl;
    }

    /*

    std::vector<Book> top10 = findTop10Books(books, keywords);

    for (std::string keyword : keywords) {
        std::cout << keyword << std::endl;
    }

    for (Book& book : top10) {

        std::vector<Paragraph> paragraphs = commonKeywords(keywords, book, 2);

        for (Paragraph& paragraph : paragraphs) {
            std::cout << "Book: " << book.title << std::endl;
            std::cout << "Page: " << paragraph.page << std::endl;
            std::cout << "Paragraph: " << paragraph.paragraph << std::endl;
            std::cout << "Frecuency: " << paragraph.frequency << std::endl;
            std::cout << std::endl;
        }
    }

*/

 
    return 0;
}
