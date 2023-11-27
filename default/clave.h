
#ifndef CLAVE_H
#define CLAVE_H

#include <vector>
#include <iostream>
#include "paragraph.h"

struct Clave {
    std::string word;
    std::vector<Paragraph> paragraphs;

    Clave() {}

    Clave(const std::string& word) : word(word) {}
};


#endif 