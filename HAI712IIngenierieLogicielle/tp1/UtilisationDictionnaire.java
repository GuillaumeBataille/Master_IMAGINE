package HAI712IIngenierieLogicielle.tp1;

public class UtilisationDictionnaire {
	
	
	static IDictionary dic;
	
	
	   public static void main(String[] args)
	   {
		
		OrderedDictionary instDico = new OrderedDictionary();
		dic = instDico;
		//dic= new OrderedDictionary();
		Object k0 = 1;
		Object v0 = new String ("valeur1") ;
		//dic.isEmpty();//v
		System.out.println("vide??");
		System.out.println(dic.isEmpty());
		System.out.println("");
		
		System.out.println("Taille avant insertion");
		System.out.println(dic.size());//0
		System.out.println("");
		
		System.out.println("Insertion");
		System.out.println(dic.put(k0,v0));
		System.out.println("");
		
		System.out.println("Retourne valeur");
		System.out.println(dic.get(1));//valeur1
		System.out.println("");
		
		System.out.println("Taille apres insertion");
		System.out.println(dic.size());//1
		System.out.println("");
		
		System.out.println("Contient la clef 1??");
		System.out.println(dic.containsKey(1));//v
		System.out.println("");
		
		System.out.println("Contient la clef 2??");
		System.out.println(dic.containsKey(2));//f
		System.out.println("");
		
		System.out.println("vide??");
		System.out.println(dic.isEmpty());//f
		
		
		
		
		
		//return 0;
	}
	
	
	
}
