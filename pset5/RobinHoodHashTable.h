#ifndef RobinHoodHashTable_Included
#define RobinHoodHashTable_Included

#include "Hashes.h"

#include <vector>

class RobinHoodHashTable {
public:
  /**
   * Constructs a new Robing Hood table with the specified number of buckets,
   * using hash functions drawn from the indicated family of hash functions.
   * Because our testing harness attempts to exercise a number of different
   * load factors, you should not change the number of buckets once the hash
   * table has initially be created.
   *
   * You can choose a hash function out of the family of hash functions by
   * declaring a variable of type HashFunction and assigning it the value
   * family->get(). For example:
   *
   *    HashFunction h;
   *    h = family->get();
   */
  RobinHoodHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family);
  
  /**
   * Cleans up all memory allocated by this hash table.
   */
  ~RobinHoodHashTable();
  
  /**
   * Inserts the specified element into this hash table. If the element already
   * exists, this operation is a no-op.
   */
  void insert(int key);
  
  /**
   * Returns whether the specified key is contained in this hash tasble.
   */
  bool contains(int key) const;
  
  /**
   * Removes the specified element from this hash table. If the element is not
   * present in the hash table, this operation is a no-op.
   *
   * You should implement this operation using backward-shift deletion: once
   * you've found the element to remove, continue scanning forward until you
   * find an element that is at its home location or an empty cell, then shift
   * each element up to that point backwards by one step.
   */
  void remove(int key);

  inline size_t index_for_data(int data) const;
  inline size_t previous_index(size_t index) const;
  inline size_t next_index(size_t index) const;
  inline size_t index_distance(size_t index1, size_t index2) const;
  
private:
  std::vector<std::pair<int, size_t>> buckets;
  HashFunction hashFunction;
  
  
  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  RobinHoodHashTable(RobinHoodHashTable const &) = delete;
  void operator=(RobinHoodHashTable const &) = delete;
};

#endif
