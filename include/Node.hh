// 红黑树节点颜色枚举
enum class Color { RED, BLACK };

// 红黑树节点结构体
class Node final{
private:
    int data;
    Color color;
    Node *parent;
    Node *left;
    Node *right;

public:
    friend class RedBlackTree; 

    Node(int data)
        : data(data), color(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {}
    
    ~Node(){}
};