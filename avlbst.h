#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    void insRecursive(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void remRecursive(AVLNode<Key, Value>* node, int diff); 
    void rotateRight(AVLNode<Key, Value>* node); 
    void rotateLeft(AVLNode<Key, Value>* node);  


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    if (this->root_ == nullptr) { //empty tree just add
        this->root_ = newNode;
        return;
    }

    AVLNode<Key, Value>* curr = dynamic_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;
    const Key& key = new_item.first;
    const Value& val = new_item.second;

    while(curr != nullptr) {
        parent = curr;

        if(key < curr->getKey())
            curr = curr->getLeft();
        else if(key > curr->getKey())
            curr = curr->getRight();
        else {
            curr->setValue(val);
            delete newNode;
            return;
        }
    }



    newNode->setParent(parent);
    if (key < parent->getKey())
        parent->setLeft(newNode);
    else    
        parent->setRight(newNode);

    newNode->setBalance(0);

    if(parent->getBalance() == -1 || parent->getBalance() == 1)
        parent ->setBalance(0);
    else if( parent->getBalance() == 0) {
        if (newNode == parent->getLeft())
            parent->setBalance(-1);
        else
            parent->setBalance(1);

        insRecursive(parent, newNode); //further broken down cases 
        
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insRecursive(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    if (parent == nullptr || parent->getParent() == nullptr) {
        return;
    }

    AVLNode<Key, Value>* grandparent = parent->getParent();

    if (grandparent->getLeft() == parent) {
        grandparent->updateBalance(-1);

        if (grandparent->getBalance() == 0) {
            return;
        } else if (grandparent->getBalance() == -1) {
            insRecursive(grandparent, parent);
        } else if (grandparent->getBalance() == -2) {
            if (parent->getLeft() == node) {
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                rotateLeft(parent);
                rotateRight(grandparent);

                if (node->getBalance() == -1) {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    node->setBalance(0);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                } else if (node->getBalance() == 1) {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    } else {
        grandparent->updateBalance(1);

        if (grandparent->getBalance() == 0) {
            return;
        } else if (grandparent->getBalance() == 1) {
            insRecursive(grandparent, parent);
        } else if (grandparent->getBalance() == 2) {
            if (parent->getRight() == node) {
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                rotateRight(parent);
                rotateLeft(grandparent);

                if (node->getBalance() == 1) {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    node->setBalance(0);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                } else if (node->getBalance() == -1) {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (target == nullptr) return;

    if (target->getLeft() != nullptr && target->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = dynamic_cast<AVLNode<Key, Value>*>(this->predecessor(dynamic_cast<Node<Key, Value>*>(target)));
        nodeSwap(target, pred);
    }

    AVLNode<Key, Value>* parent = target->getParent();
    AVLNode<Key, Value>* child = (target->getLeft() != nullptr) ? target->getLeft() : target->getRight();
    int diff = 0;

    if (parent != nullptr) {
        diff = (target == parent->getLeft()) ? 1 : -1;
    }

    if (parent == nullptr) {
        this->root_ = child;
    } else if (target == parent->getLeft()) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    if (child != nullptr) {
        child->setParent(parent);
    }

    delete target;

    if (parent != nullptr) {
        remRecursive(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* leftChild = node->getLeft();
    node->setLeft(leftChild->getRight());

    if (leftChild->getRight() != nullptr) {
        leftChild->getRight()->setParent(node);
    }

    leftChild->setParent(node->getParent());

    if (node->getParent() == nullptr) {
        this->root_ = leftChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(leftChild);
    } else {
        node->getParent()->setRight(leftChild);
    }

    leftChild->setRight(node);
    node->setParent(leftChild);

    int8_t nodeBal = node->getBalance();
    int8_t leftBal = leftChild->getBalance();

    node->setBalance(nodeBal + static_cast<int8_t>(1) - std::min(static_cast<int8_t>(0), leftBal));
    leftChild->setBalance(leftBal + static_cast<int8_t>(1) + std::max(static_cast<int8_t>(0), nodeBal));
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* rightChild = node->getRight();
    node->setRight(rightChild->getLeft());

    if (rightChild->getLeft() != nullptr) {
        rightChild->getLeft()->setParent(node);
    }

    rightChild->setParent(node->getParent());

    if (node->getParent() == nullptr) {
        this->root_ = rightChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(rightChild);
    } else {
        node->getParent()->setRight(rightChild);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);

    int8_t nodeBal = node->getBalance();
    int8_t rightBal = rightChild->getBalance();

    node->setBalance(nodeBal - static_cast<int8_t>(1) - std::max(static_cast<int8_t>(0), rightBal));
    rightChild->setBalance(rightBal - static_cast<int8_t>(1) + std::min(static_cast<int8_t>(0), nodeBal));
}

template<class Key, class Value>
void AVLTree<Key, Value>::remRecursive(AVLNode<Key, Value>* node, int diff) {
    if (!node) return;

    AVLNode<Key, Value>* parent = node->getParent();
    int ndiff = (parent && node == parent->getLeft()) ? 1 : -1;

    if (diff == -1) {
        int newBalance = node->getBalance() + diff;

        if (newBalance == -2) {
            AVLNode<Key, Value>* child = node->getLeft();

            if (child && child->getBalance() == -1) {
                rotateRight(node);
                node->setBalance(0);
                child->setBalance(0);
                if (parent) remRecursive(parent, ndiff);
            } else if (child && child->getBalance() == 0) {
                rotateRight(node);
                node->setBalance(-1);
                child->setBalance(1);
            } else if (child && child->getBalance() == 1) {
                AVLNode<Key, Value>* grandchild = child->getRight();
                rotateLeft(child);
                rotateRight(node);

                if (grandchild->getBalance() == 1) {
                    node->setBalance(0);
                    child->setBalance(-1);
                    grandchild->setBalance(0);
                } else if (grandchild->getBalance() == 0) {
                    node->setBalance(0);
                    child->setBalance(0);
                    grandchild->setBalance(0);
                } else if (grandchild->getBalance() == -1) {
                    node->setBalance(1);
                    child->setBalance(0);
                    grandchild->setBalance(0);
                }

                if (parent) remRecursive(parent, ndiff);
            }
        } else if (newBalance == -1) {
            node->setBalance(-1);
        } else if (newBalance == 0) {
            node->setBalance(0);
            if (parent) remRecursive(parent, ndiff);
        }
    } else if (diff == 1) {
        int newBalance = node->getBalance() + diff;

        if (newBalance == 2) {
            AVLNode<Key, Value>* child = node->getRight();

            if (child && child->getBalance() == 1) {
                rotateLeft(node);
                node->setBalance(0);
                child->setBalance(0);
                if (parent) remRecursive(parent, ndiff);
            } else if (child && child->getBalance() == 0) {
                rotateLeft(node);
                node->setBalance(1);
                child->setBalance(-1);
            } else if (child && child->getBalance() == -1) {
                AVLNode<Key, Value>* grandchild = child->getLeft();
                rotateRight(child);
                rotateLeft(node);

                if (grandchild->getBalance() == -1) {
                    node->setBalance(0);
                    child->setBalance(1);
                    grandchild->setBalance(0);
                } else if (grandchild->getBalance() == 0) {
                    node->setBalance(0);
                    child->setBalance(0);
                    grandchild->setBalance(0);
                } else if (grandchild->getBalance() == 1) {
                    node->setBalance(-1);
                    child->setBalance(0);
                    grandchild->setBalance(0);
                }

                if (parent) remRecursive(parent, ndiff);
            }
        } else if (newBalance == 1) {
            node->setBalance(1);
        } else if (newBalance == 0) {
            node->setBalance(0);
            if (parent) remRecursive(parent, ndiff);
        }
    }
}


#endif
