package rmq;

public class KLookup {

  private int[] kLookup;

  /**
   * Initialize a lookup table for k values to allow constant time lookup
   */
  public KLookup(int n) {
    kLookup = new int[n + 1];
    int k = 0;
    int two_to_the_k = 1;
    for (int i = 0; i < n + 1; i++) {
      if (two_to_the_k <= i) {
        k++;
        two_to_the_k *= 2; // optimized to bitshift anyway
      }
      kLookup[i] = Math.max(0, k - 1); // because k is already too large
    }
  }

  public int at(int i) {
    int k = kLookup[i];
    // System.out.println( "kLookup at " + i + " resulted in " + k);
    return k;
  }

  @Override
  public String toString() {
    return java.util.Arrays.toString(kLookup);
  }

}