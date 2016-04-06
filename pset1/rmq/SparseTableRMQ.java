package rmq;

/**
 * An &lt;O(n log n), O(1)&gt; implementation of RMQ that uses a sparse table
 * to do lookups efficiently.
 *
 * You will implement this class for problem 3.ii of Problem Set One.
 */
public class SparseTableRMQ implements RMQ {

    private KLookup kLookup;
    private int[][] lookup;
    private float[] elems;

    /**
     * Creates a new SparseTableRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public SparseTableRMQ(float[] elems) {
      this.elems = elems;
      int n = elems.length;
      kLookup = new KLookup(n);
      int log_n = kLookup.at(n);
      lookup = new int[n][log_n+1];

      // Fill first column with identities
      for (int i = 0; i < n; i++) {
        lookup[i][0] = i;
      }

      // Construct table by taking mins of pairs of half the powers of two
      for (int k = 1; k <= log_n; k++) {
        int numberOfMins = n - (int)Math.pow(2, k) + 1;
        // System.out.println( "k: " + k + " numberOfMins: " + numberOfMins );
        for (int index = 0; index < numberOfMins; index++) {
          int offset = (int)Math.pow(2, k - 1);
          int first  = lookup[index         ][k - 1];
          int second = lookup[index + offset][k - 1];
          int minimumIndex = elems[first] <= elems[second] ? first : second;
          lookup[index][k] = minimumIndex;
        }
      }
      // System.out.println( kLookup.toString() );

    }

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
      assert i <= j;
      assert i >= 0;
      assert i < elems.length;
      assert j >= i;
      assert j < elems.length;
      // System.out.println( i ); System.out.println( j );
      // System.out.println( "kLookup: " + kLookup.toString() );
      // System.out.println( "elems: " + java.util.Arrays.toString(elems) );
      // printLookupTable();
      // System.out.println( "queried: (" + i + ", " + j + ")" );
      int k = kLookup.at(j - i + 1);
      int i_ = i + (int)Math.pow(2, k) - 1;
      int j_ = j - (int)Math.pow(2, k) + 1;
      // System.out.println( "Overlapping: (" + i + ", " + i_ + ") and (" + j_ + ", " + j + ")" );
      int first  = lookup[i ][k];
      int second = lookup[j_][k];
      // System.out.println( "MinIndex of first: " + first + " MinIndex of second: " + second);
      int minimumIndex = elems[first] <= elems[second] ? first : second;
      return minimumIndex;
    }

    private void printLookupTable() {
      String output = "LookupTable";
      for (int i = 0; i < lookup.length; i++) {
        output += "\n| ";
        for (int j = 0; j < lookup[0].length; j++) {
            output += lookup[i][j] + " ";
          }
      }
      System.out.println(output);
    }

}
