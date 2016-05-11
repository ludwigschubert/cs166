#include <assert.h>
#include <cmath>
#include "WeightBalancedTree.h"

BinaryTreeNode *node_for_weights(size_t start, size_t end, const std::vector<double> &weights, double total_weight) {

  if (start == end) return new BinaryTreeNode(start);

  double left_weight = 0.0, right_weight = 0.0, weight_diff = total_weight, last_weight_diff;
  size_t last_split_point, left = 0, right = 0;
  bool inc_left = true;

  do {
    last_weight_diff = weight_diff;
    if (inc_left) {
      left++;
      left_weight += weights[start + left];
      last_split_point = left;
      weight_diff = fabs(left_weight - (total_weight-left_weight));
    } else {
      right++;
      right_weight += weights[end - right];
      last_split_point = right;
      weight_diff = fabs(right_weight - (total_weight-right_weight));
    }
    inc_left = !inc_left;
  } while (last_weight_diff >= weight_diff);

  BinaryTreeNode *node = new BinaryTreeNode(last_split_point);

  size_t next_end = last_split_point - 1; // TODO: check ranges
  if (start <= next_end) {
    node->left_child = node_for_weights(start, next_end, weights, left_weight);
  }

  size_t next_start = last_split_point + 1;// TODO: check ranges
  if (next_start <= end) {
    node->right_child = node_for_weights(next_start, end, weights, right_weight);
  }

  return node;
}

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
 * elements 0, 1, 2, ..., weights.size() - 1.
 *
 * Notice that the keys are not given to you explicitly, but the assumption
 * is that the keys are 0, 1, 2, ..., weights.size() - 1 with weights
 * weights[0], weights[1], weights[2], ..., weights[weights.size() - 1].
 */
WeightBalancedTree::WeightBalancedTree(const std::vector<double> &weights) {

  // "In time O(n), compute the total sum of the weights"
  double total_weight = 0.0;
  for (auto weight : weights) {
    total_weight += weight;
  }

  // "Then, use the following recursive process"
  this->root = node_for_weights(0, weights.size() - 1, weights, total_weight);
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