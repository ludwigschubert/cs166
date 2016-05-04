#ifndef WeightBalancedTree_Included
#define WeightBalancedTree_Included

#include <stddef.h>
#include <vector>

class WeightBalancedTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
   * elements 0, 1, 2, ..., weights.size() - 1.
   *
   * Notice that the keys are not given to you explicitly, but the assumption
   * is that the keys are 0, 1, 2, ..., weights.size() - 1 with weights
   * weights[0], weights[1], weights[2], ..., weights[weights.size() - 1].
   */
  WeightBalancedTree(const std::vector<double>& weights);
  
  /**
   * Cleans up all memory allocated by the tree. Remember that destructors are
   * invoked automatically in C++, so you should never need to directly invoke
   * this member function.
   */
  ~WeightBalancedTree();
  
  /**
   * Returns whether the given key is present in the weight-balanced tree.
   */
  bool contains(int key) const;

private:
  // TODO: Add any necessary new types or fields here.
  
  
  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  WeightBalancedTree(WeightBalancedTree const &) = delete;
  void operator=(WeightBalancedTree const &) = delete;
};

#endif
