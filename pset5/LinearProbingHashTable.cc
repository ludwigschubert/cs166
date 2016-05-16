#include <cassert>
#include "LinearProbingHashTable.h"

LinearProbingHashTable::LinearProbingHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family)
{
  this->hashFunction = family->get();
  this->buckets = std::vector<int>(numBuckets);
}

LinearProbingHashTable::~LinearProbingHashTable()
{
  // TODO: Implement this
}

void LinearProbingHashTable::insert(int data)
{
  size_t index = this->index_for_data(data);
  while(this->buckets[index]) {
    if(this->buckets[index] == data) {
      return;
    }
    index = next_index(index);
  }
  this->buckets[index] = data;
}

bool LinearProbingHashTable::contains(int data) const
{
  size_t index = this->index_for_data(data);
  while(this->buckets[index]) {
    if(this->buckets[index] == data) {
      return true;
    }
    index = this->next_index(index);
  }
  return false;
}

void LinearProbingHashTable::remove(int data)
{
  size_t index = this->index_for_data(data);
  while(this->buckets[index]) {
    if(this->buckets[index] == data) {
      this->buckets[index] = 0;
      break;
    }
    index = next_index(index);
  }
  // repair "hole" by moving everything else back...
  index = next_index(index);
  while(this->buckets[index]) {
    size_t correct_index = this->index_for_data(this->buckets[index]);
    if (index == correct_index) {
      return; // until something is hashed correctly...
    }
    size_t previous_index = this->previous_index(index);
    this->buckets[previous_index] = this->buckets[index];
    index = next_index(index);
  } // or there is an empty slot
}

size_t LinearProbingHashTable::previous_index(size_t index) const
{
  size_t previous_index;
  if (index == 0) {
    previous_index = this->buckets.size() - 1;
  } else {
    previous_index = index - 1;
  }
  assert(previous_index < this->buckets.size());
  return previous_index;
}

size_t LinearProbingHashTable::next_index(size_t index) const
{
  return ++index % this->buckets.size();
}

size_t LinearProbingHashTable::index_for_data(int data) const
{
  size_t hash_value = this->hashFunction(data);
  size_t index = hash_value % this->buckets.size();
  return index;
}
