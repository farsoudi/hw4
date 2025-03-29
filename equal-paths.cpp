#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;



bool traverse (Node * root, int depth, int& longest) {
    if (root == nullptr) // empty tree
        return true;

    if (root->left == nullptr && root->right == nullptr) {
        if (longest == 0) {
            longest = depth;
        }
        return depth == longest; 
    }

    bool left = traverse(root->left, depth + 1, longest);
    bool right = traverse(root->right, depth + 1, longest);

    return left && right; 
}


bool equalPaths(Node * root)
{
    int longest = 0;

    return traverse(root, 0, longest);

}

