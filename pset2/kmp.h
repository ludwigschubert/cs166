#ifndef KMP_Included
#define KMP_Included

/* Type: matcher
 * ---------------------------------------------------------------------------
 * An opaque type representing a Knuth-Morris-Pratt matcher. You will define
 * the internals in kmp.c.
 */
struct matcher;

/* Function: struct matcher* create_matcher_for(const char* pattern);
 * Usage: struct matcher* find_meaning = create_matcher_for("meaning");
 * ---------------------------------------------------------------------------
 * Given a pointer to a null-terminated string, creates and returns a pointer
 * to a newly-constructed matcher object that can be used to match all copies
 * of that string.
 *
 * You can assume that the provided pattern is given as a valid null-terminated
 * string and don't need to check for this. Your program can behave however
 * you'd like if you run out of memory. You can also assume that the pattern
 * has length at least one.
 *
 * The client should be able to deallocate or mutate the contents of the
 * buffer pointed at by pattern without causing the returned matcher to
 * malfunction. You may want to make your own local copy of the pattern
 * string in the event that you'd like to reference it later on.
 */
struct matcher* create_matcher_for(const char* pattern);

/* Function: destroy_matcher(struct matcher* matcher);
 * ---------------------------------------------------------------------------
 * Disposes of all memory associated with the given matcher.
 */
void destroy_matcher(struct matcher* matcher);

/* Function: for_each_match(struct matcher* matcher,
 *                          const char* text,
 *                          void (*callback)(const char* match, void* aux),
 *                          void* aux);
 * ---------------------------------------------------------------------------
 * Given a matcher object, a text string, and a callback function to invoke on
 * all matches in that string, uses the matcher to report back all matches of
 * the pattern string inside of the text string. Each match is reported by
 * calling the specified callback, passing as arguments a pointer to the
 * occurrence of the match inside the text string, along with some auxiliary
 * data of the user's choice. For example, given a matcher for "aabaa" and the
 * string aabaabaabaa, the callback should be invoked on these positions:
 *
 *    aabaabaabaa
 *    ^  ^  ^
 *
 * You should invoke the callback function on the matches in the string in the
 * order in which they appear and should only invoke the callback once per
 * match. When reporting a match, you just need to pass a pointer to the spot
 * in the string where the match occurs. For efficiency reasons, you should
 * not make a copy of that match, nor should you attempt to change the
 * text string.
 */
void for_each_match(struct matcher* matcher,
		    const char* text,
		    void (*callback)(const char* match, void* aux),
		    void* aux);

#endif
