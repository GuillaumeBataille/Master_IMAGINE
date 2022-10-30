#include "Position.cpp"
#include <vector>
class robot
{
private:
    // Position courante et d'empilement;
    Position current_pos; // Position courante du robot
    Position empilement;  // case d'empilement proche de la porte

    // field of view :
    Position Front;            // Position en face du robot (la ou il va avancer)
    Position Left;             // Position a sa gauche (utile pour check ou est la porte)
    Position Right;            // Position a sa droite // USELESS
    char in_view;              // Ce qui est en vue (arbitraitement on regarde a droite)

    // Notion de direction
    Position direction[4];    // Tab des 4 directions, changer a chaque bump
    int current_direction_id; // Indice de la direction courante (mod 4)
    // Booléen d'état, utilisé pour les comportement

    // A vu une chaise, a update
    bool currently_triggered;             // True si il a été trigger par une chaise
    Position checkpoint; // Position avant d'aller chopper la chaise afin de pouvoir revenir sur ses pas et de tester si on a fait un tour complet
                                          // On le set a current_postion lorsque l'on trigger une chaise pendant le parcours sur les bords de la salle.
    int nb_chairs_grabbed;                // Nombre de chaises actuellement sur le robot;

    bool is_in_a_room; // True si on est dans une salle, False dans le couloir (chanquement a chaque passage d'une porte)

    int behavior; // 
public:
    // Constructeur :
    robot(Position pos, char ** Environnement);

    // Méthodes:
    // Trigger -> position
    char front_trigger(char **Environnement); // Check si y'a un objet en facade (peut importe sa nature)
    char left_trigger(char **Environnement);  // Check si y'a un objet a gauche

    char long_right_trigger(char **Environnement); // Methode appelé lorsque le robot avance sur les bords
                                                         // Check sur FOV si il y a une chaise, si 
                                                         // Set aussi Position before running toward chair a curren_pos

    void rotate(); // compute la direction (+1 a l'id)

    void move(char **Environnement); // Deplacement qui va
                                     //  Supprimer le r de la position d'avant //DONE
                                     //  Tester si dans la direction[current_direction_id] il ya un mur//DONE
                                     //  Si pas obstacle : compute current_pos, front, left,right , FOV;//DONE
                                     //  Si obstacle de type mur :  reotate() + recompute ;
                                     // Si obstacle de type porte (P_close ou P_open) : pass_a_door();
                                     //  puis on rappelle move();
                                     // enfin on ecrit r sur la case courante

    void compute_direction(); // Recompute les direction front, left et right en fonction d'une nouvelle position courante
    void grab_a_chair(char **Environnement);

    void pass_a_door(char **Environnement); // Appeler si on essaye de move sur une porte
                                            // set_is_in_room a l'nverse de sa valeur
                                            //  Si la porte est P_close; on la transforme en P_open
                                            //  se mettre sur la porte et compute comme si on était sur la porte
    void next_move(char ** Environnement);
};
