#include "fenwick-tree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/*
 * "private" function declarations
 */

void recursive_increment(struct fenwick_tree* tree, size_t index, int amount, size_t node, bool went_left);
int recursive_cumulative_frequency(const struct fenwick_tree* tree, size_t index, size_t node, bool went_left, int current_total);

size_t child(const struct fenwick_tree *tree, size_t parent, bool left);
bool child_exists(const struct fenwick_tree* tree, size_t parent, bool left);

struct fenwick_tree {
    size_t size;
    size_t root_node;
	int *histogram;
    int *sub_tree_sums;
};

struct fenwick_tree* create_tree_of_size(size_t size) {

    // Allocate struct and internal arrays
    struct fenwick_tree* tree = (struct fenwick_tree*) malloc(sizeof(struct fenwick_tree));
    tree->histogram     = (int*) malloc( (size + 1) * sizeof(int));
    tree->sub_tree_sums = (int*) malloc( (size + 1) * sizeof(int));

    // Find root node; the largest power of two within our tree
    size_t root_node = 1;
    while (root_node <= size) {
        root_node *= 2;
    }
    tree->root_node = root_node / 2;

    // Initialize arrays
    tree->size = size;
    tree->histogram[0] = -42;
    tree->sub_tree_sums[0] = -42;
    for (size_t i = 1; i <= size; ++i) {
        tree->histogram[i]     = 0;
        tree->sub_tree_sums[i] = 0;
    }

    return tree;
}

void destroy_tree(struct fenwick_tree* tree) {
    free(tree->sub_tree_sums);
	free(tree->histogram);
    free(tree);
}

void increment(struct fenwick_tree* tree, size_t index, int amount) {
    assert(0 < index && index <= tree->size);
    recursive_increment(tree, index, amount, tree->root_node, false);
}

void recursive_increment(struct fenwick_tree* tree, size_t index, int amount, size_t node, bool went_left) {
    if (node == index) { // "found" node; increment histogram
        tree->histogram[node]     += amount;
        tree->sub_tree_sums[node] += amount;
    } else { // continue descending
        bool go_left = index < node;
        recursive_increment(tree, index, amount, child(tree, node, go_left), go_left);
        tree->sub_tree_sums[node] += amount;
    }

}

int cumulative_frequency(const struct fenwick_tree* tree, size_t index) {
    int recursive_result = recursive_cumulative_frequency(tree, index, tree->root_node, false, 0);
    return recursive_result;
}

int recursive_cumulative_frequency(const struct fenwick_tree* tree, size_t index, size_t node, bool went_left, int current_total) {
    if (node == index) { // "found" node
        int local_total = current_total;
        local_total += tree->histogram[node];
        if (child_exists(tree, node, true)) {
            local_total += tree->sub_tree_sums[child(tree, node, true)];
        }
        return local_total;
    } else { // continue descending
        bool go_left = index < node;
        if (go_left) {
            return recursive_cumulative_frequency(tree, index, child(tree, node, go_left), go_left, current_total);
        } else {
            int local_total = current_total;
            local_total += tree->histogram[node];
            if (child_exists(tree, node, true)) {
                local_total += tree->sub_tree_sums[child(tree, node, true)];
            }
            return recursive_cumulative_frequency(tree, index, child(tree, node, go_left), go_left, local_total);
        }
    }
}


/*
 * Index Arithmetic :-)
 */

bool child_exists(const struct fenwick_tree* tree, size_t parent, bool left) {
    size_t child_index = child(tree, parent, true);
    bool not_parent = child_index != parent;
    bool not_negative = child_index > 0; // we ignore the 0'th index
    return not_parent && not_negative;
}

size_t child(const struct fenwick_tree *tree, size_t parent, bool left) {
    size_t two_to_node_level = (parent & -parent) / 2; // magic; via pset

    size_t potential_child_node;
    if (left) {
        potential_child_node = parent - two_to_node_level;
    } else {
        potential_child_node = parent + two_to_node_level;;
        while(potential_child_node > tree->size){
            two_to_node_level /= 2;
            potential_child_node = parent + two_to_node_level;
        }
    }
    assert(1 <= potential_child_node && potential_child_node <= tree->size);
    return potential_child_node;
}
