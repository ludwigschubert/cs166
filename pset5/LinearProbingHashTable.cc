#include <cassert>
#include "LinearProbingHashTable.h"

static int TOMBSTONE = -1;
static int EMPTY = -2;

LinearProbingHashTable::LinearProbingHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family)
{
  this->hashFunction = family->get();
  this->buckets = std::vector<int>(numBuckets, EMPTY);
}

LinearProbingHashTable::~LinearProbingHashTable()
{
  // TODO: Implement this
}

void LinearProbingHashTable::insert(int data)
{
  size_t index = this->index_for_data(data);
  while (this->buckets[index] != EMPTY) {
    if (this->buckets[index] == data) return; // found data; don't insert duplicate
    if (this->buckets[index] == TOMBSTONE) break; // found TOMBSTONE, insert here
    index = next_index(index); // continue scanning
  }
  this->buckets[index] = data;
}

bool LinearProbingHashTable::contains(int data) const
{
  size_t index = this->index_for_data(data);
  while (this->buckets[index] != EMPTY) {
    if (this->buckets[index] == data) return true;
    index = this->next_index(index);
  }
  return false;
}

void LinearProbingHashTable::remove(int data)
{
  size_t index = this->index_for_data(data);
  while(this->buckets[index] != EMPTY) {
    if(this->buckets[index] == data) {
      this->buckets[index] = TOMBSTONE;
      return;
    }
    index = next_index(index);
  }
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
