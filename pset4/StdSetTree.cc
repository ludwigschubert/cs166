#include "StdSetTree.h"
using namespace std;

/* The constructor here is given the access probabilities of the numbers 0, 1,
 * 2, ..., weights.size() - 1, but it can't actually use them because std::set
 * is agnostic to access probabilities. We therefore completely ignore the
 * weights and just use the vector for information about what keys to store in
 * the hash table.
 */
StdSetTree::StdSetTree(const std::vector<double>& weights) {
  for (size_t i = 0; i < weights.size(); i++) {
    elems.insert(int(i));
  }
}

StdSetTree::~StdSetTree() {
  /* The std::set type automatically reclaims all its memory, so this
   * destructor doesn't need to do anything. On the other hand, your
   * data structures probably will need to free all the memory you've
   * allocated.
   */
}

bool StdSetTree::contains(int key) const {
  return elems.find(key) != elems.end();
}
