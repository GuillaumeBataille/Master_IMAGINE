package HAI712IIngenierieLogicielle.tp1;

public interface IDictionary  {
	//rend la valeur associ ́ee `a key dans le receveur.
	public Object get(Object key);
	//entre une nouveau couple cle-valeur dans le receveur, rend le receveur.
	public IDictionary put(Object key, Object value);
	//rend True si le receveur est vide, faux sinon
	public boolean isEmpty() ;
	//rend True si la cĺe est connue dans le receveur, faux sinon.
	public boolean containsKey(Object key);
	public int size();

}
