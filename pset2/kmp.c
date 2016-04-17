#include "kmp.h"

struct matcher {
  /* TODO: Fill this in! */
};

struct matcher* create_matcher_for(const char* pattern) {
  /* TODO: Fill this in! */
}

void destroy_matcher(struct matcher* matcher) {
  /* TODO: Fill this in! */
}

void for_each_match(struct matcher* matcher,
		    const char* text,
		    void (*callback)(const char* match, void* aux),
		    void* aux) {
  /* TODO: Fill this in! */
}
