#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include "bTreeNode.h"
#include <algorithm>
#include <fstream>

class BTree {
private:

    int maxKeys = 70; // Maximo numero de claves por nodo
    BTreeNode* root; // Puntero a la raiz del arbol

    //--------------------------- Metodos auxiliares ---------------------------//

    // Inserta una clave en un nodo que no esta lleno
    void insertNonFull(BTreeNode* node, const std::string& keyword, int id, int page, const std::string& paragraph) 
    {
        int lastKey = node->keys.size() - 1; // Indice de la ultima clave

        // Buscar si la clave ya existe
        for (int i = 0; i < node->keys.size(); i++) 
        {
            if (node->keys[i].word == keyword) 
            {
                node->keys[i].paragraphs.push_back(Paragraph(id, page, paragraph));
                return;
            }
        }

        // Si el nodo es una hoja
        if (node->isLeaf) 
        {
            Clave newKey(keyword);
            newKey.paragraphs.push_back(Paragraph(id, page, paragraph));
            node->keys.push_back(newKey);
        } 
        else
        {
            // Buscar hijo donde insertar la clave
            while (lastKey >= 0 && keyword < node->keys[lastKey].word) 
            {
                lastKey--;
            }
            lastKey++;

            // Si la clave ya existe, insertar el parrafo en la clave
            if (lastKey < node->keys.size() && keyword == node->keys[lastKey].word) 
            {
                node->keys[lastKey].paragraphs.push_back(Paragraph(id, page, paragraph));
            }
            else
            {
                // Si el hijo esta lleno, hacer split
                if (node->children[lastKey]->keys.size() == maxKeys) {
                    splitChild(node, lastKey, node->children[lastKey]);
                    if (keyword > node->keys[lastKey].word) {
                        lastKey++;
                    }
                }

                // Insertar clave en el hijo
                insertNonFull(node->children[lastKey], keyword, id, page, paragraph);
            }
        }

        // Ordenar las claves en el nodo
        std::sort(node->keys.begin(), node->keys.end(), [](const Clave& a, const Clave& b) {
            return a.word < b.word;
        });
    }

    // Splitchild de toda la vida
    void splitChild(BTreeNode* parent, int index, BTreeNode* child) 
    {
        
        BTreeNode* newNode = new BTreeNode(child->isLeaf);

        // Insertar nueva clave en el padre
        parent->keys.insert(parent->keys.begin() + index, child->keys[maxKeys / 2]);

        // Insertar nuevo hijo en el padre
        parent->children.insert(parent->children.begin() + index + 1, newNode);

        // Copiar claves y punteros al nuevo nodo
        newNode->keys.assign(child->keys.begin() + maxKeys / 2 + 1, child->keys.end());
        child->keys.resize(maxKeys / 2); // Redimensionar vector de claves

        // Si el nodo no es una hoja, copiar punteros a los hijos
        if (!child->isLeaf) 
        {
            newNode->children.assign(child->children.begin() + maxKeys / 2 + 1, child->children.end());
            child->children.resize(maxKeys / 2 + 1);
        }
    }

    // Busca una palabra en el arbol
    std::vector<Paragraph> findKeyword(BTreeNode* node, const std::string& keyword) 
    {
        // Search for the keyword in the B-Tree
        int i = 0;
        while (i < node->keys.size() && keyword > node->keys[i].word) 
        {
            i++;
        }

        if (i < node->keys.size() && keyword == node->keys[i].word) 
        {
            return node->keys[i].paragraphs;
        } 
        else if (!node->isLeaf) 
        {
            // Keyword not found, search in the appropriate child node
            return findKeyword(node->children[i], keyword);
        }

        // Keyword not found
        return std::vector<Paragraph>();
    }

    // Imprime el arbol
    void printNode(BTreeNode* node) 
    {
        if (node != nullptr) 
        {
            for (int i = 0; i < node->keys.size(); i++) {
                std::cout << node->keys[i].word << " ";
                
            }
            std::cout << std::endl;

            if (!node->isLeaf) 
            {
                for (int i = 0; i < node->children.size(); i++) {
                    printNode(node->children[i]);
                }
            }
        }
    }

