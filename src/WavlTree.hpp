#ifndef __PROJ_FOUR_WAVL_TREE_HPP
#define __PROJ_FOUR_WAVL_TREE_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

namespace shindler::ics46::project4 {

template <typename K, typename V>
class WavlTree {
   private:
   struct Node
   {
        K key;
        V value;
        Node* rightChild;
        Node* leftChild;
        Node* parent;
        bool isEmpty;
        int rank; // the bottom-most node has rank 0
   };
   Node* root;
    // fill in private member data here
    // If you need to declare private functions, do so here too.

   public:
    WavlTree();

    // In general, a copy constructor and assignment operator
    // are good things to have.
    // For this quarter, I am not requiring these.
    //	WavlTree(const WavlTree & st);
    //	WavlTree & operator=(const WavlTree & st);
    // You do not need to implement these
    WavlTree(const WavlTree &) = delete;
    WavlTree(WavlTree &&) = delete;
    WavlTree &operator=(const WavlTree &) = delete;
    WavlTree &operator=(WavlTree &&) = delete;

    // The destructor is required.
    ~WavlTree();

    // size() returns the number of distinct keys in the tree.
    [[nodiscard]] size_t size() const noexcept;

    // isEmpty() returns true if and only if the tree has no values in it.
    [[nodiscard]] bool empty() const noexcept;

    // contains() returns true if and only if there
    //  is a (key, value) pair in the tree
    //	that has the given key as its key.
    bool contains(const K &key) const noexcept;

    // returns the rank on which the node with this key resides.
    // If !contains(k), this will return a std::nullopt.
    std::optional<unsigned> rank(const K &key) const;

    // find returns the value associated with the given key
    // If !contains(k), this function will throw a std::runtime_error().
    // There needs to be a version for const and non-const WavlTrees.
    V& find(const K &key);
    const V& find(const K &key) const;

    // Inserts the given key-value pair into
    // the tree and performs the balancing operation(s)
    // if needed as described in lecture.
    // If the key already exists in the tree,
    // you may do as you please (no test cases in
    // the grading script will deal with this situation)
    void insert(const K &key, const V &value);

    bool promote(Node* q);

    void restructure(Node* t);

    void caseOne(Node* t, bool tLeft, bool pLeft);
    void caseTwo(Node* t, bool tLeft, bool pLeft);
    void caseThree(Node* t, bool tLeft, bool pLeft);
    void caseFour(Node* t, bool tLeft, bool pLeft);

    //note that pRight is for p at end result, pLeft is for p at beginning
    void q_parent(Node* t, bool pRight, bool pLeft);
    void t_parent(Node* t, bool pRight, bool pLeft);

    void makeRightChild(Node* parent, Node* rightChild);
    void makeLeftChild(Node* parent, Node* leftChild);

    // Deletes the given key from the tree
    // and performs the balancing operation(s) if needed.
    // If the key does not exist in the tree,
    // do not modify the tree.
    void remove(const K &key);

    void demoteParent(Node* t);

    // The following three functions all return
    // the set of keys in the tree as a standard vector.
    // Each returns them in a different order.
    [[nodiscard]] std::vector<K> inOrder() const;
    [[nodiscard]] std::vector<K> preOrder() const;
    [[nodiscard]] std::vector<K> postOrder() const;

