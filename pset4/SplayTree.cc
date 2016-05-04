#include "SplayTree.h"

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new splay tree holding those
 * elements.
 *
 * Because splay trees rearrange their elements in response to queries, you
 * can safely ignore the assigned probabilities here and just build a BST
 * storing the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
 */
SplayTree::SplayTree(const std::vector<double>& weights) {
  // TODO: Implement this!
}

/**
 * Frees all memory used by this tree.
 */
SplayTree::~SplayTree() {
  // TODO: Implement this!
}

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool SplayTree::contains(int key) {
  // TODO: Implement this!
  return false;
}
