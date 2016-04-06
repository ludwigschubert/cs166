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
      // System.out.println( "this.topElems" + java.util.Arrays.toString(this.topElems) );
      // System.out.println("Constructing cartesianTreeNumber: (" + i + "," + j + ")");
    }

    protected int bottomStructure_rmq(int i, int j) {
      System.out.println( "i: " + i + " j: " + j + " blockSize: " + this.blockSize );
      assert j - i < this.blockSize;

      int blockIndex = i / this.blockSize;
      assert blockIndex == j / this.blockSize;

      int cartesianTreeNumber = this.cartesianTreeNumbers[blockIndex];
      PrecomputedRMQ rmqStructure = this.bottomRMQStructures[cartesianTreeNumber];
      int offset = blockIndex * this.blockSize;
      System.out.println( "offset: " + offset);
      int minimumIndex = rmqStructure.rmq(i - offset, j - offset); // todo

      return minimumIndex + offset; // todo
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
