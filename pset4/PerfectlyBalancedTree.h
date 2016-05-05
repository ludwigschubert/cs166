#ifndef PerfectlyBalancedTree_Included
#define PerfectlyBalancedTree_Included

#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <vector>

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

  class BinaryTreeNode {
  public:
    int key;
    BinaryTreeNode *left_child;
    BinaryTreeNode *right_child;

    BinaryTreeNode(int key) {
      this->key = key;
      this->left_child = nullptr;
      this->right_child = nullptr;
    }

    /**
     * Recursively constructs a perfectly balanced complete search tree.
     * This does not take keys as we're just assuming an 1...n-1 complete tree.
     */
    static BinaryTreeNode *make_tree(size_t left, size_t right) {

      assert(left <= right);

      if (left == right) {
        return new BinaryTreeNode(left);
      } else {
        size_t pivot = (left + right + 1) / 2;

        BinaryTreeNode *node = new BinaryTreeNode(pivot);

        size_t next_right = pivot - 1;
        if (next_right >= left) {
          node->left_child = make_tree(left, next_right);
        }

        size_t next_left = pivot + 1;
        if (next_left <= right) {
          node->right_child = make_tree(next_left, right);
        }

        return node;
      }
    }
  };

  BinaryTreeNode *root;

  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  PerfectlyBalancedTree(PerfectlyBalancedTree const &) = delete;
  void operator=(PerfectlyBalancedTree const &) = delete;
};

#endif
