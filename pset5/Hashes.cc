#include "Hashes.h"
#include <random>
#include <array>

static std::default_random_engine engine(137);

static const size_t kLargePrime = (1u << 31) - 1;

static size_t randomFieldElem() {
  std::uniform_int_distribution<size_t> dist(0, kLargePrime - 1);
  return dist(engine);
}

static size_t random32Bits() {
  std::uniform_int_distribution<size_t> dist;
  return dist(engine);
}


std::shared_ptr<HashFamily> twoIndependentHashFamily() {
  class TwoIndependentHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      size_t a = randomFieldElem();
      size_t b = randomFieldElem();
      
      return [a, b] (int key) {
        return (a * key + b) % kLargePrime;
      };
    }
    
    virtual std::string name() const {
      return "2-Independent Polynomial Hash";
    }
  };
  
  return std::make_shared<TwoIndependentHashFamily>();
}

std::shared_ptr<HashFamily> threeIndependentHashFamily() {
  class ThreeIndependentHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      size_t a = randomFieldElem();
      size_t b = randomFieldElem();
      size_t c = randomFieldElem();
      
      return [a, b, c] (int key) {
        return (a * key * key + b * key + c) % kLargePrime;
      };
    }
    
    virtual std::string name() const {
      return "3-Independent Polynomial Hash";
    }
  };
  
  return std::make_shared<ThreeIndependentHashFamily>();
}

std::shared_ptr<HashFamily> fiveIndependentHashFamily() {
  class FiveIndependentHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      size_t a = randomFieldElem();
      size_t b = randomFieldElem();
      size_t c = randomFieldElem();
      size_t d = randomFieldElem();
      size_t e = randomFieldElem();
      
      return [a, b, c, d, e] (int key) {
        return (a * key * key * key * key + b * key * key * key + 
                c * key * key + d * key + e) % kLargePrime;
      };
    }
    
    virtual std::string name() const {
      return "5-Independent Polynomial Hash";
    }
  };
  
  return std::make_shared<FiveIndependentHashFamily>();
}

std::shared_ptr<HashFamily> tabulationHashFamily() {
  class TabulationHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      std::array<std::array<size_t, 256>, 4> table;
      for (size_t i = 0; i < 4; i++) {
        for (size_t byte = 0; byte < 256; byte++) {
          table[i][byte] = random32Bits();
        }
      }
      
      return [table] (int key) {
        size_t result = 0;
        for (size_t i = 0; i < 4; i++) {
          result ^= table[i][(key & (0xFF << (i * 8))) >> (i * 8)];
        }
        return result % kLargePrime;
      };
    }
    
    virtual std::string name() const {
      return "3-Independent Tabulation Hash";
    }
  };
  
  return std::make_shared<TabulationHashFamily>();
}

std::shared_ptr<HashFamily> identityHash() {
  class IdentityHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      return [] (int key) {
        return key;
      };
    }
    
    virtual std::string name() const {
      return "Identity Hash";
    }
  };
  
  return std::make_shared<IdentityHashFamily>();
}

std::shared_ptr<HashFamily> jenkinsHash() {
  class JenkinsHashFamily: public HashFamily {
  public:
    virtual HashFunction get() const {
      return [] (int a) {
        a = (a + 0x7ed55d16) + (a << 12);
        a = (a ^ 0xc761c23c) ^ (a >> 19);
        a = (a + 0x165667b1) + (a << 5);
        a = (a + 0xd3a2646c) ^ (a << 9);
        a = (a + 0xfd7046c5) + (a << 3);
        a = (a ^ 0xb55a4f09) ^ (a >> 16);
        return a;
      };
    }
    
    virtual std::string name() const {
      return "Jenkins Hash";
    }
  };
  
  return std::make_shared<JenkinsHashFamily>();
}
