#include <iostream>

enum Color{
    RED,
    BLACK
};

class Node{
private:
    Color color;
    int key;
    int val;
};


class Rbtree{
public:
    void insert(Node* node);
    void delete(Node* node);
    void leftRotate();
    void rightRotate();

private:

};

void Rbtree::insert(Node* node){

}