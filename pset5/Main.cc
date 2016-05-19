#include <iostream>

#include "Hashes.h"
#include "ChainedHashTable.h"
#include "SecondChoiceHashTable.h"
#include "LinearProbingHashTable.h"
#include "RobinHoodHashTable.h"
#include "CuckooHashTable.h"
#include "Timing.h"

int main() {
  /* A list of all true families of hash functions (that is, hash families
   * where we can sample as many hash functions as we need.) These hash
   * families can be used in any hash table type.
   */
  auto allHashFamilies  = {
    twoIndependentHashFamily(),
    threeIndependentHashFamily(),
    fiveIndependentHashFamily(),
    tabulationHashFamily()
  };
  
  /* A list of all types of hash functions available, including families with
   * just a single hash function in them. These are suitable for everything
   * except second-choice hashing and cuckoo hashing.
   */
  auto allHashFunctions = {
    twoIndependentHashFamily(),
    threeIndependentHashFamily(),
    fiveIndependentHashFamily(),
    tabulationHashFamily(),
    identityHash(),
    jenkinsHash()
  };

  std::cout << "Correctness Tests" << std::endl;
  std::cout << "  Chained:        " << (checkCorrectness<ChainedHashTable>(allHashFamilies) ? "pass" : "fail") << std::endl;
  std::cout << "  Second-Choice:  " << (checkCorrectness<SecondChoiceHashTable>(allHashFamilies) ? "pass" : "fail") << std::endl;
  std::cout << "  Linear Probing: " << (checkCorrectness<LinearProbingHashTable>(allHashFamilies) ? "pass" : "fail") << std::endl;
  std::cout << "  Robin Hood:     " << (checkCorrectness<RobinHoodHashTable>(allHashFamilies) ? "pass" : "fail") << std::endl;
  std::cout << "  Cuckoo:         " << (checkCorrectness<CuckooHashTable>(allHashFamilies) ? "pass" : "fail") << std::endl;
  std::cout << std::endl;

  /* Test linear probing variants. */
  auto probingLoadFactors = {0.3, 0.5, 0.7, 0.9, 0.99};

  std::cout << "#### Timing Linear Probing ####" << std::endl;
  doAllReports<LinearProbingHashTable>(allHashFunctions, probingLoadFactors);
  std::cout << "###########################" << std::endl;
  std::cout << std::endl;
  
  std::cout << "#### Timing Robin Hood ####" << std::endl;
  doAllReports<RobinHoodHashTable>(allHashFunctions, probingLoadFactors);
  std::cout << "###########################" << std::endl;
  std::cout << std::endl;
  
  
  /* Test chained hashing variants. */
  auto chainedLoadFactors = {0.3, 0.5, 0.7, 0.9, 0.99, 2.0, 5.00};
  
  std::cout << "#### Timing Chained ####" << std::endl;
  doAllReports<ChainedHashTable>(allHashFunctions, chainedLoadFactors);
  std::cout << "###########################" << std::endl;
  std::cout << std::endl;
  
  std::cout << "#### Timing Second-Choice ####" << std::endl;
  doAllReports<SecondChoiceHashTable>(allHashFamilies, chainedLoadFactors);
  std::cout << "###########################" << std::endl;
  std::cout << std::endl;
  
  /* Test cuckoo hashing. */
  auto cuckooLoadFactors = {0.2, 0.3, 0.4, 0.45, 0.47};

  std::cout << "#### Timing Cuckoo Hashing ####" << std::endl;
  doAllReports<CuckooHashTable>(allHashFamilies, cuckooLoadFactors);
  std::cout << "###########################" << std::endl;
  std::cout << std::endl;
}
