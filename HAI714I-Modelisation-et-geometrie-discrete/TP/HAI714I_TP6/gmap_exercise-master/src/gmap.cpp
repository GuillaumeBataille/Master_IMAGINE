// #include "gmap.hpp"
#include <algorithm>
#include <iostream>
#ifdef GMAP_CORE
/*------------------------------------------------------------------------*/

/* 
    Create a new dart and return its id. 
    Set its alpha_i to itself (fixed points) 
    Use maxid to determine the new id. Dont forget to increment it.
*/
// Créer un nouveau brin qui pour tout alpha 0 1 ou 2 pointe vers lui même
GMap::id_t GMap::add_dart() // NEW 1.a ajout d'un nouveau brin
{
    id_t dart = maxid;                                  // Récup maxid pour l'id du dart courant
    maxid++;                                            // Incrément de maxid pour le prochain dart
    alphas[dart] = alpha_container_t(dart, dart, dart); // Set les alpha a lui même
    return dart;
}

/*------------------------------------------------------------------------*/

// Return the application of the alpha_deg on dart
// Si on veut récup un alpha deg particulier
GMap::id_t GMap::alpha(degree_t degree, id_t dart) const // NEW 1.a fix l'affichage
{
    assert(degree < 3);
    assert(dart < maxid);
    return alphas.at(dart)[degree];
}

// Return the application of a composition of alphas on dart
// Si on veut récup une sucession de alpha deg successif stocké dans un vect (a lire a l'envers)
GMap::id_t GMap::alpha(degreelist_t degrees, id_t dart) const //NEW  1.b gère un vecteur de degré
{
    std::reverse(degrees.begin(), degrees.end()); // Inversion des degré
    for (degree_t degree : degrees)
    { // Pour tout les elements degree de degrees
        assert(degree < 3);
        assert(dart < maxid);
        dart = alphas.at(dart)[degree];
    }
    return dart;
}

// Test if dart is free for alpha_degree (if it is a fixed point)
// Test si on a un point fixe : c'est un point qui lorsque qu'il subit alpha deg il reste le même
bool GMap::is_free(degree_t degree, id_t dart) const // NEW 1.b teste si un vect de degré est libre
{
    assert(degree < 3);
    assert(dart < maxid);
    return alpha(degree, dart) == dart;
}

/*------------------------------------------------------------------------*/

// Link the two darts with a relation alpha_degree if they are both free
// Lie un dart a un autre avec comme relation alpha degree ssi ils sont libres tout les deux.
bool GMap::link_darts(degree_t degree, id_t dart1, id_t dart2) // NEW 1.c
{
    // Test si ils sont libres
    if (!is_free(degree, dart1))
        return false;
    if (!is_free(degree, dart2))
        return false;

    //Association de 1 a 2 et 2 a 1 pour l'alpha degree;
    alphas[dart1][degree] = dart2;
    alphas[dart2][degree] = dart1;
    return true;
}

/*------------------------------------------------------------------------*/

/*
        Test the validity of the structure. 
        Check if alpha_0 and alpha_1 are involutions with no fixed points.
        Check if alpha_2 is an involution.
        Check if alpha_0 o alpha_2 is an involution
*/
// Verifie les test evoqué au dessus :
bool GMap::is_valid() const //NEW 1.d
{

    //darts() retourne la liste de tout les darts créé
    for (id_t dart : darts())
    { // Pour tout dart

        //Alpha0
        if (alpha({0, 0}, dart) != dart) // Si alpha 0 n'est pas une involution
            return false;                // pas valide
        if (is_free(0, dart))            // Si dart est libre
            return false;                // pas valide

        //Alpha1
        if (alpha({1, 1}, dart) != dart) // Si alpha 1 n'est pas une involution
            return false;                // pas valide
        if (is_free(1, dart))            // Si dart n'est pas libre en alpha1
            return false;                // pas valide

        //Alpha2
        if (alpha({2, 2}, dart) != dart)       // Si alpha 2 n'est pas une involution
            return false;                      // pas valide
        if (alpha({0, 2, 0, 2}, dart) != dart) // Si on fait le tour et qu'on retombe pas sur dart
            return false;                      // pas valide
    }
    return true;
}

/*------------------------------------------------------------------------*/

/* 
    Return the orbit of dart using a list of alpha relation.
    Example of use : gmap.orbit([0,1],0).
*/
GMap::idlist_t GMap::orbit(const degreelist_t &alphas, id_t dart) const // NEW 2
{
    idlist_t result;             // Liste resultat
    idset_t marked;              // Set des objets marqués (set contient la methode .count())
    idlist_t toprocess = {dart}; // Liste contenant les elements a traiter
    id_t current_dart;           // Le dart courant
    while (toprocess.size() != 0)
    {                                    // Tant qu'il y a des elements a traiter
        current_dart = toprocess.back(); // Recup le dernier element de la liste a traiter

        if (marked.count(current_dart) == 0) // Si d n'est pas marqué
        {
            marked.insert(current_dart);    // On l'ajoute dans marked
            result.push_back(current_dart); // On l'ajoute dans result
            toprocess.pop_back();           // On passe au prochain a traiter
            for (degree_t degree : alphas)  // Pour chaque degree de list_of_alpha_value
            {
                toprocess.push_back(alpha(degree, current_dart)); // Rajouter alpha_degree(d) dans to_process
            }
        }
        else // Si il est deja marqué
        {
            toprocess.pop_back(); // On passe au prochain a traiter
        }
    }
    return result;
}

