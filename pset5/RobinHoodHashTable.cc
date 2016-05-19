#include "RobinHoodHashTable.h"

static int EMPTY = -1;

RobinHoodHashTable::RobinHoodHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->hashFunction = family->get();
  this->buckets = std::vector<std::pair<int, size_t>>(numBuckets, std::pair<int, size_t>(EMPTY, 0));
}

RobinHoodHashTable::~RobinHoodHashTable() {
  // TODO: Implement this
}

void RobinHoodHashTable::insert(int data) {
  size_t index = this->index_for_data(data);
  size_t home = index;
  int data_at_index;
  size_t home_at_index;
  while(this->buckets[index].first != EMPTY) {
    std::tie(data_at_index, home_at_index) = this->buckets[index];
    if (data_at_index == data) return; // found data; don't insert duplicate
    size_t data_at_index_distance = index_distance(index, home_at_index);
    size_t data_distance          = index_distance(index, home);
    if (data_at_index_distance < data_distance) {
      this->buckets[index] = std::pair<int, size_t>(data, home);
      insert(data_at_index);
      return;
    }
    index = next_index(index); // continue scanning
  }
  this->buckets[index] = std::pair<int, size_t>(data, home);
}

bool RobinHoodHashTable::contains(int data) const {
  size_t index = this->index_for_data(data);
  size_t home = index;
  int data_at_index;
  size_t home_at_index;
  while(this->buckets[index].first != EMPTY) {
    std::tie(data_at_index, home_at_index) = this->buckets[index];
    if (data_at_index == data) return true;
    size_t data_at_index_distance = index_distance(index, home_at_index);
    size_t data_distance          = index_distance(index, home);
    if (data_at_index_distance < data_distance) return false;
    index = this->next_index(index);
  }
  return false;
}

void RobinHoodHashTable::remove(int data) {
  size_t index = this->index_for_data(data);
  size_t home = index;
  int data_at_index;
  size_t home_at_index;

  // search for element to remove
  while(true) {
    std::tie(data_at_index, home_at_index) = this->buckets[index];

    // found?
    if (data_at_index == data) break;

    // found hole? give up search
    if (data_at_index == EMPTY) return;

    // too far? give up search
    size_t data_at_index_distance = index_distance(index, home_at_index);
    size_t data_distance          = index_distance(index, home);
    if (data_at_index_distance < data_distance) return;

    index = this->next_index(index);
  }
  // found element
  size_t shift_start = index;

  // search for end of elements to shift
  index = this->next_index(index);
  while(true) {
    std::tie(data_at_index, home_at_index) = this->buckets[index];

    // found hole? end here
    if (data_at_index == EMPTY) break;

    // element in its place
    if (!index_distance(index, home_at_index)) break;

    index = this->next_index(index);
  }

  // shift everything left by one
  size_t shift_end = previous_index(index);
  size_t distance = index_distance(shift_end, shift_start);
  index = shift_start;
  for (size_t count = 0; count < distance; count++) {
    size_t next_index = this->next_index(index);
    this->buckets[index] = this->buckets[next_index];
    index = next_index;
  }

  this->buckets[shift_end] = std::pair<int, size_t>(EMPTY, 0);

}

/* Helper */

inline size_t RobinHoodHashTable::previous_index(size_t index) const
{
  if (index == 0) {
    return this->buckets.size() - 1;
  } else {
    return index - 1;
  }
}

inline size_t RobinHoodHashTable::next_index(size_t index) const
{
  return ++index % this->buckets.size();
}

inline size_t RobinHoodHashTable::index_for_data(int data) const
{
  size_t hash_value = this->hashFunction(data);
  size_t index = hash_value % this->buckets.size();
  return index;
}

size_t RobinHoodHashTable::index_distance(size_t index1, size_t index2) const
{
  return (index1 - index2) % this->buckets.size();
}
