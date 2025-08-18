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
    [[nodiscard]] size_t sizeHelper(Node* curr) const noexcept;

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
    void promote(Node* q);
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

    bool isTwoTwoNode(Node* curr);
    bool isTwoTwoNodeLegal(Node* curr);

    // Deletes the given key from the tree
    // and performs the balancing operation(s) if needed.
    // If the key does not exist in the tree,
    // do not modify the tree.
    void remove(const K &key);
    void remove_node(Node* q);
    void demoteParent(Node* q);

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

    auto leftmost = [](Node* n) 
    {
        while (n!=nullptr && n->leftChild!=nullptr) { n = n->leftChild; }
        return n;
    };

    Node* to_delete = nullptr;

    Node* curr = leftmost(root);
    while(curr != nullptr)
    {
        if(curr->rightChild!=nullptr) 
        { 
            curr = leftmost(curr->rightChild); 
        }
        else //right child is empty, end of postorder going
        {
            delete to_delete;
            to_delete = curr;
            while (curr->parent!= nullptr && (curr->parent->rightChild == curr || 
            curr->parent->rightChild==nullptr)) //while im a non-root and (a right child, or left child's parent with no right child), go up to parent and add it
            {
                curr = curr->parent;
                delete to_delete;
                to_delete = curr;
            }
            //rn im either a left child of a parent with a right child or the root
            if(curr->parent == nullptr)
            {
                delete curr;
                curr = nullptr;
            }
            else 
            {
                curr = curr->parent;
            }
        }
    }
}

template <typename K, typename V>
size_t WavlTree<K, V>::size() const noexcept {
    //essentially preorder
    return sizeHelper(root);
    // unsigned int size = 0;

    // Node *t = root;

    // while(t != nullptr && !t->isEmpty)
    // {
    //     ++size;
    //     if(!t->leftChild->isEmpty) 
    //     { 
    //         t = t->leftChild; 
    //     }
    //     else if (!t->rightChild->isEmpty) 
    //     {
    //         t = t->rightChild;
    //     }
    //     else //im at a leaf node, so i need to go back up to the next unseen right child
    //     {  
    //         while(t->parent != nullptr && (t->parent->rightChild == t
    //         || t->parent->rightChild->isEmpty))
    //         {
    //             t = t->parent;
    //         }
    //         if(t->parent != nullptr)
    //         {
    //             t = t->parent->rightChild;
    //         }
    //         else 
    //         {
    //             t = t->parent;
    //         }
    //     }
    // }
    // return size;
}

template <typename K, typename V>
size_t WavlTree<K, V>::sizeHelper(Node* curr) const noexcept {
    if(curr->isEmpty)
    {
        return 0;
    }
    return 1 + sizeHelper(curr->leftChild) + sizeHelper(curr->rightChild);
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
    Node* q = root;
    
    while (!q->isEmpty) 
    {
        if (q->key == key) 
        {
            return;
        }
        q = (key<q->key) ? q->leftChild : q->rightChild;
    } 
    //so q is empty, we can insert here
    q->key = key;
    q->value = value;
    q->isEmpty = false;
    q->leftChild = new Node{K{}, V{}, nullptr, nullptr, q, true, 0};
    q->rightChild = new Node{K{}, V{}, nullptr, nullptr, q, true, 0};

    //alright, now i need to promote.
    promote(q);
}

template <typename K, typename V>
void WavlTree<K, V>::promote(Node* q) {
    ++q->rank;
    // std::cout << "node " << q->key << " is rank " << q->rank << std::endl;
    
    Node* p = q->parent; //nullpointer at first one
    if (p == nullptr || q->rank != p->rank)
    {
        return;
    }
    Node* s = (q == p->leftChild) ? p->rightChild : p->leftChild;

    int rankDifference = p->rank - s->rank;
    switch (rankDifference) 
    {
        case 1:
        {
            promote(p);
            break;
        }
        case 2:
        {
            Node* t = (q->leftChild->rank > q->rightChild->rank ? q->leftChild : q->rightChild);
            restructure(t); //t is closest child of q
        }
    }    
}