/*
    Return the ordered orbit of dart using a list of alpha relations by applying
    repeatingly the alpha relations of the list to dart.
    Example of use. gmap.orderedorbit([0,1],0).
    Warning: No fixed point for the given alpha should be contained.
*/
GMap::idlist_t GMap::orderedorbit(const degreelist_t &list_of_alpha_value, id_t dart) const
{
    idlist_t result;
    id_t current_dart = dart;
    unsigned char current_alpha_index = 0;
    unsigned char next_alpha_index;
    size_t n_alpha = list_of_alpha_value.size();

    do
    {
        result.push_back(current_dart);                              // ajouter current_dart au resultat
        next_alpha_index = list_of_alpha_value[current_alpha_index]; // prendre le prochain alpha de list_of_alpha_value avec current_alpha_index
        current_alpha_index = (current_alpha_index + 1) % n_alpha;   // incrémenter current_alpha_index
        current_dart = alpha(next_alpha_index, current_dart);        // changer current_dart par alpha_current_alpha(current_dart)
    } while (current_dart = !dart);                                  // Tant que current_dart est différent de dart;

    return result;
}

/*
    Sew two elements of degree 'degree' that start at dart1 and dart2.
    Determine first the orbits of dart to sew and heck if they are compatible.
    Sew pairs of corresponding darts
    # and if they have different embedding  positions, merge them. 
*/
bool GMap::sew_dart(degree_t degree, id_t dart1, id_t dart2) // NEW 4.a et 4.b
{

    idlist_t list_orbit1, list_orbit2; // Liste des orbites 1 et 2
    bool res;                          // Le resultat booléen

    if (degree == 1) // Si le degr& vaut 1
    {
        res = link_darts(degree, dart1, dart2); // On tente directement le lien
    }
    else // Sinon
    {

        if (degree == 0) // Si degre = 0
        {                // On s'attarde sur les orbites de degrès 2 pour les lier (pour etre cohérents )
            list_orbit1 = orbit({2}, dart1);
            list_orbit2 = orbit({2}, dart2);
        }

        if (degree == 2) // Si degre = 2
        {                // On s'attarde sur les orbites de degrès 0 pour les lier (pour etre cohérents )
            list_orbit1 = orbit({0}, dart1);
            list_orbit2 = orbit({0}, dart2);
        }

        if (list_orbit1.size() != list_orbit2.size()) // On teste la taille des orbites, si pas égal
        {                                             // On ne lie pas et on retourne false
            return false;
        }

        for (size_t i = 0; i < list_orbit1.size(); i++) // Pour le nbr d'éléments  a lier
        {

            res = link_darts(degree, list_orbit1[i], list_orbit2[i]); // On lie l'élement de la liste 1 a celui de la liste 2
            if (!res)                                                 // Si ce liage échoue en renvoyant false, on return false;
                return false;                                         // Normalement il faudrait aussi unlink..
        }
    }

    return res;
}

// Compute the Euler-Poincare characteristic of the subdivision
int GMap::eulercharacteristic() const // NEW 5

{ // Version spéciale car on ne tavaille qu'avec des cubes dans ce tp
    size_t sommet,
        aretes, faces, dart_numbers;

    sommet = elements(0).size(); // recupère le nombre de structure 0cellule
    aretes = elements(1).size(); // recupère le nombre de structure 1cellule
    faces = elements(2).size();  // recupère le nombre de structure 2cellule
    std::cout << " sommet : " << sommet << " / aretes : " << aretes << " / faces : " << faces << " /  darts size : " << darts().size() << std::endl;
    return sommet - aretes + faces;
}

#endif
/*------------------------------------------------------------------------*/

/*
    Check if a dart of the orbit representing the vertex has already been 
    associated with a value in propertydict. If yes, return this dart, else
    return the dart passed as argument.
*/

template <class T>
GMap::id_t EmbeddedGMap<T>::get_embedding_dart(id_t dart) const // NEW 3
{
    idlist_t list_orbit = orbit({1, 2}, dart); // Recupère la liste des orbites de degré 1,2

    for (id_t current_id : list_orbit) // Pour toute les orbites
    {
        if (properties.count(current_id) != 0) // Si il existe une propriété sur l'élement courant
        {
            return current_id; // On retourne l'id de cet element
        }
    }
    return dart;
}

/*------------------------------------------------------------------------*/

#ifdef GMAP_CORE

GMap3D GMap3D::dual() // NEW 7
{

    GMap3D dual_gmap;                // Création d'un nouveau GMap3D : dual_gmap
    dual_gmap.maxid = maxid;         // On lui donne comme maxid le maxid courant de this
    dual_gmap.alphas = this->alphas; // On lui attribue tout ce que alphas de this contenait
    //dual_gmap.properties = this->properties;
    for (id_t dart : this->darts()) // Pour chaque dart on inverse les alpha 0 avec les 2 pour le dual
    {
        dual_gmap.alphas[dart][0] = this->alphas[dart][2];
        dual_gmap.alphas[dart][2] = this->alphas[dart][0];
    }

    for (id_t ele : this->elements(2)) // Pour toute les faces de this.
    {
        for (id_t id : orbit({0, 1}, ele)) // On parcours les faces de this via les orbit des 2-cellule
        {
            vec3_t pos = this->element_center(2, ele); // On determine les baricentres de ces points la
            dual_gmap.set_position(ele, pos);          // On les set dans dual_gmap
        }
    }
    return dual_gmap;
}

//NEW 8

// Comment passer de vertices a 2-Gmap? Ce que ça va couter et le gain que ça va donner

// Half-edge vs 2Gmap

// Liste de vertices vs 2Gmap

/*------------------------------------------------------------------------*/

#endif