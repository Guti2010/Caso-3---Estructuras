// paragraph.h
#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <iostream>

struct Paragraph {
    int id;
    int page;
    int frequency;
    std::string paragraph;

    Paragraph(int ID, int page, const std::string& paragraph)
        : id(ID), page(page), paragraph(paragraph) {}
};

#endif // PARAGRAPH_H