template <typename K, typename V>
void WavlTree<K, V>::restructure(Node* t)
{
    //currently, my parent's rank is the same as its parent... i think
    //no, that's only the case if i just inserted
    Node* q = t->parent;
    Node* p = q->parent;
    Node* THE_parent = p->parent;

    bool sameRank = (q->rank == p->rank); //case 1&2 or 3&4

    bool qLeft = (p->leftChild == q);
    bool tLeft = (q->leftChild == t);
    // case 1&3 (left and left) or 2&4 (left and right)

    bool pLeft = (THE_parent != nullptr && THE_parent->leftChild == p);
    //just whether THE parent takes our parent as left or right child

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
    --t->parent->parent->rank; // --p->rank
    q_parent(t, tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseTwo(Node* t, bool tLeft, bool pLeft)
{
    ++t->rank;
    --t->parent->rank; // --q->rank
    --t->parent->parent->rank; // --p->rank
    t_parent(t, !tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseThree(Node* t, bool tLeft, bool pLeft)
{
    ++t->parent->rank; // ++q->rank
    --t->parent->parent->rank; // --p->rank
    q_parent(t, tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::caseFour(Node* t, bool tLeft, bool pLeft)
{
    t->rank += 2;
    t->parent->parent->rank -= 2; // p->rank -= 2
    --t->parent->rank; // --q->rank
    t_parent(t, !tLeft, pLeft);
}

template <typename K, typename V>
void WavlTree<K, V>::q_parent(Node* t, bool pRight, bool pLeft)
{
    Node* q = t->parent;
    Node* p = q->parent;
    Node* THE_parent = p->parent;

    if(pRight) //if p ends up on the right
    {
        Node* q_orig_right_child = q->rightChild;

        makeRightChild(q, p);
        makeLeftChild(p, q_orig_right_child);
    }
    else
    {
        Node* q_orig_left_child = q->leftChild;

        makeLeftChild(q, p);
        makeRightChild(p, q_orig_left_child);
    }

    if(pLeft)
    {
        makeLeftChild(THE_parent, q);
    }
    else 
    {
        makeRightChild(THE_parent, q);
    }

    if(isTwoTwoNode(q->leftChild))
    {
        --q->leftChild->rank;
        demoteParent(q->leftChild);
    }
    if(isTwoTwoNode(q->rightChild))
    {
        --q->rightChild->rank;
        demoteParent(q->rightChild);
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

    if (pRight) // if p ends up on the right
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

    if(isTwoTwoNode(t->leftChild))
    {
        --t->leftChild->rank;
        demoteParent(t->leftChild);
    }
    if(isTwoTwoNode(t->rightChild))
    {
        --t->rightChild->rank;
        demoteParent(t->rightChild);
    }
}


template <typename K, typename V>
void WavlTree<K, V>::makeRightChild(Node* parent, Node* rightChild)
{
    rightChild->parent = parent;
    if(parent == nullptr)
    {
        root = rightChild;
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
        root = leftChild;
        return;
    }
    parent->leftChild = leftChild;
}


template <typename K, typename V>
bool WavlTree<K, V>::isTwoTwoNode(Node* curr)
{
    return (curr != nullptr && !curr->isEmpty
                && curr->leftChild->isEmpty && curr->rightChild->isEmpty
                && curr->rank - curr->leftChild->rank == 2 
                && curr->rank - curr->rightChild->rank == 2);
}

template <typename K, typename V>
bool WavlTree<K, V>::isTwoTwoNodeLegal(Node* curr)
{
    return (curr != nullptr && !curr->isEmpty
                && curr->rank - curr->leftChild->rank == 2 
                && curr->rank - curr->rightChild->rank == 2);
}

template <typename K, typename V>
void WavlTree<K, V>::remove_node(Node* q) {
    //UNLEASH EXCHANGE PROTOCOL ;(
    if (!q->leftChild->isEmpty && !q->rightChild->isEmpty)
    {
        Node* successor = q;
        successor = successor->rightChild;
        while(!successor->leftChild->isEmpty)
        {
            successor = successor->leftChild;
        }
        std::swap(q->key, successor->key);
        std::swap(q->value, successor->value);
        q = successor;
    }

    Node* u = q;
    if(u->rightChild->isEmpty)
    {
        delete u->rightChild; // v
        q = q->leftChild; //q
    }
    else 
    {
        delete q->leftChild; // v
        q = q->rightChild; // q
    }

    Node* p = u->parent;
    if (p != nullptr && u == p->leftChild) 
    {
        makeLeftChild(p, q);
    }
    else 
    {
        makeRightChild(p, q);
    }

    delete u;

    demoteParent(q);

    if(isTwoTwoNode(q->parent))
    {
        --q->parent->rank; //...hopefully prevents 2-2
        demoteParent(q->parent);
    }
}

template <typename K, typename V>
void WavlTree<K, V>::remove(const K &key) {
    Node* q = root;
    while (q != nullptr && !q->isEmpty) 
    {
        if (q->key == key) 
        {
            remove_node(q);
            break;
        }
        q = (key < q->key) ? q->leftChild : q->rightChild;
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
            if(isTwoTwoNodeLegal(s))
            {
                --p->rank;
                --s->rank;
                demoteParent(p);
            }
            else 
            {
                Node* t = (s->leftChild->rank > s->rightChild->rank) ? s->leftChild : s->rightChild;
                restructure(t); //t is the nearest child of s
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
