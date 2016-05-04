#ifndef SplayTree_Included
#define SplayTree_Included

#include <stddef.h>
#include <vector>

/**
 * A type representing a binary search tree backed by a splay tree.
 */
class SplayTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new splay tree holding those
   * elements.
   *
   * Because splay trees rearrange their elements in response to queries, you
   * can safely ignore the assigned probabilities here and just build a BST
   * storing the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
   */
  SplayTree(const std::vector<double>& weights);
  
  /**
   * Cleans up all memory allocated by the tree. Remember that destructors are
   * invoked automatically in C++, so you should never need to directly invoke
   * this member function.
   */
  ~SplayTree();
  
  /**
   * Searches the splay tree for the given key, returning whether or not that
   * key is present in the tree. We strongly recommend implementing this
   * method using the top-down splaying technique outlined in Sleator and
   * Tarjan's original paper.
   */
  bool contains(int key);
  
private:
  // TODO: Add any necessary new types or fields here.


  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  SplayTree(SplayTree const &) = delete;
  void operator=(SplayTree const &) = delete;
};

#endif
