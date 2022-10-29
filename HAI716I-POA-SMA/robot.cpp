#include "robot.h"
#include "Position.cpp"

// Definition du constructeur:
robot::robot(Position pos)
{

    robot::current_pos = pos; // Pos courante = position de création;
    robot::empilement;        // Voir comment le set a chaque entrée dans une salle

    // Set le voisinage -> 0 == en bas a droite et parcours dans le sens anti horaire:
    robot::Voisinage[0] = Position{current_pos.r - 1, current_pos.c - 1};
    robot::Voisinage[1] = Position{current_pos.r - 1, current_pos.c};
    robot::Voisinage[2] = Position{current_pos.r - 1, current_pos.c + 1};
    robot::Voisinage[3] = Position{current_pos.r, current_pos.c - 1};
    robot::Voisinage[4] = Position{current_pos.r, current_pos.c + 1};
    robot::Voisinage[5] = Position{current_pos.r + 1, current_pos.c - 1};
    robot::Voisinage[6] = Position{current_pos.r + 1, current_pos.c};
    robot::Voisinage[7] = Position{current_pos.r + 1, current_pos.c + 1};

    // Set le FOV des 4 directions:
    Position direction;
    for (int i = 0; i < 4; i++)
    { // Pour chaqu'une des 4 direction
        // UP
        if (i == 0)
            direction == Position{(1, 0)};
        // DOWN
        if (i == 1)
            direction == Position{(-1, 0)};
        // LEFT
        if (i == 2)
            direction == Position{(0, -1)};
        // RIGHT
        if (i == 3)
            direction == Position{(0, 1)};

        for (int j = 1; j < 4; j++) // Pour une vue de portée 3
        {                           // On ajoute a la position courante le decalage j * la direction d et cela 3 fois par direction
            FOV[i * j + i] = Position{current_pos.r + direction.r * j, current_pos.c + direction.r * c};
        }
    }
}

// Methodes :

// Trigger_voisinage;
Position trigger_Voisinage(char **Map)
{
    Position pos_trig;
    for (Position view_pos : robot::Voisinage) // Pour toute les positions de mon voisinage
    {
        if (M[view_pos.r] != 'x')
    }
    return pos_trig;
};
// Trigger_chair;

void trigger_chair
{
    for (Position view_pos : robot::Voisinage)
    { // Pour chaque position du voisinage
    }
            for each
                "field of view" : if (Mat_total[pos] == "c")
                                      currently_triggered = true;

            // vector['c'].add(pos);

            break;
}
/*
x = vide;
c = chaise;
r = robot;
p = porte;
Position[2][nb_couloir]
Position[2][nb_salle]
M = mur;
*/

// Environnement courant -> Mat_total[i,j];

// Position empilement
// empilement initialiser cst

// Constructeur :
//  cst_agent(Position _pos ,TransfertFunction _tp){

//   empilement // case d'empilement proche de la porte
//}

// Constructeur :

cst_agent(Position _pos)
{

    // Attribut :
    Position current_pos; // Position courante du robot

    Position empilement;   // case d'empilement proche de la porte
                           // field of view :
    Position[8] Voisinage; // Voisinage via current_pos
    Position[3] FOV_up;    // current_pos
    Position[3] FOV_down;
    Position[3] FOV_left;
    Position[3] FOV_right;

    bool currently_triggered; //
    bool hold_a_chair;
    // vector<Position>[nb_objet]

    // Methodes :

    void trigger
    {
            for each
                "field of view" : if (Mat_total[pos] == "c")
                                      currently_triggered = true;

            // vector['c'].add(pos);

            break;
    }

    // Appel constant tant que currently_triggered == true;
    void grab_a_chair
    {
        si une des cases de mon voisinage == pos(position obtenue lors du trigger et stocké)
                                                 currently_triggered = false;
        hold_a_chair = true;
    }
    /* direction
    u = up (1,0)
    d = down; (-1,0)
    l = left; (0,-1)
    r = right;(0,1)
    */
    // direciton sera generé aléatoirement;
    void moove(direction d)
    {
        if (Voisinage[current_pos + d] == 'x')
        // Fait mon move
    }

    bool
    isVide(Environement env, Position _pos)
    {
        env.isVide(_pos.geti(), _pos.getj());
    }
    void
    moove()
    {
    }

    // si objet devant moi  ;
    // tant que objet devant moi deplace sur cote
    // deplace dans direction
    void
    mooveDirection(Position d)
    {

        for (Position d1 = new Position(d); getBump() || !isVide(Voisinage, new Position(pos.geti() + d1.geti.(), pos.getj() + d1.getj.()));
             d.equals(Direction.up) ? d1.setj(d1.getj() + 1) : d1.seti(d1.geti() + 1))
        {
            pos = new Position(d1);
        }
        if (getBump())
        {
            for (Position d1 = new Position(d); getBump() || !isVide(Voisinage, new Position(pos.geti() + d1.geti.(), pos.getj() + d1.getj.()));
                 d.geti() != 0 ? d1.setj(d1.getj() - 1) : d1.seti(d1.geti() - 1))
            {
                pos = new Position(d1);
            }
        }
        if (getBump())
        {
            return EXIT_FAILURE;
        }
        d.geti() != 0 ? pos.seti(pos.geti() + d.geti()) : pos.setj(pos.getj() + d.getj());
    }

    class Environement
    {
        char **matrix; // std::array<std::array<char , sz_col> ,sz_row >matrix;
        size_t sz_row, sz_col;
        class Alphabet
        {
            bool isChaise(int i, int j)
            {
                return matrix[i][j] == 'c';
            }
            bool isPorte(int i, int j)
            {
                return matrix[i][j] == 'P';
            }
            bool isVide(int i, int j)
            {
                return !(isPorte(i, j) || isChaise(i, j));
            }

        } Alphabet alphabet;

        Environement(char **matrix, const size_t sz_col, const size_t sz_row)
        {

            memcpy(this.matrix, matrix, sizeof(char) * sz_col * sz_row);
            this.sz_col = sz_col;
            this.sz_row = sz_row;
        }

        bool isChaise(int i, int j)
        {
            return alphabet.isChaise(i, j);
        }
        bool isPorte(int i, int j)
        {
            return alphabet.isPorte(i, j);
        }
        bool isVide(int i, int j)
        {
            return alphabet.isVide(i, j);
        }

    }

    class Position
    {
        int i, j;

        Direction()
        {
            this.i = 0;
            this.j = 0;
        }

        Direction(Position _pos)
        {
            this.i = _pos.geti();
            this.j = _pos.getj();
        }

    }

    void
    triggerResponse()
    {

        int sign_i = (pos.geti() < emplacement.geti() ? 1 : -1);
        for (; pos.geti() != emplacement.geti(); agent.mooveDirection(new Position(0, sign_i)))
            ;
        int sign_j = (pos.getj() < emplacement.getj() ? 1 : -1);
        for (; pos.getj() != emplacement.getj(); agent.mooveDirection(new Position(sign_j, 0)))
            ;
    }

    // 3 behavior :
    1 - J 'ai pas de chaise et j' ai rien de trigger - next move->bouger random(si possible);
    2 - J 'ai un trigger (vu une chaise) - next move->bouger vers la position qui a causer le trigger; 3 - J ' ai une chaise - next move->aller vers la position d' empilement;
}