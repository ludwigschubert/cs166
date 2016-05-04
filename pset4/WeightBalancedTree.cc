#include <assert.h>
#include <iostream>
#include "WeightBalancedTree.h"

#define DEBUG

using namespace std;

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
 * elements 0, 1, 2, ..., weights.size() - 1.
 *
 * Notice that the keys are not given to you explicitly, but the assumption
 * is that the keys are 0, 1, 2, ..., weights.size() - 1 with weights
 * weights[0], weights[1], weights[2], ..., weights[weights.size() - 1].
 */
WeightBalancedTree::WeightBalancedTree(const vector<double>& weights) {

  //initialize fields
  this->root = nullptr;

  // construct weight balanced tree structure
  vector<size_t> ordered_indices = ordered(weights);
  for (auto i : ordered_indices) {
    this->insert(i);
  }
}

/**
 * Frees all memory used by this tree.
 */
WeightBalancedTree::~WeightBalancedTree() {
  // TODO: Implement this!
}

/**
 * Determines whether the specified key is present in the weight-balanced
 * tree.
 */
bool WeightBalancedTree::contains(int key) const {

  // start searching from the root
  BinaryTreeNode *node = this->root;

  // iteratively walk down the tree
  while (node) {
    if (node->key == key) {
      return true;
    } else if (node->key > key) {
      node = node->left_child;
    } else {
      node = node->right_child;
    }
  }

  // found a null-path; giving up
  return false;
}

void WeightBalancedTree::insert(int key) {

  // start searching from the root
  BinaryTreeNode *node = this->root;

  if (!node) {
    this->root = new BinaryTreeNode(key);
    return;
  }

//  auto depth = 0u;

  // iteratively walk down the tree
  while (node) {
//   depth++;
    if (node->key == key) {
      return;
    } else if (node->key > key) {
      if (node->left_child) {
        node = node->left_child;
      } else {
        node->left_child = new BinaryTreeNode(key);
//        cout << "inserted at depth ";
//        cout << depth;
//        cout << endl;
        return;
      }
    } else {
      if (node->right_child) {
        node = node->right_child;
      } else {
        node->right_child = new BinaryTreeNode(key);
//        cout << "inserted at depth ";
//        cout << depth;
//        cout << endl;
        return;
      }
    }
  }

  assert(false); //, "value should either be found or inserted!"
}