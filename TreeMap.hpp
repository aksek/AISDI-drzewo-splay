#ifndef TREE_MAP_HPP
#define TREE_MAP_HPP

#include"Node.hpp"

/*
 * Slownik
 *
 */
template<typename KeyType, typename ValueType>
class TreeMap
{
  public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;

    TreeMap() = default;    // konstruktor trywialny
    ~TreeMap() = default;   // destruktor trywialny

    /*!
     * true jezeli slownik jest pusty
     */
    bool isEmpty() const
    {
        return this->root == nullptr;
    }

    /*!
     * dodaje wpis do slownika
     */
    void insert(const key_type& key, const mapped_type &value) {
        this->splay(key);
        if (this->root->key == key) {
            this->root->value = value;
        } else {
            
        }
    }

    /*!
     * dodaje wpis do slownika przez podanie pary klucz-wartosc
     */
    void insert(const value_type &key_value)
    {
        this->insert(key_value.first, key_value.second);
    }

    /*!
     * zwraca referencje na wartosc dla podanego klucza
     *
     * jezeli elementu nie ma w slowniku, dodaje go
     */
    mapped_type& operator[](const key_type& key)
    {
        throw std::runtime_error("TODO: operator[]");
    }

    /*!
     * zwraca wartosc dla podanego klucza
     */
    const mapped_type& value(const key_type& key) const
    {
        throw std::runtime_error("TODO: value");
    }

    /*!
     * zwraca informacje, czy istnieje w slowniku podany klucz
     */
    bool contains(const key_type& key) const {
        throw std::runtime_error("TODO: contains");
    }

    /*!
     * zwraca liczbe wpisow w slowniku
     */
    size_t size() const {
        throw std::runtime_error("TODO: size");
    }
private:
    Node<key_type, mapped_type> *root;
    void splay(key_type key) {
        Node<key_type, mapped_type>* el = this->find(key);
        if (el->parent == this->root) {
            if (el == el->parent->right) {
                this->rotateLeft(el);
            } else {
                this->rotateRight(el);
            }
        } else if (el == el->parent->left && el->parent->parent->right == el->parent ) {
            this->rotateRight(el);
            this->rotateLeft(el);
        } else if (el == el->parent->right && el->parent->parent->left == el->parent) {
            this->rotateLeft(el);
            this->rotateRight(el);
        } else if (el == el->parent->left && el->parent->parent->left == el->parent) { 
            this->rotateRight(el->parent);
            this->rotateRight(el);
        } else {
            this->rotateLeft(el->parent);
            this->rotateLeft(el);
        }
    }
    Node<key_type, mapped_type>* findEl(key_type key) {
        Node<key_type, mapped_type> *currentRoot = this->root;
        while (1) {
            if (key <= currentRoot->key) {
                if (currentRoot->left) {
                    currentRoot = currentRoot->left; 
                } else {
                   break;
                }
                 
            } else {
                if(currentRoot->right) {
                    currentRoot = currentRoot->right;  
                } else {
                    break;
                }
            }
        }
        return currentRoot;
    }
    void rotateLeft(Node<key_type, mapped_type>* el) {
        Node<key_type, mapped_type>* temp;
        if (el->parent == this->root) {
            temp = this->root;
            this->root = el->parent;
            this->root->left = temp;
        } else if (el->parent == el->parent->parent->left) {
            temp = el->parent->parent->left;
            el->parent->parent->left = el;
            el->parent = temp->parent;

            temp->right = el->left;
            el->left = temp;
        } else if (el->parent == el->parent->parent->right) {
            temp = el->parent->parent->right;
            el->parent->parent->right = el;
            el->parent = temp->parent;

            temp->right = el->left;
            el->left = temp;
        }
    }

    void rotateRight(Node<key_type, mapped_type>* el) {
        Node<key_type, mapped_type>* temp;
        if (el->parent == this->root) {
            temp = this->root;
            this->root = el->parent;
            this->root->right = temp;
        } else if (el->parent == el->parent->parent->right) {
            temp = el->parent->parent->right;
            el->parent->parent->right = el;
            el->parent = temp->parent;

            temp->left = el->right;
            el->right = temp;
        } else if (el->parent == el->parent->parent->left) {
            temp = el->parent->parent->left;
            el->parent->parent->left = el;
            el->parent = temp->parent;

            temp->left = el->right;
            el->right = temp;
        }
    }
};


#endif