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

 
    return 0;
}
