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
    std::vector<Position> FOV; // Vue jusqu'au mur en direction de la salle (arbitraitement on regarde a droite)

    // Notion de direction
    Position direction[4];    // Tab des 4 directions, changer a chaque bump
    int current_direction_id; // Indice de la direction courante (mod 4)
    // Booléen d'état, utilisé pour les comportement

    // A vu une chaise, a update
    bool currently_triggered;             // True si il a été trigger par une chaise
    Position before_running_toward_chair; // Position avant d'aller chopper la chaise afin de pouvoir revenir sur ses pas et de tester si on a fait un tour complet
                                          // On le set a current_postion lorsque l'on trigger une chaise pendant le parcours sur les bords de la salle.
    int nd_chairs_grabbed;                // Nombre de chaises actuellement sur le robot;

    bool is_in_a_room; // True si on est dans une salle, False dans le couloir (chanquement a chaque passage d'une porte)

public:
    // Constructeur :
    robot();
    robot(Position pos);

    // Méthodes:
    // Trigger -> position
    char trigger_front(char **Environnement); // Check si y'a un objet en facade (peut importe sa nature)
    char trigger_left(char **Environnement);  // Check si y'a un objet a gauche

    void trigger_long_right_chair(char **Environnement); // Methode appelé lorsque le robot avance sur les bords
                                                         // Check sur FOV si il y a une chaise, si oui, set a true
                                                         // Set aussi Position before running toward chair a curren_pos

    void rotate(); // compute la direction (+1 a l'id)

    void move(char **Environnement); // Deplacement qui va
                                     //  Supprimer le r de la position d'avant
                                     //  Tester si dans la direction[current_direction_id] il ya un mur
                                     //  Si pas obstacle : compute current_pos, front, left,right , FOV;
                                     //  Si obstacle de type mur :  reotate();
                                     // Si obstacle de type porte (P_close ou P_open) : pass_a_door();
                                     //  puis on rappelle move();
                                     // enfin on ecrit r sur la case courante

    void pass_a_door(char **Environnement); // Appeler si on essaye de move sur une porte
                                            // set_is_in_room a l'nverse de sa valeur
                                            //  Si la porte est P_close; on la transforme en P_open
                                            //  se mettre sur la porte et compute comme si on était sur la porte
};
