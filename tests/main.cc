/******************************
*  Github:yichenyiyan  QAQ    *
*  							  *
*  							  *
*  							  *
*******************************/


#include "../include/RedBlackTree.hh"
#include <iostream>
#include <vector>


/*红黑树实现：数据测试
插入序列：{10, 5, 3, 7, 15, 12, 18}

中序遍历顺序：3, 5, 7, 10, 12, 15, 18
*/
void
testFun1(){
    std::vector<int> test = {10, 5, 3, 7, 15, 12, 18};
    RedBlackTree* rbtree = new RedBlackTree();
    if(rbtree->empty())
        std::cout<<"Yeah! it is empty!"<<std::endl;
    for(int i = 0; i < test.size(); ++i){
        rbtree->insert(test[i]);
    }
    if(!rbtree->empty())
        std::cout<<"No! it is no empty! and its size is "<< rbtree->size() <<std::endl;

    rbtree->ergodicTree();
    rbtree->deleteNode(5);
    std::cout<<"after del one Node its size: "<<rbtree->size()<<std::endl;
    rbtree->deleteNode(18);
    std::cout<<"after del second Node its size: "<<rbtree->size()<<std::endl;
    rbtree->ergodicTree();

    delete rbtree;
}


int 
main(int argc, char** argv){

    testFun1();

    return 0;
}



