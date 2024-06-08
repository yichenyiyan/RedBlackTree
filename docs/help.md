一种常用数据结构——红黑树的实现
实现语言：C++
面向对象编程思想

类的属性及其方法


Node:
属性：
    data:储存的数据（由于未使用模板编程，所以只能储存int类型的数据，可以扩展一下，如改为储存结构体类型、或者指针等）
    left:指向左孩子节点
    right:指向右孩子节点
    parent:指向父节点


RedBlackTree：
属性：
    root:根节点
    size:树的结点个数

主要方法：
    void insert(int); //插入一个数据节点
    void deleteNode(int);//删除一个数据节点
    const size_t& size();//获取当前树的节点数
    bool empty()；//判断树当前是否为空
    void ergodicTree();//遍历树（中序遍历实现）

