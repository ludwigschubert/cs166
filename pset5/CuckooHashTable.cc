#include "CuckooHashTable.h"

constexpr size_t CuckooHashTable::log2(size_t n) const
{
  return (n > 1) ? 1 + log2(n >> 1) : 0;
}

CuckooHashTable::CuckooHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family)
{
  this->hash_family = family;
  init(numBuckets / 2);

}

void CuckooHashTable::init(int number_of_buckets)
{
  this->number_of_buckets = number_of_buckets;
  this->buckets_left  = std::vector<std::pair<int, size_t>>(this->number_of_buckets);
  this->buckets_right = std::vector<std::pair<int, size_t>>(this->number_of_buckets);

  this->hash_function_left  = this->hash_family->get();
  this->hash_function_right = this->hash_family->get();

  this->insert_in_left = false;
  this->number_of_elements = 0;
  this->rehash_threshold = 5;
}

CuckooHashTable::~CuckooHashTable()
{
  // TODO: Implement this
}

void CuckooHashTable::insert(int data)
{
  this->insert_in(std::pair<int, size_t>(data, 0));
}

bool CuckooHashTable::insert_in(std::pair<int, size_t> data)
{
  if (this->contains(data.first)) return true;

  this->insert_in_left = !this->insert_in_left;
  if (data.second > this->rehash_threshold) {
    if (this->is_rehashing) {
      return false; // abort and start new rehashing
    } else {
      rehash();
    }
  }

  std::pair<size_t, size_t> indices = indices_for_data(data.first);
  if (this->insert_in_left) {

    size_t index = indices.first;
    if (! this->buckets_left[index].first) {
      this->buckets_left[index] = data;
    } else { // displace
      auto displaced_data = this->buckets_left[index];
      this->buckets_left[index] = data;
      displaced_data.second++;
      insert_in(displaced_data);
    }

  } else {

    size_t index = indices.second;
    if (! this->buckets_right[index].first) {
      this->buckets_right[index] = data;
    } else { // displace
      auto displaced_data = this->buckets_right[index];
      this->buckets_right[index] = data;
      displaced_data.second++;
      insert_in(displaced_data);
    }

  }
  this->number_of_elements++;
  this->rehash_threshold = 6 * log2(number_of_elements);
  return true; // success
}

bool CuckooHashTable::contains(int data) const
{
  size_t index_left, index_right;
  std::tie(index_left, index_right) = indices_for_data(data);
  return this->buckets_left[index_left].first == data ||
         this->buckets_right[index_right].first == data;
}

void CuckooHashTable::remove(int data)
{
  size_t index_left, index_right;
  std::tie(index_left, index_right) = indices_for_data(data);
  if (this->buckets_left[index_left].first == data) {
    this->buckets_left[index_left] = std::pair<int, size_t>();
    this->number_of_elements--;
  } else if (this->buckets_right[index_right].first == data) {
    this->buckets_right[index_right] = std::pair<int, size_t>();
    this->number_of_elements--;
  }

  this->rehash_threshold = 6 * log2(number_of_elements);
}

void CuckooHashTable::rehash()
{
  this->is_rehashing = true;
  bool success = false;

  while (!success) {
    std::vector<std::pair<int, size_t>> old_left = this->buckets_left;
    std::vector<std::pair<int, size_t>> old_right = this->buckets_right;

    init(this->number_of_buckets);

    success = true;

    for (auto it : old_left) {
      int data = it.first;
      success &= insert_in(std::pair<int, size_t>(data, 0));
      if (!success) break;
    }

    for (auto it : old_right) {
      int data = it.first;
      success &= insert_in(std::pair<int, size_t>(data, 0));
      if (!success) break;
    }
  }

}

inline std::pair<size_t, size_t> CuckooHashTable::indices_for_data(int data) const
{
  size_t hash_value_left = this->hash_function_left(data);
  size_t index_left = hash_value_left % this->number_of_buckets;

  size_t hash_value_right = this->hash_function_right(data);
  size_t index_right = hash_value_right % this->number_of_buckets;

  return std::pair<size_t, size_t>(index_left, index_right);
}
