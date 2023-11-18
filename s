std::vector<Paragraph> search(const std::string& response, const std::vector<std::string>& keywords, const std::vector<Book>& library) {
    
    Chat chat;
    
    std::vector<Paragraph> usefulParagraphs;

    // Obtener los 10 libros principales
    std::vector<Book> top10 = findTop10Books(library, keywords);

    for (Book& book : top10) {
        // Obtener párrafos relacionados
        std::vector<Paragraph> paragraphs = commonKeywords(keywords, book, 3);

        // Filtrar y agregar hasta 3 párrafos útiles
        for (const Paragraph& paragraph : paragraphs) {

            cout << "Parrafo: " <<paragraph.paragraph << endl;

            std::string result = chat.getCompletion("Does this phrase: "+ response + "make sense with some sentence of this paragraph: "+ paragraph.paragraph + "?" + " If it does, return in the next format: True | only what exactly make sense with the paragraph, if it doesn't, return False | none");

            cout << result << endl;

            std::istringstream iss(result);
            
            std::string array[2];

            std::getline(iss, array[0], '|');
            std::getline(iss, array[1]);

            if (array[0] == "True" && usefulParagraphs.size() < 3) {

                Paragraph finalParagraph(paragraph.id, paragraph.page, array[1]);

                // Agregar el párrafo al vector de párrafos útiles
                usefulParagraphs.push_back(finalParagraph);
            }
        }

        // Si ya tenemos 3 párrafos útiles, salir del bucle
        if (usefulParagraphs.size() >= 3) {
            break;
        }
    }

    return usefulParagraphs;
}