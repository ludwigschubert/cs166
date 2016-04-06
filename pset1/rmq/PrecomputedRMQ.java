package rmq;
/**
 * An &lt;O(n<sup>2</sup>), O(1)&gt; implementation of RMQ that precomputes the
 * value of RMQ_A(i, j) for all possible i and j.
 *
 * You will implement this class for problem 3.i of Problem Set One.
 */
public class PrecomputedRMQ implements RMQ {

    public int[][] lookup;

    /**
     * Creates a new PrecomputedRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public PrecomputedRMQ(float[] elems) {

      int numberOfElements = elems.length;
      lookup = new int[numberOfElements][numberOfElements];

      // Fill diagonal with identities
      for (int i = 0; i < numberOfElements; i++) {
        lookup[i][i] = i;
      }

      // Construct table by taking mins of pairs left and below entries
      for (int level = 1; level < numberOfElements; level++) {
        int numberOfPossibleLookups = numberOfElements - level;
        for (int index = 0; index < numberOfPossibleLookups; index++) {
          int first  = lookup[index    ][index + level - 1];
          int second = lookup[index + 1][index + level    ];
          int minimumIndex = elems[first] <= elems[second] ? first : second;
          lookup[index][index+level] = minimumIndex;
        }
      }

    }

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
    	return lookup[i][j];
    }
}
