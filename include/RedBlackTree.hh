#include "Node.hh"
#include <iostream>

// 红黑树类
class RedBlackTree final{

private:
    Node* root;
    size_t _size;

private:
    //左旋
    void leftRotate(Node*);
    //右旋
    void rightRotate(Node*);

    void insertFixUp(Node*);

    void deleteFixUp(Node*);

    void Transplant(Node*, Node*);

    Node* Minimum(Node*);

    void inOrderHelper(Node*);

    void destroyTree(Node*);

public:
    RedBlackTree();
    ~RedBlackTree();

public:
    void insert(int);

    void deleteNode(int);

public:
    inline const size_t& size(){
        return this->_size;
    }

    inline bool empty(){
        return this->_size == 0 ? true : false;
    }

    //遍历方式默认为中序遍历
    inline void ergodicTree(){
        inOrderHelper(root);
        std::cout << std::endl;
    }

};