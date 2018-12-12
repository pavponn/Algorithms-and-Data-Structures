public class ZFunction {

    public int[] zFunction(String str) {

        int[] z = new int[str.length()];
        int l = 0;
        int r = 0;
        for (int i = 1; i < str.length(); ++i) {
            if (i <= r) {
                z[i] = Math.min(r - i + 1, z[i - l]);
            }
            while (z[i] + i < str.length() && str.charAt(z[i]) == str.charAt(z[i] + i)) {
                ++z[i];
            }
            if (i + z[i] - 1 > r) {
                l = i;
                r = i + z[i] - 1;
            }

        }
        return z;
    }

    public void test() {
        String str = "lol#abacabalolollo";
        int[] zFunction = zFunction(str);
        for (int i = 1; i < zFunction.length; ++i) {
            System.out.print(zFunction[i] + " ");
        }
    }

    public static void main(String[] args) {
        new ZFunction().test();

    }
}