    void inOrderHelper(Node* curr, std::vector<K>& vec) const;
    void preOrderHelper(Node* curr, std::vector<K>& vec) const;
    void postOrderHelper(Node* curr, std::vector<K>& vec) const;
};

template <typename K, typename V>
WavlTree<K, V>::WavlTree() : root(new Node{K{}, V{}, nullptr, nullptr, nullptr, true, 0}) {
    
}

template <typename K, typename V>
WavlTree<K, V>::~WavlTree() {
    std::cout << "got to destructor" << std::endl;

    auto leftmost = [](Node* n) 
    {
        while (n!=nullptr && !n->isEmpty && !n->leftChild->isEmpty) { n = n->leftChild; }
        return n;
    };

    Node* to_delete = nullptr;

    Node* curr = leftmost(root);
    while(curr != nullptr && !curr->isEmpty)
    {
        if(!curr->rightChild->isEmpty) 
        { 
            curr = leftmost(curr->rightChild); 
        }
        else //right child is empty, end of postorder going
        {
            delete to_delete;
            to_delete = curr;
            while (curr->parent!= nullptr && (curr->parent->rightChild == curr || 
            curr->parent->rightChild->isEmpty)) //while im a non-root and (a right child, or left child's parent with no right child), go up to parent and add it
            {
                curr = curr->parent;
                delete to_delete;
                to_delete = curr;
            }
            //rn im either a left child of a parent with a right child or the root
            curr = curr->parent;
        }
    }
    std::cout << "got out of des" << std::endl;
}

template <typename K, typename V>
size_t WavlTree<K, V>::size() const noexcept {
    //essentially preorder
    unsigned int size = 0;

    Node *t = root;

    while(t != nullptr && !t->isEmpty)
    {
        ++size;
        if(!t->leftChild->isEmpty) 
        { 
            t = t->leftChild; 
        }
        else if (!t->rightChild->isEmpty) 
        {
            t = t->rightChild;
        }
        else //im at a leaf node, so i need to go back up to the next unseen right child
        {  
            while(t->parent != nullptr && (t->parent->rightChild == t
            || t->parent->rightChild->isEmpty))
            {
                t = t->parent;
            }
            if(t->parent != nullptr)
            {
                t = t->parent->rightChild;
            }
            else 
            {
                t = t->parent;
            }
        }
    }
    return size;
}

template <typename K, typename V>
bool WavlTree<K, V>::empty() const noexcept {
    return root->isEmpty;
}

template <typename K, typename V>
bool WavlTree<K, V>::contains(const K &key) const noexcept {
    Node* current = root;
    while (current != nullptr && !current->isEmpty) 
    {
        if (current->key == key) 
        {
            return true;
        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    }
    return false;
}

template <typename K, typename V>
std::optional<unsigned> WavlTree<K, V>::rank(const K &key) const {
    Node* current = root;
    while (current != nullptr && !current->isEmpty) 
    {
        if (current->key == key) 
        {
            return current->rank;
        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    }
    return std::nullopt;
}

template <typename K, typename V>
V& WavlTree<K, V>::find(const K &key) {
    Node* current = root;
    while (current != nullptr && !current->isEmpty) 
    {
        if (current->key == key) 
        {
            return current->value;
        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    }
    throw std::runtime_error("Key not found.");
}

template <typename K, typename V>
const V& WavlTree<K, V>::find(const K &key) const {
    Node* current = root;
    while (current != nullptr && !current->isEmpty) 
    {
        if (current->key == key) 
        {
            return current->value;
        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    }
    throw std::runtime_error("Key not found.");
}

template <typename K, typename V>
void WavlTree<K, V>::insert(const K &key, const V &value) {
    Node* current = root;
    std::cout << "just entered insert, about to put in " << key << std::endl;
    
    
    while (!current->isEmpty) 
    {
        if (current->key == key) 
        {
            return;
        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    } 
    //so current is empty, we can insert here
    current->key = key;
    current->value = value;
    current->isEmpty = false;
    current->leftChild = new Node{K{}, V{}, nullptr, nullptr, current, true, 0};
    current->rightChild = new Node{K{}, V{}, nullptr, nullptr, current, true, 0};

    //alright, now i need to promote.
    std::cout << "got to promote" << std::endl;
    promote(current);
    Node* printed = root;
    while(printed!=nullptr)
    {
        std::cout << "curr node is " << printed->key << std::endl;
        printed = printed->leftChild;
    }
    printed = root;
    std::cout << "back to root!" << std::endl;
    while(printed!=nullptr)
    {
        std::cout << "curr node is " << printed->key << std::endl;
        printed = printed->rightChild;
    }

    printed = root;
    std::cout << "back to root!" << std::endl;
    if(!printed->isEmpty)
    {
        if(!printed->leftChild->isEmpty && !printed->leftChild->rightChild->isEmpty)
        {
            std::cout << "right of left is " << current->key << std::endl;
        }
    }
    
}

template <typename K, typename V>
bool WavlTree<K, V>::promote(Node* q) {
    ++q->rank;
    std::cout << "node " << q->key << " is rank " << q->rank << std::endl;
    
    Node* p = q->parent; //nullpointer at first one
    if (p == nullptr)
    {
        return true;
    }
    Node* s = (q == p->leftChild) ? p->rightChild : p->leftChild;
    if (p == nullptr || q->rank != p->rank)
    {
        return true;
    }

    int rankDifference = p->rank - s->rank;
    switch (rankDifference) 
    {
        case 1:
        {
            if (!promote(p))
            {
                std::cout << "im restructuring?!" << std::endl;
                std::cout << "its from node " << q->key << std::endl;
                restructure(q);
            };
            return true;
        }
        case 2:
        {
            return false;
        }
    }
    return false; //i should NEVER get here
    
}

template <typename K, typename V>
void WavlTree<K, V>::restructure(Node* t)
{
    //currently, my parent's rank is the same as its parent... i think
    //no, that's only the case if i just inserted
    Node* q = t->parent;
    Node* p = q->parent;
    Node* THE_parent = p->parent;

    bool sameRank = (q->rank == p->rank);

    bool qLeft = (p->leftChild == q);
    bool tLeft = (q->leftChild == t);

    bool pLeft = (THE_parent != nullptr && THE_parent->leftChild == p);

    int caseNum = (sameRank ? 0 : 2) + ((qLeft==tLeft) ? 1 : 2);

    switch (caseNum) 
    {
        case 1: caseOne(t, tLeft, pLeft); break;
        case 2: caseTwo(t, tLeft, pLeft); break;
        case 3: caseThree(t, tLeft, pLeft); break;
        case 4: caseFour(t, tLeft, pLeft); break;
    }

    while (root->parent!=nullptr) //surely this wont cause problems...
    {
        root = root->parent;
    }
}


template <typename K, typename V>
void WavlTree<K, V>::caseOne(Node* t, bool tLeft, bool pLeft)
{
    --t->parent->parent->rank;
    q_parent(t, tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseTwo(Node* t, bool tLeft, bool pLeft)
{
    ++t->rank;
    --t->parent->rank;
    --t->parent->parent->rank;
    t_parent(t, !tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseThree(Node* t, bool tLeft, bool pLeft)
{
    ++t->parent->rank;
    --t->parent->parent->rank;
    q_parent(t, tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseFour(Node* t, bool tLeft, bool pLeft)
{
    t->rank += 2;
    t->parent->parent->rank -= 2;
    --t->parent->rank;
    t_parent(t, !tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::q_parent(Node* t, bool pRight, bool pLeft)
{
    Node* q = t->parent;
    Node* p = q->parent;
    Node* THE_parent = p->parent;

    if(pRight)
    {
        Node* q_orig_right_child = q->rightChild;

        makeRightChild(q, p);
        makeLeftChild(q->rightChild, q_orig_right_child);
    }
    else
    {
        Node* q_orig_left_child = q->leftChild;

        makeLeftChild(q, p);
        makeRightChild(q->leftChild, q_orig_left_child);
    }

    if(pLeft)
    {
        makeLeftChild(THE_parent, q);
    }
    else 
    {
        makeRightChild(THE_parent, q);
    }
}

template <typename K, typename V>
void WavlTree<K, V>::t_parent(Node* t, bool pRight, bool pLeft)
{
    Node* q = t->parent;
    Node* p = q->parent;
    Node* THE_parent = p->parent;

    Node* t_new_left_child = p;
    Node* t_new_right_child = q;

    if (pRight)
    {
        t_new_left_child = q;
        t_new_right_child = p;
    }

    makeRightChild(t_new_left_child, t->leftChild);
    makeLeftChild(t_new_right_child, t->rightChild);

    makeLeftChild(t, t_new_left_child);
    makeRightChild(t, t_new_right_child);

    if(pLeft)
    {
        makeLeftChild(THE_parent, t);
    }
    else 
    {
        makeRightChild(THE_parent, t);
    }
}


template <typename K, typename V>
void WavlTree<K, V>::makeRightChild(Node* parent, Node* rightChild)
{
    rightChild->parent = parent;
    if(parent == nullptr)
    {
        return;
    }
    parent->rightChild = rightChild;
}

template <typename K, typename V>
void WavlTree<K, V>::makeLeftChild(Node* parent, Node* leftChild)
{
    leftChild->parent = parent;
    if(parent == nullptr)
    {
        return;
    }
    parent->leftChild = leftChild;
}

template <typename K, typename V>
void WavlTree<K, V>::remove(const K &key) {
    Node* current = root;
    while (current != nullptr && !current->isEmpty) 
    {
        if (current->key == key) 
        {
            //UNLEASH EXCHANGE PROTOCOL ;(
            if (!current->leftChild->isEmpty && !current->rightChild->isEmpty)
            {
                std::swap(current->key, current->rightChild->key);
                std::swap(current->value, current->rightChild->value);
                current = current->rightChild;
                while(!current->leftChild->isEmpty)
                {
                    std::swap(current->key, current->leftChild->key);
                    std::swap(current->value, current->leftChild->value);
                    current = current->leftChild;
                }
            }

            Node* to_delete = current;
            if(current->rightChild->isEmpty)
            {
                delete current->rightChild;
                current = current->leftChild;
            }
            else 
            {
                delete current->leftChild;
                current = current->rightChild;
            }

            if (to_delete->parent != nullptr && to_delete == to_delete->parent->leftChild)
            {
                makeLeftChild(to_delete->parent, current);
            }
            else 
            {
                makeRightChild(to_delete->parent, current);
            }

            delete to_delete;

            demoteParent(current);

            if(current->parent != nullptr 
            && current->parent->leftChild->isEmpty
            && current->parent->rightChild->isEmpty 
            && current->parent->rank - current->parent->leftChild->rank == 2
            && current->parent->rank - current->parent->rightChild->rank == 2)
            {
                --current->parent->rank; //...hopefully prevents 2-2
                demoteParent(current->parent);
            }
            
            //ALL RIGHT, two cases
            //a) somehow right child was empty by default, i delete that
            //b) for all other cases, left is empty, i delete that

        }
        current = (key<current->key) ? current->leftChild : current->rightChild;
    }
}

template <typename K, typename V>
void WavlTree<K, V>::demoteParent(Node* q) 
{
    Node* p = q->parent;
    if(p == nullptr || (p->rank - q->rank) == 2)
    {
        return;
    }
    //now i know parent is not null and rank difference is 3 (prob...)
    // assert(p->rank - q->rank == 3);

    Node* s = (p->leftChild == q) ? p->rightChild : p->leftChild;
    unsigned int rankDiff = p->rank - s->rank;
    switch (rankDiff) 
    {
        case 2: 
        {
            --p->rank;
            demoteParent(p);
            break;
        }
        case 1:
        {
            if(s->leftChild!=nullptr && s->rightChild!=nullptr
                && s->leftChild->isEmpty && s->rightChild->isEmpty
                && s->rank - s->leftChild->rank == 2 
                && s->rank - s->rightChild->rank == 2)
                {
                    --p->rank;
                    --s->rank;
                    demoteParent(p);
                }
            else 
            {
                if(!q->rightChild->isEmpty)
                {
                    restructure(q->rightChild);
                }
                else  //i think left can't be empty here
                {
                    restructure(q->leftChild);
                }
            }
        }
    }
}

template <typename K, typename V>
std::vector<K> WavlTree<K, V>::inOrder() const {
    std::vector<K> inOrder{};
    inOrderHelper(root, inOrder);
    return inOrder;
}

template <typename K, typename V>
void WavlTree<K, V>::inOrderHelper(Node* curr, std::vector<K>& vec) const 
{
    if(curr->isEmpty)
    {
        return;
    }
    inOrderHelper(curr->leftChild, vec);
    vec.push_back(curr->key);
    inOrderHelper(curr->rightChild, vec);
}

template <typename K, typename V>
std::vector<K> WavlTree<K, V>::preOrder() const {
    std::vector<K> preOrder{};
    preOrderHelper(root, preOrder);
    return preOrder;
}

template <typename K, typename V>
void WavlTree<K, V>::preOrderHelper(Node* curr, std::vector<K>& vec) const 
{
    if(curr->isEmpty)
    {
        return;
    }
    vec.push_back(curr->key);
    preOrderHelper(curr->leftChild, vec);
    preOrderHelper(curr->rightChild, vec);
}

template <typename K, typename V>
std::vector<K> WavlTree<K, V>::postOrder() const {
    std::vector<K> postOrder{};
    postOrderHelper(root, postOrder);
    return postOrder;
}

template <typename K, typename V>
void WavlTree<K, V>::postOrderHelper(Node* curr, std::vector<K>& vec) const 
{
    if(curr->isEmpty)
    {
        return;
    }
    postOrderHelper(curr->leftChild, vec);
    postOrderHelper(curr->rightChild, vec);
    vec.push_back(curr->key);
}

}  // namespace shindler::ics46::project4

#endif
