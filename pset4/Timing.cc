#include "Timing.h"
#include <algorithm>

/**
 * Returns a random number generator that generates data according to a Zipfian
 * distribution, a type of distribution that occurs frequently in real-world
 * data, especially exst. See https://en.wikipedia.org/wiki/Zipf%27s_law.
 * Higher z values give more uneven data. When z is 0, the distribution is uniform.
 */
std::discrete_distribution<int> zipfian(size_t count, double z) {
  std::vector<double> weights(count);
  for (uint i = 0; i < count; i++) {
    weights[i] = 1 / pow(i + 1, z);
  }
  
  /* Permute the elements. This makes it unlikely that the elements that will
   * be looked up will be anywhere near one another.
   */
  std::random_shuffle(weights.begin(), weights.end());
  return std::discrete_distribution<int>(weights.begin(), weights.end());
}
