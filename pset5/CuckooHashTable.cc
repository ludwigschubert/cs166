#include "CuckooHashTable.h"

constexpr size_t CuckooHashTable::log2(size_t n)
{
  return (n > 1) ? 1 + log2(n >> 1) : 0;
}

CuckooHashTable::CuckooHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family)
{
  this->hash_family = family;

  size_t number_of_buckets = numBuckets / 2;
  this->buckets_left  = std::vector<std::pair<int, size_t>>(number_of_buckets);
  this->buckets_right = std::vector<std::pair<int, size_t>>(number_of_buckets);

  this->hash_function_left  = this->hash_family->get();
  this->hash_function_right = this->hash_family->get();

  this->insert_in_left = true;
  this->number_of_elements = 0;
  this->rehash_threshold = 18;
}

CuckooHashTable::~CuckooHashTable()
{
  // TODO: Implement this
}

void CuckooHashTable::insert(int data)
{
//  int displacement_count = 6 * log2()
  this->insert_in_left = !this->insert_in_left;
  this->insert_in(std::pair<int, size_t>(data, 0));
  this->number_of_elements++;
//  this->rehash_threshold = 6 * log2(number_of_elements);
}

void CuckooHashTable::insert_in(std::pair<int, size_t> data)
{
  if (data.second > this->rehash_threshold) rehash();

  std::pair<size_t, size_t> indices = indices_for_data(data.first);
  if (this->insert_in_left) {

    size_t index = indices.first;
    if (this->buckets_left[index].first == data.first) return; // found!
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
    if (this->buckets_right[index].first == data.first) return; // found!
    if (! this->buckets_right[index].first) {
      this->buckets_right[index] = data;
    } else { // displace
      auto displaced_data = this->buckets_right[index];
      this->buckets_right[index] = data;
      displaced_data.second++;
      insert_in(displaced_data);
    }

  }
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

//  this->rehash_threshold = 6 * log2(number_of_elements);
}

void CuckooHashTable::rehash()
{
  // simple way first
  std::vector<std::pair<int, size_t>> old_left  = this->buckets_left;
  std::vector<std::pair<int, size_t>> old_right = this->buckets_right;
}

inline std::pair<size_t, size_t> CuckooHashTable::indices_for_data(int data) const
{
  size_t hash_value_left = this->hash_function_left(data);
  size_t index_left = hash_value_left % this->buckets_left.size();

  size_t hash_value_right = this->hash_function_right(data);
  size_t index_right = hash_value_right % this->buckets_right.size();

  return std::pair<size_t, size_t>(index_left, index_right);
}
