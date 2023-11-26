#include <iostream>
#include "openai.hpp"
#include "json.hpp"
#include <string>
using json = nlohmann::json;
using namespace std;

class DallE {
public:

    string getImage(string pPrompt){
        openai::start("");
        json request = {
                {"model", "dall-e-2"},
                {"prompt", pPrompt},
                {"n", 1},
                {"size", "256x256"}
            };

        auto image = openai::image().create(request);
        
        json jresponse = json::parse(image.dump(2));
        string url = image["data"][0]["url"].get<string>();
        return url; 
    }
   
};