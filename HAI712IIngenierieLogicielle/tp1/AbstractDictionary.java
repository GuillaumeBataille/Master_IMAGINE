package HAI712IIngenierieLogicielle.tp1;

public abstract class AbstractDictionary implements IDictionary {
    protected Object[] value;
    protected  Object[] key;

    public AbstractDictionary(int i) {
        this.value = new Object[i];
        this.key = new Object[i];
    }
    public Object get(Object Key) {
        int indice =  indexOf(Key);
        if(indice== -1){
            return null;
        }
        return value[indice];
    }

    public IDictionary put(Object key, Object value) {
        if (!this.containsKey(key)) {

            int indice = newIndexOf(key);
            this.key[indice] = key;
            this.value[indice] = value;
        } else {
            int indice = indexOf(key);
            this.value[indice] = value;
        }
        return this;
    }
    public boolean isEmpty() {
        return this.value.length == 0;
    }
    public boolean containsKey(Object key) {
        return (indexOf(key) != -1);

    }

    public abstract int size() ;

    public abstract int indexOf(Object key);

    public abstract int newIndexOf(Object key);
}
