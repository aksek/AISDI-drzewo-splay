

#ifndef NODE_HPP
#define NODE_HPP


#include"TreeMap.hpp"

template<typename KeyType, typename ValueType>
class Node {
        KeyType key;
        ValueType value;
        Node *parent;
        Node *leftChild;
        Node *rightChild;   
        friend class TreeMap<KeyType, ValueType>;    
    public:
        Node(KeyType key, ValueType value, Node *parent = nullptr, Node *leftChild = nullptr, Node *rightChild = nullptr) : 
                key(key), value(value), parent(parent), leftChild(leftChild), rightChild(rightChild) {};
};

#endif