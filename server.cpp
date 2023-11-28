#include <iostream>
#include <boost/asio.hpp>
#include "bookIndex.h"
#include "lib/DallEapi.cpp"
#include "lib/json.hpp"

using namespace boost::asio;
using ip::tcp;

void handle_request(tcp::socket& socket, std::vector<Book> books) {

    Chat chat;
    DallE imageCreator;

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

    // Check if the request is a POST and the URL ends with "books"
    if (request_data.find("OPTIONS") != std::string::npos) {
        // Create the HTTP response
        std::string http_response = "HTTP/1.1 200 OK\r\n";

        // Add CORS headers
        http_response += "Access-Control-Allow-Origin: *\r\n";
        http_response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
        http_response += "Access-Control-Allow-Headers: Content-Type\r\n";

        // End the headers
        http_response += "\r\n";

        // Send the response
        boost::asio::write(socket, boost::asio::buffer(http_response), error);
    }
    else if (request_data.find("GET") != std::string::npos && request_data.find("/books/") != std::string::npos) {
        
        // Extract the "books" part from the URL
        std::string url = request_data.substr(request_data.find("/books/") + 7);
        std::string phrase = url.substr(0, url.find("/"));

        // change %20 to space
        for (int i = 0; i < phrase.size(); i++) {
            if (phrase[i] == '%') {
                phrase[i] = ' ';
                phrase.erase(i + 1, 2);
            }
        }

        // Remove last 5 characters
        phrase = phrase.substr(0, phrase.size() - 5);

        // Program starts here

        std::string answer = chat.getCompletion("Give the keywords of next phrase separated by coma, lower case and singular in its base form: "+ phrase);

        std::vector<std::string> keywords = RemoveCommonWords(answer);

        std::vector<Fragment> finalFragments = search(answer, keywords, books);


        // Create a JSON object with the book name and paragraph
        nlohmann::json response_json;

        for (const Fragment& fragment : finalFragments) {
            json fragmentJson;

            fragmentJson["author"] = fragment.author;
            fragmentJson["book"] = fragment.book;
            fragmentJson["page"] = fragment.page;
            fragmentJson["sentiment"] = fragment.sentiment;
            fragmentJson["paragraph"] = fragment.paragraph;
            cout << "Generando imagen" << endl;
            fragmentJson["image"] = imageCreator.getImage("Create a image representing with people or objetcs what the following phrase says: " + fragment.paragraph);

            response_json.push_back(fragmentJson);
        }

        // Convert the JSON object to a string
        std::string response = response_json.dump(6);

        // Create the HTTP response
        std::string http_response = "HTTP/1.1 200 OK\r\n";

        // Add CORS headers
        http_response += "Access-Control-Allow-Origin: *\r\n";
        http_response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
        http_response += "Access-Control-Allow-Headers: Content-Type\r\n";

        // Add other headers and the response body
        http_response += "Content-Type: application/json\r\n";
        http_response += "Content-Length: " + std::to_string(response.length()) + "\r\n";
        http_response += "\r\n";
        http_response += response;

        // Send the response
        boost::asio::write(socket, boost::asio::buffer(http_response), error);
        
        if (error) {
            std::cerr << "Error sending response: " << error.message() << std::endl;
            return;
        }
        else {
            std::cout << "Sent response:\n" << http_response << std::endl;
        }
    } 
    else {
        // Send a 404 response if the method and path don't match any known handlers
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(response), error);
        if (error) {
            std::cerr << "Error sending response: " << error.message() << std::endl;
            return;
        }
    }
}

int main() {

    // First load

    std::vector<Book> books = findBooks();
    findPages(books);

    buildHashtable(books);

    for (Book& book : books) {
        processBook(book);
    }

    cout << "FULL LOADED" << endl;

    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        // Handle the request
        handle_request(socket, books);
    }

    return 0;
}


