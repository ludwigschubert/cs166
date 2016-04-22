#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fenwick-tree.h"

/* Constants controlling the size of the ranges to test. */
static const size_t kMaxSmallSize = 10;
static const size_t kMaxLargeSize = 1000000;
static const size_t kStepSize =     100000;

/* Runs a small set of tests on the Fenwick tree, reporting output to stdout
 * for testing purposes.
 */
static void smallTests(void) {
	printf("==== Start Small Tests ====\n");
	for (size_t size = 0; size <= kMaxSmallSize; size++) {
		struct fenwick_tree* tree = create_tree_of_size(size);
		printf("Tree size: %zu\n", size);
		
		/* Increment each index by its position. This has the net effect of making
		 * an array of triangular numbers.
		 */
		for (size_t i = 1; i <= size; i++) {
			increment(tree, i, (int)i);
		}
		
		/* Print out a comparison between the expected and actual values. */
		printf("  Expected: ");
		for (size_t i = 1; i <= size; i++) {
			/* The expected values are the triangular numbers, which are given by
			 * n(n + 1) / 2.
			 */
			int n = (int)i;
			int expected = n * (n + 1) / 2;
			printf("%d ", expected);
		}
		printf("\n  Actual:   ");
		for (size_t i = 1; i <= size; i++) {
		  	printf("%d ", cumulative_frequency(tree, i));
		}
		printf("\n");
		
		destroy_tree(tree);
	}
}

/* Stress-tests the Fenwick tree on larger values. */
static void largeTests(void) {
	printf("==== Start Large Tests ====\n");
	for (size_t size = 0; size <= kMaxLargeSize; size += kStepSize) {
		struct fenwick_tree* tree = create_tree_of_size(size);
		printf("Tree size: %zu\n", size);
		
		/* Increment each index by its position. This has the net effect of making
		 * an array of triangular numbers.
		 */
		for (size_t i = 1; i <= size; i++) {
			increment(tree, i, (int)i);
		}
		
		/* The expected values are the triangular numbers, which are given by
		 * n(n + 1) / 2.
		 */
		bool success = true;
		for (size_t i = 1; i <= size; i++) {
			int expected = i * (i + 1) / 2;
			int actual   = cumulative_frequency(tree, i);
			if (expected != actual) {
				success = false;
				break;
			}
		}
		
		if (success) {
			printf("    Pass\n");
		} else {
			printf("  ! Failed!\n");
		}
		
		destroy_tree(tree);
	}
}

int main(int argc, char* argv[]) {
	smallTests();
	largeTests();
	return 0;
}
