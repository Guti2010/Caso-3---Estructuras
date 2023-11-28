#include <iostream>
#include <boost/asio.hpp>
#include "bookIndex.h"

using namespace boost::asio;
using ip::tcp;

void handle_request(tcp::socket& socket) {
    boost::asio::streambuf buffer;
    boost::system::error_code error;

    // Read the request from the client
    boost::asio::read_until(socket, buffer, "\r\n\r\n", error);
    if (error) {
        std::cerr << "Error reading request: " << error.message() << std::endl;
        return;
    }

    // Process the request
    std::string request_data((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
    std::cout << "Received request:\n" << request_data << std::endl;

    // Your code starts here
    Chat chat;

    std::vector<Book> books = findBooks();
    findPages(books);

    buildHashtable(books);

    for (Book& book : books) {
        processBook(book);
    }

    std::string response = chat.getCompletion("Give the keywords of next phrase separated by coma, lower case and singular in its base form: "+ request_data);

    std::vector<std::string> keywords = RemoveCommonWords(response);

    std::vector<Fragment> finalFragments = search(response, keywords, books);

    // Build the response string
    std::string response_string;
    for (Fragment& Fragment : finalFragments) {
        response_string += "\nBook: " + Fragment.book;
        response_string += "\nPage: " + std::to_string(Fragment.page);
        response_string += "\nSentiment: " + Fragment.sentiment;
        response_string += "\nParagraph: " + Fragment.paragraph;
    }

    // Send the response back to the client
    std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response_string.size()) + "\r\n\r\n" + response_string;
    boost::asio::write(socket, boost::asio::buffer(http_response), error);
    if (error) {
        std::cerr << "Error sending response: " << error.message() << std::endl;
        return;
    }
}

int main() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        // Handle the request
        handle_request(socket);
    }

    return 0;
}
