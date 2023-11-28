#include <iostream>
#include "bookIndex.h"
#include "lib/DallEapi.cpp"

int main() {
    //GPT gpt;
    Chat chat;
    DallE imageCreator;

    std::vector<Book> books = findBooks();
    findPages(books);

    buildHashtable(books);

    for (Book& book : books) {
        processBook(book);
    }

    cout << "Ingresar frase: ";
    std::string prompt;
    getline(cin, prompt);

    std::string response = chat.getCompletion("Give the keywords of next phrase separated by coma, lower case and singular in its base form: "+ prompt);

    cout << "Response: " << response << endl;

    std::vector<std::string> keywords = RemoveCommonWords(response);

    std::vector<Fragment> finalFragments = search(response, keywords, books);

    for (Fragment& Fragment : finalFragments) {
        string url = imageCreator.getImage(Fragment.paragraph);
        std::cout << std::endl << "Book: " << Fragment.book << std::endl;
        std::cout << "Page: " << Fragment.page << std::endl;
        std::cout << "Sentiment: " << Fragment.sentiment << std::endl;
        std::cout << "Paragraph: " << Fragment.paragraph << std::endl;
        std::cout << "Image: " << url << std::endl;
    }
    
    return 0;
}
