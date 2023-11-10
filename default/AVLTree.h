#include <iostream>
#include <algorithm>

template <typename T>
class AVLNode {
public:
    T key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(T key) : key(key), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getBalance(AVLNode<T>* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode<T>* rotateRight(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + std::max(height(y->left), height(y->right));
        x->height = 1 + std::max(height(x->left), height(x->right));

        return x;
    }

    AVLNode<T>* rotateLeft(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + std::max(height(x->left), height(x->right));
        y->height = 1 + std::max(height(y->left), height(y->right));

        return y;
    }

    AVLNode<T>* insert(AVLNode<T>* node, T key) {
       if (node == nullptr) return new AVLNode<T>(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            return node; // Duplicados no están permitidos en este ejemplo
        }

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1) {
            if (key < node->left->key) {
                return rotateRight(node);
            } else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        if (balance < -1) {
            if (key > node->right->key) {
                return rotateLeft(node);
            } else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    void inorderTraversal(AVLNode<T>* node) {
        if (node == nullptr) return;

        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }
    void orderTraversal(AVLNode<T>* node) {
    if (node == nullptr) return;

        std::cout << node->key << " ";  // Primero imprimimos el valor del nodo actual
        orderTraversal(node->left);   // Luego visitamos el nodo hijo izquierdo
        orderTraversal(node->right);  // Finalmente, visitamos el nodo hijo derecho
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T key) {
        root = insert(root, key);
    }

    void inorder() {
        inorderTraversal(root);
    }

    void order() {
        orderTraversal(root);
    }
};