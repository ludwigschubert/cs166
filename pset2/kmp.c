#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "kmp.h"

struct matcher {
    size_t pattern_length; // strlen(pattern)
    char* pattern;      // pattern to match
	int* suffix_links;  // length: pattern_length + 1
};

struct matcher* create_matcher_for(const char* pattern) {
    struct matcher* aMatcher = (struct matcher*) malloc( sizeof(struct matcher) );

    // copy string
    aMatcher->pattern_length = strlen(pattern);
    aMatcher->pattern = (char*) malloc(aMatcher->pattern_length * sizeof(char));
    strcpy(aMatcher->pattern, pattern);

    // create suffix_link array
    aMatcher->suffix_links = (int*) malloc((aMatcher->pattern_length + 1) * sizeof(int));

    // compute suffix_length_array

    aMatcher->suffix_links[0] = 0; // root 'points' to 'itself'
    aMatcher->suffix_links[1] = 0; // first node 'points' to 'root'
    for (int i = 2; i < aMatcher->pattern_length + 1; ++i) {

        int currentNode = i;
        int previousNode = i - 1;
        char currentCharacter = aMatcher->pattern[currentNode-1];

        char characterAfterLinkedNode;
        while (true) {
            previousNode = aMatcher->suffix_links[previousNode];
            characterAfterLinkedNode = aMatcher->pattern[previousNode];

            if (characterAfterLinkedNode == currentCharacter) {
                aMatcher->suffix_links[currentNode] = previousNode + 1;
                break;
            }

            if (previousNode == 0) {
                aMatcher->suffix_links[currentNode] = previousNode;
                break;
            }
        }

    }

    return aMatcher;
}

void destroy_matcher(struct matcher* matcher) {
    free(matcher->pattern);
    free(matcher->suffix_links);
    free(matcher);
    return;
}

void for_each_match(struct matcher* matcher,
		    const char* text,
		    void (*callback)(const char* match, void* aux),
		    void* aux) {
    // Start at the root node in the trie
    int current_node = 0;
    // For each character in the string
    for (int i = 0; i < strlen(text); ++i) {
        // While there is no edge labeled with that character
        while (text[i] != matcher->pattern[current_node]) {
            if (current_node == 0) break; // break out of this loop, or
            current_node = matcher->suffix_links[current_node]; // follow a suffix link.
        }
        // If there is an edge labeled with that character, follow it
        if (text[i] == matcher->pattern[current_node]) {
            current_node++;
        }
        // If the current node corresponds to a match, report it
        if (current_node == matcher->pattern_length) {
            callback(text + i - (matcher->pattern_length - 1), aux);
        }
    }
}
