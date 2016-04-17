package rmq;
/**
 * An &lt;O(n), O(log n)&gt; implementation of the RMQ as a hybrid between
 * the sparse table (on top) and no-precomputation structure (on bottom)
 *
 * You will implement this class for problem 3.iii of Problem Set One.
 */
public class HybridRMQ implements RMQ {

    protected float[] elems;
    protected SparseTableRMQ topStructure;
    protected int[] topElemsMinIndexes;
    protected float[] topElems;
    protected int blockSize;

    /**
     * Creates a new HybridRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public HybridRMQ(float[] elems) {
        this.elems = elems;
        if (elems.length > 1) {
          prepareRMQStructure();
        }
    }

    protected void prepareRMQStructure(){
      int n = elems.length;
      this.blockSize = (int) Math.floor( Math.log(n)/Math.log(2) );
      int numberOfBlocks = n / this.blockSize;

      // preprocess this.topElems
      this.topElems           = new float[numberOfBlocks];
      this.topElemsMinIndexes = new int[numberOfBlocks];
      for (int index = 0; index < numberOfBlocks; index++) {
        int i =  index      * this.blockSize;
        int j = (index + 1) * this.blockSize - 1;
        if (j >= n) { j = n - 1; } // last block is not necessarily full size
        // System.out.println("Constructing topElems: (" + i + "," + j + ")");
        int minimumIndex = linearScan(i,j);
        this.topElemsMinIndexes[index] = minimumIndex;
        this.topElems[index] = elems[minimumIndex];
      }

      this.topStructure = new SparseTableRMQ(this.topElems);
      // System.out.println( "----------------------------------------------" );
      // System.out.println( "n: " + n + " blockSize: " + this.blockSize );
      // System.out.println( "elems" + java.util.Arrays.toString(this.elems) );
      // System.out.println( "this.topElems" + java.util.Arrays.toString(this.topElems) );
    }

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
//        System.out.println("Queried: (" + i + "," + j + ")");
        if (this.topStructure == null) {
          return linearScan(i, j);
        } else {

          int blockIndexStart = i / this.blockSize;
          int blockIndexEnd = j / this.blockSize;
          int numberOfBlocksSpanned = blockIndexEnd - blockIndexStart + 1;

          if (numberOfBlocksSpanned <= 2) {

            return bottomStructure_rmq(i, j);

          } else { // spans three or more blocks

            // Compute indices
            int iTop         = i / this.blockSize + 1;
            int jTop         = j / this.blockSize - 1;
            int iBottomLeft  = i;
            int jBottomLeft  = iTop * this.blockSize - 1;
            int iBottomRight = jTop * this.blockSize;
            int jBottomRight = j;

            // System.out.println("Top: (" + iTop + "," + jTop + ") " + "Left: ("  + iBottomLeft + "," + jBottomLeft + ") " + "Right: (" + iBottomRight + "," + jBottomRight + ") ");

            // Compute mins
            int   minimumIndexBottomLeft  = bottomStructure_rmq(iBottomLeft, jBottomLeft);
            float minimumBottomLeft       = this.elems[minimumIndexBottomLeft];
            int   minimumIndexBottomRight = bottomStructure_rmq(iBottomRight, jBottomRight);
            float minimumBottomRight      = this.elems[minimumIndexBottomRight];
            int   minimumIndexTopInTopStructure = this.topStructure.rmq(iTop, jTop);
            int   minimumIndexTopInElems  = this.topElemsMinIndexes[minimumIndexTopInTopStructure];
            float minimumTop              = this.elems[minimumIndexTopInElems];

            int minimumIndex;
            // Argmin, basically
            if (minimumBottomLeft < minimumBottomRight) {
              if (minimumBottomLeft < minimumTop) {
                minimumIndex = minimumIndexBottomLeft;
              } else {
                minimumIndex = minimumIndexTopInElems;
              }
            } else {
              if (minimumBottomRight < minimumTop) {
                minimumIndex = minimumIndexBottomRight;
              } else {
                minimumIndex = minimumIndexTopInElems;
              }
            }

            return minimumIndex;

          }

        }
    }

    protected int bottomStructure_rmq(int i, int j) {
      return linearScan(i, j);
    }

    protected int linearScan(int i, int j) {
      float currentMinimum = Float.MAX_VALUE;
      int minimumIndex = Integer.MIN_VALUE;
      for (int index = i; index <= j; index++) {
        float currentValue = this.elems[index];
        if (currentValue < currentMinimum) {
          currentMinimum = currentValue;
          minimumIndex = index;
        }
      }
      return minimumIndex;
    }
}
