#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

#include <iostream>
#include <stdexcept>

// 红黑树（模板实现版本）

template <typename T>
class RedBlackTree {
public:
    RedBlackTree();
    ~RedBlackTree();

    void insert(const T& value);
    void remove(const T& value);
    bool find(const T& value) const;

    void inorderTraversal(void (*visit)(const T& value)) const;
    void clear();

private:
    enum Color { RED, BLACK };

    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const T& value, Color color = RED)
            : data(value), color(color), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void insertFixup(Node* node);
    void deleteFixup(Node* node);
    Node* minimum(Node* node) const;
    void transplant(Node* u, Node* v);
    void clear(Node* node);
    void inorderTraversal(Node* node, void (*visit)(const T& value)) const;
    Node* findNode(const T& value) const;
};

template <typename T>
RedBlackTree<T>::RedBlackTree() : root(nullptr) {}

template <typename T>
RedBlackTree<T>::~RedBlackTree() {
    clear();
}

template <typename T>
void RedBlackTree<T>::clear() {
    clear(root);
    root = nullptr;
}

template <typename T>
void RedBlackTree<T>::clear(Node* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename T>
void RedBlackTree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template <typename T>
void RedBlackTree<T>::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

template <typename T>
void RedBlackTree<T>::insert(const T& value) {
    Node* z = new Node(value);
    Node* y = nullptr;
    Node* x = root;

    while (x != nullptr) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == nullptr) {
        root = z;
    }
    else if (z->data < y->data) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    insertFixup(z);
}

template <typename T>
void RedBlackTree<T>::insertFixup(Node* z) {
    while (z->parent != nullptr && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(z->parent->parent);
            }
        }
        else {
            Node* y = z->parent->parent->left;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::remove(const T& value) {
    Node* z = findNode(value);
    if (z == nullptr) {
        throw std::invalid_argument("Value not found in the tree");
    }

    Node* y = z;
    Node* x = nullptr;
    Color originalColor = y->color;

    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        originalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;

    if (originalColor == BLACK && x != nullptr) {
        deleteFixup(x);
    }
}

template <typename T>
void RedBlackTree<T>::deleteFixup(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        }
        else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::minimum(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <typename T>
bool RedBlackTree<T>::find(const T& value) const {
    return findNode(value) != nullptr;
}

template <typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::findNode(const T& value) const {
    Node* current = root;
    while (current != nullptr) {
        if (value < current->data) {
            current = current->left;
        }
        else if (value > current->data) {
            current = current->right;
        }
        else {
            return current;
        }
    }
    return nullptr;
}

template <typename T>
void RedBlackTree<T>::inorderTraversal(void (*visit)(const T& value)) const {
    inorderTraversal(root, visit);
}

template <typename T>
void RedBlackTree<T>::inorderTraversal(Node* node, void (*visit)(const T& value)) const {
    if (node != nullptr) {
        inorderTraversal(node->left, visit);
        visit(node->data);
        inorderTraversal(node->right, visit);
    }
}

#endif