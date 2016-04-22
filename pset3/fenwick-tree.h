#ifndef FenwickTree_Included
#define FenwickTree_Included

#include <stddef.h>

/* Type: fenwick_tree
 * ---------------------------------------------------------------------------
 * An opaque type representing a Fenwick tree. You will define this type in
 * fenwick-tree.c.
 */
struct fenwick_tree;

/* Function: create_tree_of_size(size_t size);
 * Usage: struct fenwick_tree* tree = create_tree_of_size(137);
 * ---------------------------------------------------------------------------
 * Creates and returns a new Fenwick tree of that given size. You do not need
 * to worry about the case where there is insufficient memory to allocate the
 * structure. The frequency of each element should initially be zero.
 */
struct fenwick_tree* create_tree_of_size(size_t size);

/* Function: destroy_tree(struct fenwick_tree* tree);
 * ---------------------------------------------------------------------------
 * Disposes of all memory associated with the given Fenwick tree.
 */
void destroy_tree(struct fenwick_tree* tree);

/* Function: increment(struct fenwick_tree* tree, size_t index, int amount);
 * Usage: increment(tree, 100, -3);
 * ---------------------------------------------------------------------------
 * Given a Fenwick tree and an index in that tree, increments the specified
 * index by the specified amount. Remember that Fenwick trees are 1-indexed!
 * You do not need to handle the case where the index is out of bounds.
 */
void increment(struct fenwick_tree* tree, size_t index, int amount);

/* Function: cumulative_frequency(const struct fenwick_tree* tree, size_t index);
 * Usage: printf("%d\n", cumulative_frequency(tree, 100));
 * ---------------------------------------------------------------------------
 * Given a Fenwick tree and an index in that tree, returns the cumulative
 * frequency of the elements between indices 1 and the specified index,
 * inclusive. Remember that Fenwick trees are 1-indexed! You do not need to
 * handle the case where the index is out of bounds.
 */
int cumulative_frequency(const struct fenwick_tree* tree, size_t index);

#endif
