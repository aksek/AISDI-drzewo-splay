#include <iostream>
#include <chrono>
#include <utility>
#include <vector>
#include <map>

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
    }
    ~TreeMap() {
        eraseAll(this->root);
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
        return this->root == nullptr;
    }

    /*!
     * dodaje wpis do slownika
     */
    void insert(const key_type& key, const mapped_type &value) {
        if (isEmpty()) {
            root = new Node<key_type, mapped_type> (key, value);
            siz += 1;
        } else {
            this->splay(key);
            if (this->root->key == key) {
                this->root->value = value;
            } else {
                Node<key_type, mapped_type> *newNode = new Node<key_type, mapped_type> (key, value);
                Node<key_type, mapped_type> *temp = this->root;
                root = newNode;
                if (key < this->root->key) {
                    root->left = temp->left;
                    if (root->left)
                        root->left->parent = root;
                    temp->left = nullptr;
                    root->right = temp;
                    temp->parent = root;
                } else {
                    root->right = temp->right;
                    if (root->right)
                        root->right->parent = root;
                    temp->right = nullptr;
                    root->left = temp;
                    temp->parent = root;
                }
                siz += 1;
            }
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
        splay(key);
        if (isEmpty() || !isEmpty() && root->key != key)
            insert(key, (mapped_type)0);
        return root->value;
    }

    /*!
     * zwraca wartosc dla podanego klucza
     */
    const mapped_type& value(const key_type& key)
    {
        splay(key);
        if (!isEmpty() && root->key == key)
            return root->value;
        else
            throw std::runtime_error("element does not exist");
    }

    /*!
     * zwraca informacje, czy istnieje w slowniku podany klucz
     */
    bool contains(const key_type& key) {
        splay(key);
        if (!isEmpty() && root->key == key)
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
    Node<key_type, mapped_type> *root;
    size_t siz;

    void splay(key_type key) {
        Node<key_type, mapped_type> *el = this->findClosest(key);

        while (el != this->root) {
            if (el->parent == this->root) {
                if (el->parent->right && el == el->parent->right) {
                    this->rotateLeft(el);
                } else {
                    this->rotateRight(el);
                }
            } else if (el->parent->left && el->parent->parent->right 
            && el == el->parent->left && el->parent->parent->right == el->parent ) {
                this->rotateRight(el);
                this->rotateLeft(el);
            } else if (el->parent->right && el->parent->parent->left 
            && el == el->parent->right && el->parent->parent->left == el->parent) {
                this->rotateLeft(el);
                this->rotateRight(el);
            } else if (el->parent->left && el->parent->parent->left 
            && el == el->parent->left && el->parent->parent->left == el->parent) { 
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
        if (isEmpty()) {
            return nullptr;
        } else {
            Node<key_type, mapped_type> *currentRoot = this->root;
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
            return currentRoot;
        }
    }

    void rotateLeft(Node<key_type, mapped_type>* el) {
        Node<key_type, mapped_type>* temp;
        if (el->parent == this->root) {
            temp = this->root;
            this->root = el;
            temp->right = el->left;
            if (temp->right)
                temp->right->parent = temp;
            this->root->left = temp;
            temp->parent = this->root;
        } else if (el->parent == el->parent->parent->left) {
            temp = el->parent;
            el->parent->parent->left = el;
            el->parent = temp->parent;

            temp->right = el->left;
            if (el->left)
                el->left->parent = temp;
            el->left = temp;
            temp->parent = el;
        } else if (el->parent == el->parent->parent->right) {
            temp = el->parent;
            el->parent->parent->right = el;
            el->parent = temp->parent;

            temp->right = el->left;
            if (el->left)
                el->left->parent = temp;
            el->left = temp;
            temp->parent = el;
        }
    }

    void rotateRight(Node<key_type, mapped_type>* el) {
        Node<key_type, mapped_type>* temp;
        if (el->parent == this->root) {
            temp = this->root;
            this->root = el;
            temp->left = el->right;
            if (temp->left)
                temp->left->parent = temp;
            this->root->right = temp;
            temp->parent = this->root;
        } else if (el->parent == el->parent->parent->right) {
            temp = el->parent;
            el->parent->parent->right = el;
            el->parent = temp->parent;

            temp->left = el->right;
            if (el->right)
                el->right->parent = temp;
            el->right = temp;
            temp->parent = el;
        } else if (el->parent == el->parent->parent->left) {
            temp = el->parent;
            el->parent->parent->left = el;
            el->parent = temp->parent;

            temp->left = el->right;
            if (el->right)
                el->right->parent = temp;
            el->right = temp;
            temp->parent = el;
        }
    }
};



#include "tests.h"

int main(int argc, char *argv[])
{
    unit_test();
    ifstream fp;
    fp.open("pan-tadeusz.txt");
    if (!fp.is_open()) {
        cerr << "Cannot open file.\n";
    }
    string word;
    unsigned int counter = 0;
    vector<pair<int, string> > words;
    int nWords = stoi(argv[1]);
    while (fp >> word && counter < nWords) {
        words.push_back(make_pair(rand() % nWords, word));
        ++counter;
    }
    fp.close();

    cerr << "Inserting: \n";
    cerr << "TreeMap: \n";
     TreeMap<int, string> dict;
    {
        Benchmark<std::chrono::milliseconds> a(true);
        for (int i = 0; i < nWords; ++i) {
            dict.insert(words[i]);
        }
    }
     cerr << "std::map: \n";
    map<int, string> stdDict;
    {
        Benchmark<std::chrono::milliseconds> b(true);
        for (int i = 0; i < nWords; ++i) {
            stdDict.insert(words[i]);
        }
    } 
    

    cerr << "\nSearching: \n";
    vector<int> randoms;
    for (int i = 0; i < nWords; ++i) {
        randoms.push_back(rand() % nWords);
    }

    cerr << "TreeMap: \n";
    {
        Benchmark<std::chrono::milliseconds> c(true);
        for (int i = 0; i < nWords; ++i) {
            dict.contains(i);
        }
    }

    cerr << "std::map: \n";
    {
        Benchmark<std::chrono::milliseconds> d(true);
        for (int i = 0; i < nWords; ++i) {
            stdDict.count(i);
        }
    }
    return 0;
}
