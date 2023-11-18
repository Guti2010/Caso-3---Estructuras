// paragraph.h
#ifndef FRAGMENT_H
#define FRAGMENT_H

#include <iostream>

struct Fragment {
    std::string book;
    int page;
    std::string sentiment;
    std::string paragraph;

    Fragment(std::string book, int page, const std::string& sentiment, const std::string& paragraph)
        : book(book), page(page), sentiment(sentiment), paragraph(paragraph) {}
};

#endif // FRAGMENT_H
