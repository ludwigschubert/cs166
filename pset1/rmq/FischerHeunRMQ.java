package rmq;
import java.util.*;

/**
 * An &lt;O(n), O(1)&gt; implementation of the Fischer-Heun RMQ data structure.
 *
 * You will implement this class for problem 3.iv of Problem Set One.
 */
public class FischerHeunRMQ extends HybridRMQ {

    protected PrecomputedRMQ[] bottomRMQStructures;
    protected int[] cartesianTreeNumbers;

    /**
     * Creates a new FischerHeunRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public FischerHeunRMQ(float[] elems) {
      super(elems);
    }

    @Override
    protected void prepareRMQStructure(){
      super.prepareRMQStructure();

      int n = elems.length;
      this.bottomRMQStructures = new PrecomputedRMQ[(int)Math.pow(4, this.blockSize)];
      int numberOfBlocks = n / this.blockSize;
      this.cartesianTreeNumbers = new int[numberOfBlocks];
      for (int index = 0; index < numberOfBlocks; index++) {
        int i =  index      * this.blockSize;
        int j = (index + 1) * this.blockSize - 1;
        if (j >= n) { j = n - 1; } // last block is not necessarily full size

        // System.out.println( "----------------------------------------------" );
        // System.out.println( "n: " + n + " blockSize: " + this.blockSize );
        // System.out.println( "elems" + java.util.Arrays.toString(this.elems) );
        // System.out.println( "this.topElems" + java.util.Arrays.toString(this.topElems) );
        // System.out.println("Constructing cartesianTreeNumber: (" + i + "," + j + ")");

        int cartesianTreeNumber = cartesianTreeNumber(i,j);
        this.cartesianTreeNumbers[index] = cartesianTreeNumber;
        if (this.bottomRMQStructures[cartesianTreeNumber] == null) {
          float[] subset = Arrays.copyOfRange(this.elems, i, j+1);
          this.bottomRMQStructures[cartesianTreeNumber] = new PrecomputedRMQ(subset);
        }
      }

      System.out.println( "----------------------------------------------" );
      System.out.println( "n: " + n + " blockSize: " + this.blockSize );
      System.out.println( "elems" + java.util.Arrays.toString(this.elems) );
      for (int i = 0; i < this.bottomRMQStructures.length; i++) {
        PrecomputedRMQ structure = this.bottomRMQStructures[i];
        if (structure != null) {
          System.out.println( structure.toString() );
        };
      }

      // System.out.println( "this.topElems" + java.util.Arrays.toString(this.topElems) );
      // System.out.println("Constructing cartesianTreeNumber: (" + i + "," + j + ")");
    }

    protected int bottomStructure_rmq(int i, int j) {
      System.out.println( "blockSize: " + this.blockSize );

      int blockIndex1 = i / this.blockSize;
      int blockIndex2 = j / this.blockSize;

      System.out.println( "blockIndex1: " + blockIndex1 + " blockIndex2: " + blockIndex2);

      if (blockIndex1 == blockIndex2) {
        return singleBlock_rmq(blockIndex1, i, j);
      } else {
        int block1MinimumIndex = singleBlock_rmq(blockIndex1, i, j);
        float block1Minimum = this.elems[block1MinimumIndex];
        int block2MinimumIndex = singleBlock_rmq(blockIndex2, i, j);
        float block2Minimum = this.elems[block2MinimumIndex];
        System.out.println( "block1Minimum: " + block1MinimumIndex + "," + block1Minimum + " block2Minimum: " + + block2MinimumIndex + "," + block2Minimum);
        if (block1Minimum < block2Minimum) {
          return block1MinimumIndex;
        } else {
          return block2MinimumIndex;
        }
      }
    }

    private int singleBlock_rmq(int blockIndex, int i, int j) {
      int cartesianTreeNumber = this.cartesianTreeNumbers[blockIndex];
      System.out.println( "blockIndex: " + blockIndex + " query: (" + i % this.blockSize + "," + j % this.blockSize + "), cart:" + cartesianTreeNumber);
      PrecomputedRMQ rmqStructure = this.bottomRMQStructures[cartesianTreeNumber];
      int minimumIndex = rmqStructure.rmq(i % this.blockSize, j % this.blockSize);
      System.out.println( "minimumIndex: " + minimumIndex + " blockIndex: " + blockIndex + " this.blockSize: " + this.blockSize);
      return minimumIndex + blockIndex * this.blockSize;
    }

    private int cartesianTreeNumber(int i, int j) {
      Stack<Float> stack = new Stack<Float>();
      int cartesianTreeNumber = 0;
      for (int index = i; index < j; index++) {
        float blockElement = this.elems[index];
        if (stack.isEmpty()) {
          stack.push(blockElement);
          cartesianTreeNumber = cartesianTreeNumber << 1;
          cartesianTreeNumber = cartesianTreeNumber | 1;
          continue;
        } else {
          float stackElement = stack.peek();
          if (blockElement > stackElement) {
            stack.push(blockElement);
            cartesianTreeNumber = cartesianTreeNumber << 1;
            cartesianTreeNumber = cartesianTreeNumber | 1;
            continue;
          } else {
            while (!stack.isEmpty() && blockElement < stackElement) {
              stackElement = stack.pop();
              cartesianTreeNumber = cartesianTreeNumber << 1;
            }
            stack.push(blockElement);
            cartesianTreeNumber = cartesianTreeNumber << 1;
            cartesianTreeNumber = cartesianTreeNumber | 1;
            continue;
          }
        }
      }
      while (!stack.isEmpty()) {
        stack.pop();
        cartesianTreeNumber = cartesianTreeNumber << 1;
      }
      return cartesianTreeNumber;
    }
}
