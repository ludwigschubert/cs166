#include "fenwick-tree.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * "private" function declarations
 */

void recursive_increment(struct fenwick_tree* tree, size_t index, int amount, size_t node, bool went_left);
int recursive_cumulative_frequency(const struct fenwick_tree* tree, size_t index, size_t node, bool went_left, int current_total);

size_t child(size_t parent, bool left);
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
    tree->histogram     = (int*) malloc((size + 1)* sizeof(int));
    tree->sub_tree_sums = (int*) malloc((size + 1)* sizeof(int));

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
    recursive_increment(tree, index, amount, tree->root_node, false);
}

void recursive_increment(struct fenwick_tree* tree, size_t index, int amount, size_t node, bool went_left) {
    if (node == index) { // "found" node; increment histogram
        tree->histogram[node]     += amount;
        tree->sub_tree_sums[node] += amount;
    } else { // continue descending
        bool go_left = index < node;
        recursive_increment(tree, index, amount, child(node, go_left), go_left);
        tree->sub_tree_sums[node] += amount;
    }

}

int cumulative_frequency(const struct fenwick_tree* tree, size_t index) {
    int recursive_result = recursive_cumulative_frequency(tree, index, tree->root_node, false, 0);
    if (recursive_result != naive_cumulative_frequency(tree, index)) {
        ;
    }
    return recursive_result;
}

int recursive_cumulative_frequency(const struct fenwick_tree* tree, size_t index, size_t node, bool went_left, int current_total) {
    if (node == index) { // "found" node
        int local_total = current_total;
        local_total += tree->histogram[node];
        if (child_exists(tree, node, true)) {
            local_total += tree->sub_tree_sums[child(node, true)];
        }
        return local_total;
    } else { // continue descending
        bool go_left = index < node;
        if (go_left) {
            return recursive_cumulative_frequency(tree, index, child(node, go_left), go_left, current_total);
        } else {
            int local_total = current_total;
            local_total += tree->histogram[node];
            if (child_exists(tree, node, true)) {
                local_total += tree->sub_tree_sums[child(node, true)];
            }
            return recursive_cumulative_frequency(tree, index, child(node, go_left), go_left, local_total);
        }
    }
}


/*
 * Index Arithmetic :-)
 */

bool child_exists(const struct fenwick_tree* tree, size_t parent, bool left) {
    size_t child_index = child(parent, true);
    bool valid_index = 1 <= child_index <= tree->size;
    bool not_parent = child_index != parent;
    return valid_index && not_parent;
}

size_t child(size_t parent, bool left){
    size_t two_to_node_level = (parent & -parent) / 2; // magic; via pset
    if (left) {
        return parent - two_to_node_level;
    } else {
        return parent + two_to_node_level;
    }
}

//size_t left_child(size_t parent){
//    size_t two_to_node_level = parent & -parent; // magic; via pset
//    return parent - two_to_node_level;
//}
//
//size_t right_child(size_t parent){
//    size_t two_to_node_level = parent & -parent; // magic; via pset
//    return parent + two_to_node_level;
//}

/*
 * O(n) naive implementations for checking
 */

void naive_increment(struct fenwick_tree* tree, size_t index, int amount) {
    tree->histogram[index] += amount;
}

int naive_cumulative_frequency(const struct fenwick_tree* tree, size_t index) {
    int cumulative_frequency = 0;
    for (int i = 1; i <= index; ++i) {
        cumulative_frequency += tree->histogram[i];
    }
    return cumulative_frequency;
}