    void Save(BTreeNode* node, const std::string& filePath) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filePath << std::endl;
            return;
        }

        // Serialize the tree recursively
        serializeNode(node, file);

        file.close();
    }

    void serializeNode(BTreeNode* node, std::ofstream& file) {
        if (node == nullptr) {
            return;
        }

        // Write the flag indicating if the node is a leaf
        bool isLeaf = node->isLeaf;
        file.write(reinterpret_cast<const char*>(&isLeaf), sizeof(bool));

        // Write the number of keys in the node
        int numKeys = node->keys.size();
        file.write(reinterpret_cast<const char*>(&numKeys), sizeof(int));

        // Write the keys and their paragraphs
        for (const auto& key : node->keys) {
            // Write the word
            int wordSize = key.word.size();
            file.write(reinterpret_cast<const char*>(&wordSize), sizeof(int));
            file.write(key.word.c_str(), wordSize);

            // Write the number of paragraphs
            int numParagraphs = key.paragraphs.size();
            file.write(reinterpret_cast<const char*>(&numParagraphs), sizeof(int));

            // Write each paragraph
            for (const auto& paragraph : key.paragraphs) {
                // Write the paragraph ID, page, and content
                file.write(reinterpret_cast<const char*>(&paragraph.id), sizeof(int));
                file.write(reinterpret_cast<const char*>(&paragraph.page), sizeof(int));

                int contentSize = paragraph.paragraph.size();
                file.write(reinterpret_cast<const char*>(&contentSize), sizeof(int));
                file.write(paragraph.paragraph.c_str(), contentSize);
            }
        }

        // Recursively serialize the child nodes
        for (const auto& child : node->children) {
            serializeNode(child, file);
        }
    }

    void Load(BTreeNode*& node, const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for reading: " << filePath << std::endl;
            return;
        }

        // Deserialize the tree recursively
        deserializeNode(node, file);

        file.close();
    }

    void deserializeNode(BTreeNode*& node, std::ifstream& file) {
        if (node == nullptr) {
            node = new BTreeNode();
        }

        // Read the flag indicating if the node is a leaf
        bool isLeaf;
        file.read(reinterpret_cast<char*>(&isLeaf), sizeof(bool));
        node->isLeaf = isLeaf;

        // Read the number of keys in the node
        int numKeys;
        file.read(reinterpret_cast<char*>(&numKeys), sizeof(int));

        // Read the keys and their paragraphs
        for (int i = 0; i < numKeys; i++) {
            // Read the word
            int wordSize;
            file.read(reinterpret_cast<char*>(&wordSize), sizeof(int));
            std::string word(wordSize, '\0');
            file.read(&word[0], wordSize);

            // Read the number of paragraphs
            int numParagraphs;
            file.read(reinterpret_cast<char*>(&numParagraphs), sizeof(int));

            // Find the existing key with the same word
            bool keyExists = false;
            for (auto& key : node->keys) {
                if (key.word == word) {
                    // Add the paragraphs to the existing key
                    for (int j = 0; j < numParagraphs; j++) {
                        int id, page, contentSize;
                        file.read(reinterpret_cast<char*>(&id), sizeof(int));
                        file.read(reinterpret_cast<char*>(&page), sizeof(int));
                        file.read(reinterpret_cast<char*>(&contentSize), sizeof(int));
                        std::string content(contentSize, '\0');
                        file.read(&content[0], contentSize);

                        key.paragraphs.push_back(Paragraph(id, page, content));
                    }
                    keyExists = true;
                    break;
                }
            }

            // If the key doesn't exist, create a new key and add the paragraphs
            if (!keyExists) {
                Clave newKey(word);
                for (int j = 0; j < numParagraphs; j++) {
                    int id, page, contentSize;
                    file.read(reinterpret_cast<char*>(&id), sizeof(int));
                    file.read(reinterpret_cast<char*>(&page), sizeof(int));
                    file.read(reinterpret_cast<char*>(&contentSize), sizeof(int));
                    std::string content(contentSize, '\0');
                    file.read(&content[0], contentSize);

                    newKey.paragraphs.push_back(Paragraph(id, page, content));
                }
                node->keys.push_back(newKey);
            }
        }

        // Recursively deserialize the child nodes
        if (!node->isLeaf) {
            node->children.resize(numKeys + 1); // Resize the children vector
            for (int i = 0; i < numKeys + 1; i++) {
                deserializeNode(node->children[i], file);
            }
        }
    }

    void destroyTree(BTreeNode* node) {
        if (node == nullptr) {
            return;
        }

        // Llama a la función de limpieza recursiva para cada hijo del nodo actual
        for (auto& child : node->children) {
            destroyTree(child);
            child = nullptr; // Set the pointer to nullptr after deletion
        }

        // Libera la memoria del nodo actual
        delete node;
        node = nullptr; // Set the pointer to nullptr after deletion
    }


public:

    // Constructor
    BTree() : root(nullptr) {}

    ~BTree() 
    {
        // Llama a una función de limpieza recursiva para liberar la memoria de todos los nodos
        destroyTree(root);
        cout << "Arbol destruido" << endl;
    }

    void insert(const std::string& keyword, int id, int page, const std::string& paragraph) 
    {
        // Si el arbol esta vacio, crear nodo raiz
        if (root == nullptr) {
            root = new BTreeNode(true);
            Clave newKey(keyword);
            newKey.paragraphs.push_back(Paragraph(id, page, paragraph));
            root->keys.push_back(newKey);
        } 
        else // Si no, insertar clave en el arbol
        {
            bool keywordExists = false;
            for (int i = 0; i < root->keys.size(); i++) {
                if (root->keys[i].word == keyword) {
                    root->keys[i].paragraphs.push_back(Paragraph(id, page, paragraph));
                    keywordExists = true;
                    break;
                }
            }
            std::sort(root->keys.begin(), root->keys.end(), [](const Clave& a, const Clave& b) {
                return a.word < b.word;
            });
            if (!keywordExists) {
                if (root->keys.size() == maxKeys) // Si la raiz esta llena, hacer split
                {
                    BTreeNode* newRoot = new BTreeNode(false);
                    newRoot->children.push_back(root);
                    splitChild(newRoot, 0, root);
                    root = newRoot;
                }
                
                insertNonFull(root, keyword, id, page, paragraph);
            }
        }
    }

    std::vector<Paragraph> search(const std::string& keyword) 
    {
        return findKeyword(root, keyword);
    }

    void print() 
    {
        printNode(root);
    }

    void save(const std::string& filePath) 
    {
        Save(root, filePath);
    }

    void load(const std::string& filePath) 
    {
        Load(root, filePath);
    }

};

#endif // BTREE_H