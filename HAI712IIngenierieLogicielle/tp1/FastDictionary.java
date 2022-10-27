package HAI712IIngenierieLogicielle.tp1;

import java.util.Arrays;

import static java.lang.Math.abs;

public class FastDictionary extends AbstractDictionary {
    public FastDictionary(int i) {
        super(i);
    }

    @Override
    public int size() {
        int j = 0;
        for (Object i : this.key) {
            if (i != null) {
                j++;
            }
        }
        return j;
    }

    @Override
    public int indexOf(Object key) {
        int k = abs(key.hashCode() % (this.key.length));
        int cpt = 0;
        while (!(this.key[k] == key)) {
            k = (k + 1) % (this.key.length);
            cpt++;
            if (cpt == this.key.length) {
                return -1;
            }
        }
        return k;


    }

    @Override
    public String toString() {
        return "FastDictionary{" +
                "value=" + Arrays.toString(value) +
                ", key=" + Arrays.toString(key) +
                '}';
    }

    @Override
    public int newIndexOf(Object key) {
        //verif si tableau 0.75 plein , si oui j'agrandis
        if (mustGrow()) {
            grow();}
        int k = abs(key.hashCode() % (this.key.length));

            while ((this.key[k] != null)) {
                k = (k + 1) % (this.key.length);
            }
        return k;
    }

    public boolean mustGrow() {
        return (this.size() >= 0.75 * this.key.length);
    }

    public void grow() {
        Object[] copiecle = new Object[2 * (this.key.length)];
        Object[] copievaleur = new Object[2 * (this.key.length)];
        System.arraycopy(this.key, 0, copiecle, 0, this.key.length);
        System.arraycopy(this.value, 0, copievaleur, 0, this.value.length);
        this.key = copiecle;
        this.value = copievaleur;
    }
}