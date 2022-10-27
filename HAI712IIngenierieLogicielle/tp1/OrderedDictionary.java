package HAI712IIngenierieLogicielle.tp1;
//import javax.swing.plaf.basic.BasicSplitPaneUI;
import java.util.Arrays;

public class OrderedDictionary extends AbstractDictionary {
    public OrderedDictionary() {
        super(0);

    }
    //dic prend les methodes  redéfinis dans ordered (si jamais elle sont redef)

//affectation polymorphique : affectation d'un sous ensemble

    @Override
    public int size() {
        return this.key.length;
    }

    @Override
    public int indexOf(Object key) {

        for (int i = 0; i < this.size(); i++) {
            if (this.key[i].equals(key)) {
                return i;
            }
        }
        return -1;
    }

    @Override
    public int newIndexOf(Object key) {
        Object[] copiecle = new Object[size() + 1];
        Object[] copievaleur = new Object[size() + 1];
        System.arraycopy(this.key, 0, copiecle, 0, this.key.length);
        System.arraycopy(this.value, 0, copievaleur, 0, this.value.length);
        this.key = copiecle;
        this.value = copievaleur;
        return size() - 1;
        //
    }

    @Override
    public String toString() {
        return "OrderedDictionary{" +
                "value=" + Arrays.toString(value) +
                ", key=" + Arrays.toString(key) +
                '}';
    }
}