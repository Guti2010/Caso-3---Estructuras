// bTreeNode.h
#ifndef BTREENODE_H
#define BTREENODE_H

#include <iostream>
#include <vector>
#include "paragraph.h"

struct BTreeNode {
    std::string keyword;
    std::vector<Paragraph> paragraphs;
    BTreeNode* left;
    BTreeNode* right;

    BTreeNode(const std::string& key) : keyword(key), left(nullptr), right(nullptr) {}
};

#endif // BTREENODE_H