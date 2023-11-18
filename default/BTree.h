// bTree.h
#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include "bTreeNode.h"
#include <chrono>

class BTree {
public:
    BTree() : root(nullptr) {}

    void insert(const std::string& keyword, int id, int page, const std::string& paragraph) {
        insertRecursive(root, keyword, id, page, paragraph);
    }

    std::vector<Paragraph> search(const std::string& keyword) {
        return searchRecursive(root, keyword);
    }

    void traverse() {
        std::cout << "Recorrido del árbol B:\n";
        traverseNode(root);
    }

private:
    void insertRecursive(BTreeNode*& node, const std::string& keyword, int id, int page, const std::string& paragraph) {
        if (!node) {
            node = new BTreeNode(keyword);
            node->paragraphs.emplace_back(id, page, paragraph);
            return;
        }

        if (keyword < node->keyword) {
            insertRecursive(node->left, keyword, id, page, paragraph);
        } else if (keyword > node->keyword) {
            insertRecursive(node->right, keyword, id, page, paragraph);
        } else {
            // Keyword already exists, add paragraph to the vector
            node->paragraphs.emplace_back(id, page, paragraph);
        }

        // Balancing operations if needed...
    }

    std::vector<Paragraph> searchRecursive(BTreeNode* node, const std::string& keyword) {
        if (!node) {
            return {}; // Palabra clave no encontrada
        }

        if (keyword < node->keyword) {
            return searchRecursive(node->left, keyword);
        } else if (keyword > node->keyword) {
            return searchRecursive(node->right, keyword);
        } else {
            // Palabra clave encontrada
            return node->paragraphs;
        }
    }

    void traverseNode(BTreeNode* node) {
        if (node) {
            traverseNode(node->left);
            std::cout << "Palabra clave: " << node->keyword << "\n";
            std::cout << "Párrafos:\n";
            for (const auto& paragraph : node->paragraphs) {
                std::cout << "   Página: " << paragraph.page << "\n";
                std::cout << "   Párrafo: " << paragraph.paragraph << "\n";
            }
            std::cout << "\n";
            traverseNode(node->right);
        }
    }

private:
    BTreeNode* root;
};

#endif // BTREE_H