#include "fenwick-tree.h"
#include <stddef.h>
#include <stdbool.h>

struct fenwick_tree {
	/* TODO: Fill this in! */
};

struct fenwick_tree* create_tree_of_size(size_t size) {
	/* TODO: Fill this in! */
}

void destroy_tree(struct fenwick_tree* tree) {
	/* TODO: Fill this in! */
}

void increment(struct fenwick_tree* tree, size_t index, int amount) {
	/* TODO: Fill this in! */
}

int cumulative_frequency(const struct fenwick_tree* tree, size_t index) {
	/* TODO: Fill this in! */
    return 42;
}
