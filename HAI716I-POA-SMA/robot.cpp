#include "robot.h"
#include "Position.cpp"

// Constructeur:
robot::robot(Position pos,char **Environnement) // On lui donne une position et l'Environnement (matrice de char)
{

    robot::current_pos = pos; // Pos courante = position de création;
    Environnement[pos.c][pos.r] = 'r';

    robot::empilement;        // A set uniquement en sortie de salle

    robot::nb_chairs_grabbed = 0; //Au debut il a aucune chaise sur le dos

    robot:: is_in_a_room = true; //Pour l'instant un robot sera forcément initialiser dans une salle


    //Direction (sens anti horaire)
    robot::direction[0]=Position{1,0};//up 
    robot::direction[1]=Position{0,1};//right
    robot::direction[2]=Position{-1,0};//down
    robot::direction[3]=Position{0,-1};//left

    current_direction_id = 0; // On suppose que le robot regarde en haut a la création (voir current_di)


    //Field of view

    compute_direction(); // On recupte les direction en fonction de la position initiale

    in_view = long_right_trigger(Environnement); // Appel a long_right_trigger pour savoir ce qu'on a en vue même lors de la création (car il pourra avoir une chaise aligné)

    in_view == 'c' ? currently_triggered = true : currently_triggered = false; //En fonction de ce qu'on voit on change le comportement
    currently_triggered? behavior = 1 : behavior = 2; 
}



// Mes triggers: 

char robot::front_trigger(char **Environnement){ // Retourne ce qu'on choppe en front de la position courante
    Position front_pos = add(current_pos,Front);
    return Environnement[front_pos.r][front_pos.c];
}

char robot::left_trigger(char **Environnement){ // Retourne ce qu'on choppe en left de la position courante
    Position left_pos = add(current_pos,Left);
    return Environnement[left_pos.r][left_pos.c];
}

char robot::long_right_trigger(char **Environnement){
        Position add_in_FOV = robot::Right; //Variable pour parcourir toute la ligne du FOV
        while (Environnement[add_in_FOV.r][add_in_FOV.c] == 'x'){ // Tant qu'on est en train de regarder du vide (qu'on a pas rencontrer d'obstacle)
        add_in_FOV = add(add_in_FOV,Right); // On y ajoute le vecteur Right de taille 1
        }
        // On a vu un truc
        Environnement[add_in_FOV.r][add_in_FOV.c] == 'c'? currently_triggered = true: currently_triggered = false;
    return Environnement[add_in_FOV.r][add_in_FOV.c]; // On recupère ce que notre vue à collide
}


void robot::compute_direction(){ // Recompute les directions (après un rotate par exemple)
    robot::Front = add(current_pos,direction[current_direction_id % 4]); //On suppose qu'il va regarder vers le haut a son initialisation
    robot::Left = add(current_pos,direction[current_direction_id-1 % 4]); // Sa gauche s'il regarde vers le haut
    robot::Right = add(current_pos,direction[current_direction_id+1 % 4]);// Sa droite s'il regarde vers le haut
}

void robot::rotate(){
    robot::current_direction_id = (current_direction_id + 1) % 4; // Changement de la direction courante en tourant a droite
    compute_direction(); // On recompute les directions 
};

void robot::grab_a_chair(char ** Environnement){
    robot::nb_chairs_grabbed++; // Une chaise de plus sur le dos
    Environnement[robot::Front.r][robot::Front.r] = 'x'; // On supprime la chaise de l'environnement
};

void robot::move(char **Environnement){
    Environnement[current_pos.r][current_pos.c] = 'x'; // Suppression du r courant
    if (robot::front_trigger(Environnement) == 'x') // Si la ou on doit aller y'a pas de soucis
        {
            current_pos = add(robot::current_pos,robot::direction[current_direction_id]); // Nouvelle position du robot en fct de sa direction courante
            Environnement[current_pos.r][current_pos.c] = 'r'; // Ecriture de r a la position courante
        }
    else if (robot::front_trigger(Environnement) == 'M' || 'P') // Cas ou on est face a un mur ou d'une porte
        {
        robot::rotate(); // On tourne vers la droite
        move(Environnement); // On retente de move dans cette nouvelle direction
        }
    else if (robot::front_trigger(Environnement) == 'c') // Cas ou on est face a une chaise
    {
        grab_a_chair(Environnement); // On recup cette chaise et on retry un move sur cette case
        move(Environnement);
    }
};

void robot::next_move(char **Environnement){
    // Behavior 1 : parcours tranquillou en longeant les murs
    
    if(behavior == 1){
        move(Environnement);
    }
    // Behavior 2 : J'ai trigger une chaise
    if(behavior == 2){
        rotate(); // On tourne a droite, ce qui va nous aligner avec le 
        move(Environnement);
    }
    // Behavior


}


/*
char trigger_Voisinage(char **Map)
{
    char pos_trig;
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

x = vide;
c = chaise;
r = robot;
p = porte;
Position[2][nb_couloir]
Position[2][nb_salle]
M = mur;


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
  direction
    u = up (1,0)
    d = down; (-1,0)
    l = left; (0,-1)
    r = right;(0,1)

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
}*/