#include <assert.h>
#include "ChainedHashTable.h"
#include <iostream>


ChainedHashTable::ChainedHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->hashFunction = family->get();
//  this->numBuckets = numBuckets;
  this->buckets = std::vector<std::forward_list<int>>(numBuckets, std::forward_list<int>(0));
}

ChainedHashTable::~ChainedHashTable() {
  // should not need to do anything yay pointers
}

void ChainedHashTable::insert(int data) {
  size_t index = this->index_for_data(data);
  auto chain = this->buckets.at(index);
  auto iterator = std::find(chain.begin(), chain.end(), data);
  if (iterator == chain.end()) { // did not find element, inserting
    chain.push_front(data);
    this->buckets.at(index) = chain;
  }
}

bool ChainedHashTable::contains(int data) const {
  size_t index = this->index_for_data(data);
  auto chain = this->buckets.at(index);
  auto iterator = std::find(chain.begin(), chain.end(), data);
  bool contains_data = iterator != chain.end();
  return contains_data;
}

void ChainedHashTable::remove(int data) {
  size_t index = this->index_for_data(data);
  auto chain = this->buckets.at(index);
  chain.remove(data);
  this->buckets.at(index) = chain;
}

size_t ChainedHashTable::index_for_data(int data) const {
  size_t hash_value = this->hashFunction(data);
  size_t index = hash_value % this->buckets.size();
  return index;
}