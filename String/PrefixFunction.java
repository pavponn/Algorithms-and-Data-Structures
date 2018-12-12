public class PrefixFunction {

    public int[] prefixFunction(String str) {

        int[] p = new int[str.length()];
        p[0] = 0;

        for (int i = 1; i < str.length(); ++i) {
            int j = p[i - 1];
            while (j > 0 && str.charAt(i) != str.charAt(j)) {
                j = p[j - 1];
            }
            if (str.charAt(i) == str.charAt(j)) {
                ++j;
            }
            p[i] = j;
        }
        return p;
    }

    public void test() {
        String str = "lol#abacabalolollo";
        int [] prefixFunction = prefixFunction(str);
        for (int p: prefixFunction) {
            System.out.print(p + " ");
        }

    }

    public static void main(String[] args) {
        new PrefixFunction().test();

    }
}
