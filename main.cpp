#include <iostream>
#include "bookIndex.h"
#include "lib/DallEapi.cpp"
#include "lib/json.hpp"

using json = nlohmann::json;

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

    json fragmentsJson;

    for (const Fragment& fragment : finalFragments) {
        json fragmentJson;
        fragmentJson["author"] = fragment.author;
        fragmentJson["book"] = fragment.book;
        fragmentJson["page"] = fragment.page;
        fragmentJson["sentiment"] = fragment.sentiment;
        fragmentJson["paragraph"] = fragment.paragraph;
        fragmentJson["image"] = imageCreator.getImage("Create a icon that is related to the following phrase: " + fragment.paragraph);

        fragmentsJson.push_back(fragmentJson);
    }

    std::cout << fragmentsJson.dump(6) << std::endl;

    return 0;

}
