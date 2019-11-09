#include <iostream>
#include <chrono>
#include <utility>

using namespace std;

/*!
 *  Narzedzie do zliczania czasu
 *
 *  Sposob uzycia:
 *
 *  {
 *    Benchmark<std::chrono::nanoseconds> b;
 *    // kod do zbadania
 *    size_t elapsed = b.elapsed();
 *  }
 *
 *  lub
 *
 *  {
 *    Benchmark<std::chrono::milliseconds> b(true);
 *    // kod do zbadania
 *  } // obiekt wypisze wartosc czasu w podanych jednostkach na stderr
 */
template<typename D = std::chrono::microseconds>
class Benchmark {
public:

    Benchmark(bool printOnExit = false) : m_print(printOnExit) {
        start = std::chrono::high_resolution_clock::now();
    }
    typename D::rep elapsed() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto result = std::chrono::duration_cast<D>(end-start);
        return result.count();
    }
    ~Benchmark() {
        auto result = elapsed();
        if (m_print)
        {
            std::cerr << "Czas: " << result << "\n";
        }
    }
private:
    std::chrono::high_resolution_clock::time_point start;
    bool m_print = true;
};


template<typename KeyType, typename ValueType>
class TreeMap;

/*
 * Element przechowywany w slowniku
 *
 */

template<typename KeyType, typename ValueType>
class Node {
        KeyType key;
        ValueType value;
        Node *parent;
        Node *left;
        Node *right;   
        friend class TreeMap<KeyType, ValueType>;    
    public:
        Node(KeyType key, ValueType value, Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr) : 
                key(key), value(value), parent(parent), left(left), right(right) {};
};


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

    TreeMap() {
        root = nullptr;
        siz = 0;
        cerr << "created" << endl;
    }
    ~TreeMap() {
        eraseAll(this->root);
        cerr << "deleted" << endl;
    }

    void eraseAll(Node<key_type, mapped_type> *cur) {
        if (cur->left != nullptr)
            eraseAll(cur->left);
        if (cur->right != nullptr) 
            eraseAll(cur->right);
        delete cur;
    }

    /*!
     * true jezeli slownik jest pusty
     */
    bool isEmpty() const
    {
        cerr << "isEmpty" << endl;
        return this->root == nullptr;
    }

    /*!
     * dodaje wpis do slownika
     */
    void insert(const key_type& key, const mapped_type &value) {
        cerr << "inserting " << key << endl;
        if (root == nullptr) {
            root = new Node<key_type, mapped_type> (key, value);
        } else {
            this->splay(key);
            if (this->root->key == key) {
                this->root->value = value;
            } else {
                Node<key_type, mapped_type> *newNode = new Node<key_type, mapped_type> (key, value, nullptr, nullptr, nullptr);
                Node<key_type, mapped_type> *temp = this->root;
                root = newNode;
                if (key < this->root->key) {
                    root->left = temp->left;
                    temp->left = nullptr;
                    root->right = temp;
                } else {
                    root->right = temp->right;
                    temp->right = nullptr;
                    root->left = temp;
                }
            }
        }
        siz += 1;
        cerr << "inserted, the root is now " << root->key << endl;
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
        /* Node<key_type, mapped_type> *closest = findClosest(key);
        if (closest->key == key) */
    }

    /*!
     * zwraca wartosc dla podanego klucza
     */
    const mapped_type& value(const key_type& key) const
    {
        cerr << "value" << endl;
        Node<key_type, mapped_type> *closest = findClosest(key);
        if (closest->key == key)
            return closest->value;
        else
            throw std::runtime_error("element does not exist");
    }

    /*!
     * zwraca informacje, czy istnieje w slowniku podany klucz
     */
    bool contains(const key_type& key) const {
        cerr << "contains " << key << endl;
        Node<key_type, mapped_type> *closest = findClosest(key);
        if (closest)
            cerr << closest->key << endl;
        if (closest && closest->key == key)
            return true;
        else
            return false;
    }

    /*!
     * zwraca liczbe wpisow w slowniku
     */
    size_t size() const {
        return siz;
    }
private:
    Node<KeyType, ValueType> *root;
    size_t siz;
    void splay(key_type key) {
        cerr << "splay " << key << endl;
        Node<key_type, mapped_type>* el = this->findClosest(key);

        while (el != this->root) {
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
    }
    /*!
     * Zwraca wskaźnik na element o podanym kluczu, lub najblizszy mu
     */
    Node<key_type, mapped_type>* findClosest(key_type key) const {
        cerr << "find" << endl;
        if (isEmpty()) {
            cerr << "empty" << endl;
            return nullptr;
        } else {
            Node<key_type, mapped_type> *currentRoot = this->root;
            cerr << currentRoot->key << endl;
            while (1) {
                if (key < currentRoot->key) {
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
            cerr << "found" << endl;
            return currentRoot;
        }
    }
    void rotateLeft(Node<key_type, mapped_type>* el) {
        cerr << "rotating left" << endl;
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
        cerr << "rotated left" << endl;
    }

    void rotateRight(Node<key_type, mapped_type>* el) {
        cerr << "rotating right" << endl;
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
        cerr << "rotated right" << endl;
    }
};



#include "tests.h"

int main()
{
    unit_test();

    return 0;
}
