#include "SecondChoiceHashTable.h"

SecondChoiceHashTable::SecondChoiceHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->hashFunction1 = family->get();
  this->hashFunction2 = family->get();
  this->buckets = std::vector<std::vector<int>>(numBuckets, std::vector<int>(0));
}

SecondChoiceHashTable::~SecondChoiceHashTable() {
  // TODO: Implement this
}

void SecondChoiceHashTable::insert(int data) {
  auto indices = this->indices_for_data(data);
  size_t index1 = indices.first;
  size_t index2 = indices.second;

  auto bucket1 = this->buckets.at(index1);
  auto iterator1 = std::find(bucket1.begin(), bucket1.end(), data);

  auto bucket2 = this->buckets.at(index2);
  auto iterator2 = std::find(bucket2.begin(), bucket2.end(), data);

  if (iterator1 == bucket1.end()) { // did not find element in bucket 1
    if (iterator2 == bucket2.end()) { // did not find element in bucket 2, inserting
      if (bucket1.size() < bucket2.size()) {
        bucket1.push_back(data);
        this->buckets.at(index1) = bucket1;
      } else {
        bucket2.push_back(data);
        this->buckets.at(index2) = bucket2;
      }
    }
  }
}

bool SecondChoiceHashTable::contains(int data) const {
  auto indices = this->indices_for_data(data);
  size_t index1 = indices.first;
  size_t index2 = indices.second;

  auto bucket1 = this->buckets.at(index1);
  auto iterator1 = std::find(bucket1.begin(), bucket1.end(), data);

  auto bucket2 = this->buckets.at(index2);
  auto iterator2 = std::find(bucket2.begin(), bucket2.end(), data);

  bool bucket1_contains = iterator1 != bucket1.end();
  bool bucket2_contains = iterator2 != bucket2.end();
  bool contains_data = bucket1_contains || bucket2_contains;

  return contains_data;
}

void SecondChoiceHashTable::remove(int data) {
  auto indices = this->indices_for_data(data);
  size_t index1 = indices.first;
  size_t index2 = indices.second;

  auto bucket1 = this->buckets.at(index1);
  auto iterator1 = std::find(bucket1.begin(), bucket1.end(), data);

  auto bucket2 = this->buckets.at(index2);
  auto iterator2 = std::find(bucket2.begin(), bucket2.end(), data);

  if (iterator1 != bucket1.end()) { // found element in bucket 1
    bucket1.erase(iterator1);
    this->buckets.at(index1) = bucket1;
  }

  if (iterator2 != bucket2.end()) { // found element in bucket 2, inserting
    bucket2.erase(iterator2);
    this->buckets.at(index2) = bucket2;
  }
}

std::pair<size_t, size_t> SecondChoiceHashTable::indices_for_data(int data) const {
  size_t hash_value1 = this->hashFunction1(data);
  size_t hash_value2 = this->hashFunction2(data);
  size_t index1 = hash_value1 % this->buckets.size();
  size_t index2 = hash_value2 % this->buckets.size();
  return std::pair<size_t, size_t>(index1, index2);
}