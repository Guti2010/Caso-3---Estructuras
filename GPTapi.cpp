#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

// Define the WriteCallback function outside of any class.
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

class Chat {
public:
    // Construct a POST request to the chat model endpoint and process the response.
    string getCompletion(const string& prompt, const string& model = "gpt-3.5-turbo") {
        string apiKey = "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"; // add your API key, the app will not work without it
        string baseUrl = "https://api.openai.com/v1/chat/completions";
        string response;
        CURL* curl = curl_easy_init();

        if (curl) {
            json requestData;
            requestData["model"] = model;
            requestData["messages"].push_back({{"role", "user"}, {"content", prompt}});
            requestData["temperature"] = 0;

            string requestDataStr = requestData.dump();

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
            curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                cerr << "Curl request failed: " << curl_easy_strerror(res) << endl;
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        json jresponse = json::parse(response);
        return jresponse["choices"][0]["message"]["content"].get<string>();
    }

    vector<string> Tokenize(const string& input) {
        vector<string> tokens;
        stringstream ss(input);
        string token;
        while (ss >> token) {
            // Eliminar comas de las palabras
            token.erase(remove_if(token.begin(), token.end(), ::ispunct), token.end());
            tokens.push_back(token);
        }
        return tokens;
    }

    vector<string> RemoveCommonWords(const string& input) {
        vector<string> words = Tokenize(input);
        vector<string> filteredWords;

        for (const string& word : words) {
            // Aquí puedes agregar lógica para filtrar palabras comunes si lo deseas.
            filteredWords.push_back(word);
        }

        return filteredWords;
    }
};

