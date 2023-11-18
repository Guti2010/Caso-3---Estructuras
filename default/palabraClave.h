
#ifndef PALABRACLAVE_H
#define PALABRACLAVE_H

#include <vector>
#include <iostream>
#include "paragraph.h"

struct palabraClave {
    std::string word;
    std::vector<Paragraph> paragraphs;

    palabraClave() = default; // Constructor predeterminado

    palabraClave(const std::string& w) : word(w) {}
};


#endif 