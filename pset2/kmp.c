#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "kmp.h"

struct matcher {
    int pattern_length;
    char* pattern;
	int* suffix_links;
};

struct matcher* create_matcher_for(const char* pattern) {
    struct matcher* aMatcher = (struct matcher*) malloc( sizeof(struct matcher) );

    // copy string
    aMatcher->pattern_length = strlen(pattern);
    aMatcher->pattern = (char*) malloc(aMatcher->pattern_length * sizeof(char));
    strcpy(aMatcher->pattern, pattern);

    // create suffix_link array
    aMatcher->suffix_links = (int*) malloc(aMatcher->pattern_length * sizeof(int));

    // compute suffix_length_array
    printf("create_matcher_for");
    return aMatcher;
}

void destroy_matcher(struct matcher* matcher) {
    free(matcher->pattern);
    free(matcher->suffix_links);
    free(matcher);
    printf("destroy_matcher");
    return;
}

void for_each_match(struct matcher* matcher,
		    const char* text,
		    void (*callback)(const char* match, void* aux),
		    void* aux) {
  /* TODO: Fill this in! */
}
