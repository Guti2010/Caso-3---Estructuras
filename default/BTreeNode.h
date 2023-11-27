// bTreeNode.h
#ifndef BTREENODE_H
#define BTREENODE_H

#include <vector>
#include <iostream>
#include "paragraph.h"
#include "clave.h"

using namespace std;

struct BTreeNode {

    bool isLeaf; // Indica si el nodo es una hoja
    std::vector<Clave> keys; // Claves almacenadas en el nodo
    std::vector<BTreeNode*> children; // Punteros a los hijos

    // Constructor por defecto
    BTreeNode() : isLeaf(true) {}

    // Constructor con parámetro para indicar si el nodo es una hoja
    BTreeNode(bool is_leaf) : isLeaf(is_leaf) {}

    // Destructor para liberar memoria
    ~BTreeNode() {
        for (auto child : children) {
            delete child;
        }
    }

};

#endif // BTREENODE_H
