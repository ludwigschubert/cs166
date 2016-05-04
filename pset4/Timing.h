/**
 * Header file exporting timing and testing functions. Because of C++ template
 * linker issues, all template functions are implemented in this header file.
 */
#ifndef Timing_Included
#define Timing_Included

#include <chrono>
#include <memory>
#include <random>
#include <vector>
#include <cmath>
#include <stddef.h>

/* The random seed used throughout the run. */
static const size_t kRandomSeed = 137;

/**
 * Returns a random number generator that generates data according to a Zipfian
 * distribution, a type of distribution that occurs frequently in real-world
 * data, especially exst. See https://en.wikipedia.org/wiki/Zipf%27s_law.
 * Higher z values give more uneven data. When z is 0, the distribution is uniform.
 */
std::discrete_distribution<int> zipfian(size_t count, double z);

/**
 * Given a probability distribution and a list of the underlying probabilities,
 * runs a time trial to determine how quickly the indicated number of lookups
 * can be performed on a BST of the specified type. The number of elements in
 * the underlying tree will be equal to the number of distinct probabilities,
 * and the assumption is that the elements will be 0, 1, 2, ...,
 * probabilities.size() - 1.
 */
template <typename BST, typename ProbabilityDistribution>
double timeDistribution(ProbabilityDistribution& gen,
                        const std::vector<double>& probabilities,
                        size_t numLookups) {
  std::default_random_engine engine;
  engine.seed(kRandomSeed);

  BST tree{probabilities};

  std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();

  for (size_t i = 0; i < numLookups; i++) {
    auto index = gen(engine);
    auto start = std::chrono::high_resolution_clock::now();
    tree.contains(index);
    auto end = std::chrono::high_resolution_clock::now();
    total += end - start;
  }

  return std::chrono::duration_cast<std::chrono::nanoseconds>(total).count() / 1.0e6;
}

/**
 * Given a discrete distribution, times the performance of performing BST
 * lookups on that distribution.
 */
template <typename BST>
double timeDistribution(std::discrete_distribution<int>& gen, size_t numLookups) {
  auto probabilities = gen.probabilities();
  return timeDistribution<BST>(gen, probabilities, numLookups);
}

/**
 * Given a uniform distribution, times the performance of performing BST
 * lookups on that distribution.
 */
template <typename BST>
double timeDistribution(std::uniform_int_distribution<int>& gen,
                        size_t numLookups) {
  auto probabilities = std::vector<double>(gen.max() + 1, 1.0 / (1 + gen.max()));
  return timeDistribution<BST>(gen, probabilities, numLookups);
}


/**
 * Given a BST type and a number of elements, reports the time required to
 * visit every element of that BST in sequence, either front-to-back or
 * back-to-front.
 */
template <typename BST>
double timeSequential(size_t count, int start, int step) {
  std::vector<double> probabilities = std::vector<double>(count, 1.0 / count);

  BST tree{probabilities};

  std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();

  for (int i = start; i >= 0 && i < int(count); i += step) {
    auto start = std::chrono::high_resolution_clock::now();
    tree.contains(i);
    auto end = std::chrono::high_resolution_clock::now();
    total += end - start;
  }

  return std::chrono::duration_cast<std::chrono::nanoseconds>(total).count() / 1.0e6;
}

/**
 * Given a BST type and a number of elements, reports the time required to
 * visit every element of that BST in order.
 */
template <typename BST>
double timeSequential(size_t count) {
  return timeSequential<BST>(count, 0, 1);
}


/**
 * Given a BST type and a number of elements, reports the time required to
 * visit every element of that BST in reverse order.
 */
template <typename BST>
double timeReverseSequential(size_t count) {
   return timeSequential<BST>(count, count - 1, -1);
}


/**
 * Given a BST type, a number of elements, a number of working sets, and a
 * number of lookups, times the performance of the BST when doing a large
 * number of lookups that are clustered on a small number of elements. Each
 * group of elements that's repeatedly queried is called a "working set"
 * and it's known that splay trees perform particularly well on this case.
 */
template <typename BST>
double timeWorkingSets(size_t numElems, size_t numSets, size_t numLookups) {
  std::default_random_engine engine;
  engine.seed(kRandomSeed);
  auto gen = std::uniform_int_distribution<int>(0, numElems / numSets);

  std::vector<double> probabilities(numElems, 1.0 / numElems);

  BST tree{probabilities};

  std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();

  for (size_t i = 0; i < numLookups; i++) {
        size_t blockId = (double(i) / numLookups) * numSets;
    int index = gen(engine) + blockId * numElems / numSets;
    
    auto start = std::chrono::high_resolution_clock::now();
    tree.contains(index);
    auto end = std::chrono::high_resolution_clock::now();
    total += end - start;
  }

  return std::chrono::duration_cast<std::chrono::nanoseconds>(total).count() / 1.0e6;
}


/**
 * Runs some basic correctness checks to ensure that the tree works correctly.
 * This involves looking up all the expected elements and a few that aren't
 * there.
 */
template <typename BST>
bool checkCorrectness(size_t count, size_t lookups) {
  std::default_random_engine engine;
  engine.seed(kRandomSeed);
  auto gen = std::uniform_int_distribution<int>(0, count - 1);
  auto probabilities = std::vector<double>(gen.max() + 1, 1.0 / (1 + gen.max()));

  BST tree{probabilities};

  bool passed = true;
  
  /* Generate a bunch of random in-range keys and confirm that we can find all
   * of them.
   */
  for (size_t i = 0; i < lookups; i++) {
    passed = passed && tree.contains(gen(engine));
  }

        /* Check out of range to make sure we don't find anything. */
  passed = passed && !tree.contains(-1);
  passed = passed && !tree.contains(count);
  passed = passed && !tree.contains(count+1);

  /* Do one final pass checking that all the values in range are indeed present
   * that we expect to be present.
   */
  for (size_t i = 0; i < count; i++) {
    passed = passed && tree.contains(i);
  }
  return passed;
}

#endif
