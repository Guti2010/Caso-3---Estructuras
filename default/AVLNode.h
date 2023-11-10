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