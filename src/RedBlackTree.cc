#include "../include/RedBlackTree.hh"

// 构造函数
RedBlackTree::RedBlackTree() 
    : root(nullptr), _size(0) {}

// 析构函数
RedBlackTree::~RedBlackTree() {
    destroyTree(root);
    this->_size = 0;
}

// 左旋 （图解路径：/img/旋转.png）
//左旋是指将一个节点向左移动，使其右子节点成为新的根节点，原根节点成为新根节点的左子节点。
void 
RedBlackTree::leftRotate(Node* x) {
    Node* fa = x->parent;
    Node* y = x->right;
    x->right = y->left;

    if (y->left != nullptr) {
        y->left->parent = x;
    }
    if (fa == nullptr) {
        root = y;
    }
    if (fa && x == fa->left) {
        fa->left = y;
        y->parent = fa;
    } else if(fa && x == fa->right) {
        fa->right = y;
    }

    y->left = x;
    x->parent = y;
}

// 右旋（图解路径：/img/旋转.png）
//右旋是指将一个节点向右移动，使其左子节点成为新的根节点，原根节点成为新根节点的右子节点
void 
RedBlackTree::rightRotate(Node* y) {
    Node* fa = y->parent;
    Node* x = y->left;
    y->left = x->right;

    if (x->right != nullptr) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (fa == nullptr) {
        root = x;
    }
    if (fa && y == fa->right) {
        fa->right = x;
    } 
    else if(fa && y == fa->left){
        fa->left = x;
    }

    x->right = y;
    y->parent = x;
}

// 插入节点并调整
void 
RedBlackTree::insert(int data) {
    Node* z = new Node(data);
    Node* y = nullptr;
    Node* x = root;

    //查找合适的插入位置
    //这里我默认数据小的放左边，大于等于的放右边
    while (x != nullptr) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == nullptr) {
        root = z;
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = nullptr;
    z->right = nullptr;
    z->color = Color::RED;
    this->_size++;

    insertFixUp(z);
}

// 插入调整(调整颜色以维护红黑树的性质)
void 
RedBlackTree::insertFixUp(Node* z) {
    while (z->parent != nullptr && z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y != nullptr && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y != nullptr && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}


// 替换节点
void 
RedBlackTree::Transplant(Node* node1, Node* node2) {
    if (node1->parent == nullptr) {
        root = node2;
    } else if (node1 == node1->parent->left) {
        node1->parent->left = node2;
    } else {
        node1->parent->right = node2;
    }
    if (node2 != nullptr) {
        node2->parent = node1->parent;
    }
}

// 寻找最小节点
Node* 
RedBlackTree::Minimum(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// 删除节点并调整
void RedBlackTree::deleteNode(int data) {
    Node* z = root;
    Node* y = nullptr;
    Node* x = nullptr;

    // 查找要删除的节点
    while (z != nullptr && z->data != data) {
        if (z->data < data) {
            z = z->right;
        } else {
            z = z->left;
        }
    }

    // 如果没有找到节点，则返回
    if (z == nullptr) {
        // not found
        return;
    }

    y = z;
    Color yOriginalColor = y->color;

    // 节点z有一个或无子节点
    if (z->left == nullptr) {
        x = z->right;
        Transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        Transplant(z, z->left);
    } else {
        // 节点z有两个子节点
        y = Minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;
    this->_size--;

    // 如果删除的是黑色节点，需要进行修正
    if (yOriginalColor == Color::BLACK) {
        deleteFixUp(x);
    }
}

// 删除调整
void 
RedBlackTree::deleteFixUp(Node* x) {
    while (x != root && (x == nullptr || x->color == Color::BLACK)) {
        if (x != nullptr && x->parent != nullptr && x == x->parent->left) {
            Node* w = x->parent->right;
            if (w != nullptr && w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if ((w == nullptr || (w->left == nullptr || w->left->color == Color::BLACK)) &&
                (w == nullptr || (w->right == nullptr || w->right->color == Color::BLACK))) {
                if (w != nullptr) {
                    w->color = Color::RED;
                }
                x = x->parent;
            } else {
                if (w == nullptr || (w->right == nullptr || w->right->color == Color::BLACK)) {
                    if (w != nullptr && w->left != nullptr) {
                        w->left->color = Color::BLACK;
                    }
                    if (w != nullptr) {
                        w->color = Color::RED;
                    }
                    rightRotate(w);
                    w = x->parent->right;
                }
                if (w != nullptr) {
                    w->color = x->parent->color;
                }
                x->parent->color = Color::BLACK;
                if (w != nullptr && w->right != nullptr) {
                    w->right->color = Color::BLACK;
                }
                leftRotate(x->parent);
                x = root;
            }
        } else if (x != nullptr && x->parent != nullptr) {
            Node* w = x->parent->left;
            if (w != nullptr && w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if ((w == nullptr || (w->right == nullptr || w->right->color == Color::BLACK)) &&
                (w == nullptr || (w->left == nullptr || w->left->color == Color::BLACK))) {
                if (w != nullptr) {
                    w->color = Color::RED;
                }
                x = x->parent;
            } else {
                if (w == nullptr || (w->left == nullptr || w->left->color == Color::BLACK)) {
                    if (w != nullptr && w->right != nullptr) {
                        w->right->color = Color::BLACK;
                    }
                    if (w != nullptr) {
                        w->color = Color::RED;
                    }
                    leftRotate(w);
                    w = x->parent->left;
                }
                if (w != nullptr) {
                    w->color = x->parent->color;
                }
                x->parent->color = Color::BLACK;
                if (w != nullptr && w->left != nullptr) {
                    w->left->color = Color::BLACK;
                }
                rightRotate(x->parent);
                x = root;
            }
        } else {
            break;
        }
    }
    if (x != nullptr) {
        x->color = Color::BLACK;
    }
}


// 中序遍历函数
void 
RedBlackTree::inOrderHelper(Node* node) {
    if (node == nullptr) 
        return;
    inOrderHelper(node->left);
    std::cout << node->data << " ";
    inOrderHelper(node->right);
}



// 销毁树（释放空间）
void 
RedBlackTree::destroyTree(Node* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
