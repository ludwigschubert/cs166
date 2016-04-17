#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "kmp.h"

/* Constant: kMaxResults
 * ---------------------------------------------------------------------------
 * The maximum number of results allowed; used as a sanity check for infinite
 * loops.
 */
static const size_t kMaxResults = 1 << 20;

/* Constants: kBufferSize, kStringSize
 * ---------------------------------------------------------------------------
 * The buffer size to use when reading test cases. kStringSize must be defined
 * to be a numeric literal exactly equal to kBufferSize - 1, as it's used in a
 * later macro controlling fscanf format strings.
 */
#define kStringSize 65535
#define kBufferSize 65536

/* Macro: MAX_SIZE
 * ---------------------------------------------------------------------------
 * A string representation of the maximum buffer size minus one.
 * The macro techniques here are based on code from Stack Overflow by R Sahu
 * (http://stackoverflow.com/a/25410835/)
 */
#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define MAX_SIZE STRINGIFY2(kStringSize)

/* Type: test_results
 * ---------------------------------------------------------------------------
 * A struct used to store information about the matches reported by a KMP 
 * matcher.
 */
struct test_results {
  size_t logical_length;
  size_t allocated_length;
  ptrdiff_t* matches;
  const char* base;
  size_t string_length;
};

/* Macro: error(msg)
 * ---------------------------------------------------------------------------
 * Reports an error and terminates the program.
 */
#define error(msg) do_error(msg, __FILE__, __LINE__)
static void do_error(const char* msg, const char* file, unsigned line) {
  fprintf(stderr, "ERROR: %s\nFile: %s\nLine: %u\n", msg, file, line);
  exit(-1);
}

/* Function: make_result(const char* base);
 * ---------------------------------------------------------------------------
 * Creates a new test_results struct for storing matches r
 */
static struct test_results* make_result(const char* base) {
  struct test_results* result = malloc(sizeof(*result));
  if (result == NULL) {
    error("No memory for result!");
  }

  /* Initialize the dynamic array. */
  result->logical_length = 0;
  result->allocated_length = 1;
  result->matches = 
    malloc(sizeof(*result->matches) * result->allocated_length);

  if (result->matches == NULL) {
    error("No memory for dynamic array!");
  }

  /* Store information about the test case. */
  result->base = base;
  result->string_length = strlen(base);

  return result;
}

/* Function: add_result(struct test_results* results,
 *                      ptrdiff_t result);
 * ---------------------------------------------------------------------------
 * Appends a matching result to the end of the results struct.
 */ 
static void add_result(struct test_results* results,
		       ptrdiff_t result) {
  if (results->logical_length == results->allocated_length) {
    results->allocated_length *= 2;
    results->matches = realloc(results->matches, 
			       sizeof(*results->matches) * results->allocated_length);
    if (results->matches == NULL) {
      error("Could not grow dynamic array.");
    }
  }
  results->matches[results->logical_length] = result;
  results->logical_length++;
}

/* Function: dispose_results(struct test_results* results);
 * ---------------------------------------------------------------------------
 * Cleans up all the memory associated with a test_results struct.
 */
static void dispose_results(struct test_results* results) {
  free(results->matches);
  free(results);
}

/* Callback: handle_result
 * ---------------------------------------------------------------------------
 * Handles a reported match inside a string.
 */
static void handle_result(const char* match, void* aux) {
  struct test_results* results = (struct test_results*) aux;

  if (results->logical_length >= kMaxResults) {
    error("Too many matches reported; possible infinite loop?");
  }

  add_result(results, match - results->base);
}

static void run_tests(const char* test_file) {
  FILE* f = fopen(test_file, "r");
  if (f == NULL) error("Can't open test file.");
  
  while (true) {
    /* Read the current pattern string. */
    char pattern[kBufferSize];
    if (fscanf(f, "%" MAX_SIZE "s", pattern) != 1) break;
    printf("Pattern: (%s)\n", pattern);

    struct matcher* matcher = create_matcher_for(pattern);

    /* Process each test case. */
    size_t num_test_cases;
    if (fscanf(f, "%zu\n", &num_test_cases) != 1) error("Cant read number of tests");

    for (size_t i = 0; i < num_test_cases; i++) {
      char text[kBufferSize];
      if (fscanf(f, "%" MAX_SIZE "s", text) != 1) error("Can't read test case.");

      printf("  Test case: (%s)\n", text);

      struct test_results* results = make_result(text);
      for_each_match(matcher, text, handle_result, results);

      /* Confirm that the matches are correct. */
      size_t num_matches;
      if (fscanf(f, "%zu", &num_matches) != 1) error("Can't read number of results.");

      size_t max = (num_matches > results->logical_length? num_matches : results->logical_length);

      for (size_t i = 0; i < max; i++) {
	/* See what's expected - as long as there's still something to read. */
	ptrdiff_t expected;
	if (i < num_matches) {
	  if (fscanf(f, "%td", &expected) != 1) error("Can't read specific result.");
	}

	/* Report errors or mismatches. */
	if (i >= results->logical_length) {
	  printf("    Expected %td, but there was no corresponding match.\n", expected);
	} else if (i >= num_matches) {
	  printf("    Did not expect anything, but a match was reported at %td\n", results->matches[i]);
	} else if (expected != results->matches[i]) {
	  printf("    Expected %td, but got %td\n", expected, results->matches[i]);
	}
      }

      dispose_results(results);
    }
    
    destroy_matcher(matcher);
  }
  fclose(f);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: run-tests test-file-name\n");
    return -1;
  }

  run_tests(argv[1]);
  return 0;
}
