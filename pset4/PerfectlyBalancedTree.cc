#include <assert.h>
#include <cmath>
#include "PerfectlyBalancedTree.h"

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new perfectly-balanced tree holding
 * those elements.
 *
 * Because perfectly-balanced trees don't care about access probabilities,
 * you should ignore the actual weights provided here and just build a
 * perfectly-balanced tree holding the keys 0, 1, 2, ..., weights.size() - 1.
 */
PerfectlyBalancedTree::PerfectlyBalancedTree(const std::vector<double>& weights) {

  // initialize fields
  this->root = nullptr;

  // set up tree structure
  auto skip_size = weights.size();
  while (skip_size > 1) {
    skip_size /= 2;
    for (auto key = 0u; key < weights.size(); key += skip_size) {
      this->insert(key); // this will try to insert duplicates; but it's so easy...
    }
  }
}

/**
 * Frees all memory used by this tree.
 */
PerfectlyBalancedTree::~PerfectlyBalancedTree() {
  // note: NOTHING to do; we're using managed pointers :D
  // 2016-05-03-23-33-57 0 Errors in Valgrind on Ubuntu VM
}

/**
 * Determines whether the specified key is present in the perfectly-balanced
 * tree.
 */
bool PerfectlyBalancedTree::contains(int key) const {

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

void PerfectlyBalancedTree::insert(int key) {

  // start searching from the root
  BinaryTreeNode *node = this->root;

  if (!node) {
    this->root = new BinaryTreeNode(key);
    return;
  }

  // iteratively walk down the tree
  while (node) {
    if (node->key == key) {
      return;
    } else if (node->key > key) {
      if (node->left_child) {
        node = node->left_child;
      } else {
        node->left_child = new BinaryTreeNode(key);
        return;
      }
    } else {
      if (node->right_child) {
        node = node->right_child;
      } else {
        node->right_child = new BinaryTreeNode(key);
        return;
      }
    }
  }

  assert(false); //, "value should either be found or inserted!"
}


