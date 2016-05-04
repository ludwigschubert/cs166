#ifndef PerfectlyBalancedTree_Included
#define PerfectlyBalancedTree_Included

#include <stddef.h>
#include <iostream>
#include <vector>

class BinaryTreeNode {
public:
  BinaryTreeNode(int key) {
    this->key = key;
    this->left_child = nullptr;
    this->right_child = nullptr;
  }

  int key;
  BinaryTreeNode *left_child;
  BinaryTreeNode *right_child;
};

class PerfectlyBalancedTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
   * elements 0, 1, 2, ..., weights.size() - 1.
   *
   * Because perfectly balanced trees have to be balanced without regard to the
   * access probabilities of the underlying elements, you should ignore the
   * assigned probabilities here and just build an optimally-balanced BST for
   * the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
   */
  PerfectlyBalancedTree(const std::vector<double>& weights);

  /**
   * Cleans up all memory allocated by the tree. Remember that destructors are
   * invoked automatically in C++, so you should never need to directly invoke
   * this member function.
   */
  ~PerfectlyBalancedTree();

  /**
   * Searches the tree for the given key, returning whether or not that key is
   * present in the tree.
   */
  bool contains(int key) const;
private:
  BinaryTreeNode *root;

  void insert(int key);

  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  PerfectlyBalancedTree(PerfectlyBalancedTree const &) = delete;
  void operator=(PerfectlyBalancedTree const &) = delete;
};

#endif
