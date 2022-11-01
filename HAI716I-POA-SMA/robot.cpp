#include "robot.h"
#include <iostream>

#define VIDE ' '
int mod(int k, int n) {  return ((k %= n) < 0) ? k+n : k;  }

// Constructeur:
robot::robot(Position pos,char **Environnement) // On lui donne une position et l'Environnement (matrice de char)
{

    robot::current_pos = pos; // Pos courante = position de création;
    Environnement[pos.r][pos.c] = 'r';

    robot::empilement;        // A set uniquement en sortie de salle

    robot::nb_chairs_grabbed = 0; //Au debut il a aucune chaise sur le dos

    robot:: is_in_a_room = true; //Pour l'instant un robot sera forcément initialiser dans une salle

    checkpoint = Position {0,0};
    //Direction (sens anti horaire)
    robot::direction[0]=Position{1,0};//up 
    robot::direction[1]=Position{0,1};//right
    robot::direction[2]=Position{-1,0};//down
    robot::direction[3]=Position{0,-1};//left

    current_direction_id = 0; // On suppose que le robot regarde en haut a la création (voir current_di)


    //Field of view

    compute_direction(); // On recupte les direction en fonction de la position initiale

    behavior = 1;
}



// Mes triggers: 

char robot::front_trigger(char **Environnement){ // Retourne ce qu'on choppe en front de la position courante
    return Environnement[Front.r][Front.c];
}

char robot::left_trigger(char **Environnement){ // Retourne ce qu'on choppe en left de la position courante
    return Environnement[Left.r][Left.c];
}

char robot::long_right_trigger(char **Environnement){
        Position add_in_FOV = robot::Right; //Variable pour parcourir toute la ligne du FOV
        while (Environnement[add_in_FOV.r][add_in_FOV.c] == VIDE){ // Tant qu'on est en train de regarder du vide (qu'on a pas rencontrer d'obstacle)
        add_in_FOV = add(add_in_FOV,direction[mod(current_direction_id+1, 4)]); // On y ajoute le vecteur Right de taille 1
        //std::cout << add_in_FOV.r<< " "<< add_in_FOV.c <<" : "<<Environnement[add_in_FOV.r][add_in_FOV.c]  << std::endl;
        }
        // On a vu un truc
    return Environnement[add_in_FOV.r][add_in_FOV.c]; // On recupère ce que notre vue à collide
}


void robot::compute_direction(){ // Recompute les directions (après un rotate par exemple)
    robot::Front = add(current_pos,direction[mod(current_direction_id, 4)]); //On suppose qu'il va regarder vers le haut a son initialisation
    robot::Left = add(current_pos,direction[mod(current_direction_id-1, 4)]); // Sa gauche s'il regarde vers le haut
    robot::Right = add(current_pos,direction[mod(current_direction_id+1, 4)]);// Sa droite s'il regarde vers le haut
};

void robot::rotate(){
    robot::current_direction_id ++ ; // Changement de la direction courante en tourant a droite
    compute_direction(); // On recompute les directions 
};

void robot::grab_a_chair(char ** Environnement){
    robot::nb_chairs_grabbed++; // Une chaise de plus sur le dos
    Environnement[robot::Front.r][robot::Front.c] = VIDE; // On supprime la chaise de l'environnement
};

void robot::move(char **Environnement){
    //Environnement[current_pos.r][current_pos.c] == 'T' ? Environnement[current_pos.r][current_pos.c] = 'T':Environnement[current_pos.r][current_pos.c] = VIDE; // Suppression du r courant
    Environnement[current_pos.r][current_pos.c] = VIDE; // Suppression du r courant
   
    if (robot::front_trigger(Environnement) == VIDE) // Si la ou on doit aller y'a pas de soucis
        {
            current_pos = add(robot::current_pos,robot::direction[mod(current_direction_id,4)]); // Nouvelle position du robot en fct de sa direction courante
            Environnement[current_pos.r][current_pos.c] = 'r'; // Ecriture de r a la position courante
            compute_direction();
        }
    
    else if (robot::front_trigger(Environnement) == 'M' || robot::front_trigger(Environnement) == 'P' ) // Cas ou on est face a un mur ou d'une porte
        {
        robot::rotate(); // On tourne vers la droite
        //Environnement[current_pos.r][current_pos.c] == 'T' ? Environnement[current_pos.r][current_pos.c] = 'T': Environnement[current_pos.r][current_pos.c] = 'r'; // Ecriture de r a la position courante
        compute_direction();
        }

    else if (robot::front_trigger(Environnement) == 'c') // Cas ou on est face a une chaise
    {
        grab_a_chair(Environnement); // On recup cette chaise
        current_pos = add(robot::current_pos,robot::direction[mod(current_direction_id,4)]); // Nouvelle position du robot en fct de sa direction courante
        Environnement[current_pos.r][current_pos.c] = 'r'; // Ecriture de r a la position courante
        compute_direction();
        if(long_right_trigger(Environnement) == 'c'){
            behavior = 2;
        }
        
        //checkpoint = current_pos;
    }
    
    else if (robot::front_trigger(Environnement) == 'T'){
            current_pos = add(robot::current_pos,robot::direction[mod(current_direction_id,4)]); // Nouvelle position du robot en fct de sa direction courante
            Environnement[current_pos.r][current_pos.c] = 'T'; // Ecriture de r a la position courante
    }
};


void robot::drop_chairs(char ** Environnement){
    if (left_trigger(Environnement) == VIDE) // Si a gauche de la porte il y a rien
    {
        Environnement[Left.r][Left.c] = 'T'; // T pour tas de chaises
        nb_chairs_grabbed = 0;
    }
    else // Sinon on les pose a droite de la porte
    {
        Environnement[Right.r][Right.c] = 'T'; // T pour tas de chaises
        nb_chairs_grabbed = 0;
    }
};

void robot::pass_a_door(char ** Environnement){ 
    Environnement[current_pos.r][current_pos.c] = VIDE; // Suppression du r courant
    current_pos = add(robot::current_pos,robot::direction[mod(current_direction_id,4)]); // En avance de deux dans la direction souhaité
    // Marquage de la porte comme Pclean ici si necessaire en case (current_pos)
    current_pos = add(robot::current_pos,robot::direction[mod(current_direction_id,4)]);
    Environnement[current_pos.r][current_pos.c] = 'r'; // Ecriture de r a la position courante
    compute_direction();
};

void robot::next_move(char **Environnement){
    // Behavior 1 : parcours tranquillou en longeant les murs
    if(behavior == 1){
        if (checkpoint == Position {0,0} && front_trigger(Environnement) == 'M'){
            checkpoint = current_pos;
        }
        if (long_right_trigger(Environnement) == 'c') // Si on regarde une chaise
        {
           ////std::cout<<"on a vu une chaise"<<std::endl;
            //std::cout<<"rotating : "<<"Front old : "<< Front.r <<","<< Front.c<<std::endl;
            rotate(); // On fait aligner le robot avec la cible
            //std::cout<<"rotating : "<<"Front new : "<< Front.r <<","<< Front.c<<std::endl;
            robot::behavior = 2; // On passe le behavior a 2
            //robot::checkpoint = robot::current_pos;
        }
        else // Sinon
        {
           // //std::cout<<"going to move "<<std::endl;
            move(Environnement); // On fait juste un deplacement
            compute_direction();
           // //std::cout<<"moved"<<std::endl;
            if (checkpoint == current_pos) // Si lorsqu'on avance on retombe sur une case checkpoint, c'est qu'on a fait le tour de la salle sans retrouver de chaise, on a donc fini
            {
                behavior = 4;
            }
        }
    }
    // Behavior 2 : J'ai trigger une chaise en longeant les murs
    else if(behavior == 2){
       // //std::cout <<" On est dans behavior 2 "<<std::endl;
        //show_direction(Environnement);
       if(front_trigger(Environnement) != 'c') // Tant qu'on a pas une chaise collé en front
        {
           // //std::cout<<"current pos "<<current_pos.r<<","<<current_pos.c<<std::endl;
            move(Environnement); // On avance
        }
        else{
       // //std::cout<<"Grab a chair"<<std::endl;
        grab_a_chair(Environnement);
        rotate(); // On fait demi tour (deux rotate vers la droite)
        rotate();
        robot::behavior = 3;
        }
    }
    // Behavior 3 : J'ai pris la chaise via le comportement 2, je fais demi-tour vers le checkpoint au mur
    else if (behavior == 3){
        if (front_trigger(Environnement) != 'M'){ // Tant qu'il n'est pas de retour collé au mur au checkpoint
            move(Environnement);
            compute_direction();
        }
        else{
       // //std::cout<<"rotating : "<<"Front old : "<< Front.r <<","<< Front.c<<std::endl;
        rotate(); // On fait aligner le robot avec la cible
        compute_direction();
       // //std::cout<<"rotating : "<<"Front new : "<< Front.r <<","<< Front.c<<std::endl;
        behavior = 1;

        
    }
    }
    // Behavior 4 : J'ai fini mon netooyage, je cherche maintenant une porte pour sortir vers un couloir
    else if (behavior == 4){
        //std::cout<<"behave = "<< 4 <<" and "<< left_trigger(Environnement) <<std::endl;
        
        if (left_trigger(Environnement) != ('P')){ // Tant qu'on a pas une porte a notre gauche
            move(Environnement);
        }
        else{
        //std::cout<<" en face de la porte"<<std::endl;
        rotate(); // Trois rotate pour passer a gauche de la ou on regarde et donc s'aligner avec la porte
        rotate();
        rotate();
        drop_chairs(Environnement); // Deposer les chaises
        Environnement[Front.r][Front.c] = 'p';
        pass_a_door(Environnement); // Passage de la porte
        is_in_a_room = false; //On quitte la salle
        behavior = 5;
        }
    }
    // Behavior 5 : Je suis dans un couloir, je cherche une porte pour retourner dans une salle afin de la nettoyer
    else if (behavior == 5){
        checkpoint = Position{0,0};
        if(front_trigger(Environnement) == (VIDE)){ // Tant qu'on a pas une porte en front
            move(Environnement);
        }
        else { // On est en face d'une porte
        front_trigger(Environnement) == 'P'? behavior = 1 : behavior = 6;
        //behavior = 1;
        pass_a_door(Environnement); // On arrive donc dans une Salle
        is_in_a_room = true;
        rotate();// On se tourne verse la gauche
        rotate();
        rotate();
        }
    }
    else if (behavior == 6){ // Viens dans une salle propre
        if(left_trigger(Environnement) != ('P')){
                move(Environnement);
        }
        else{
        rotate();
        rotate();
        rotate();
        Environnement[Front.r][Front.c] = 'p';
        pass_a_door(Environnement); // On arrive donc dans une Salle
        behavior = 5;
        }
       /* if(left_trigger(Environnement) != ('P')) // Tant qu'on ne trouve pas une porte 'P'
        {
            move(Environnement);
        }
        rotate();
        rotate();
        rotate();// On se tourne verse la gauche
        rotate();
        rotate();
        Environnement[Front.r][Front.c] = 'p';
        pass_a_door(Environnement); // On arrive donc dans une Salle
        is_in_a_room = false;
        behavior = 5 ; */
        
    }
};

void robot::show_direction(char **Environnement){
    Environnement[Left.r][Left.c] = 'L';
    //Environnement[Right.r][Right.c] = 'R';
    Environnement[Front.r][Front.c] = 'U';
    int r,c;
    Position Right_bis = Right;
    while(Environnement[Right_bis.r][Right_bis.c] == VIDE){
        Environnement[Right_bis.r][Right_bis.c] = 'o'; // representation de la vue
        Right_bis = add (Right_bis,direction[mod(current_direction_id+1, 4)]);
    }
    Environnement[Right_bis.r][Right_bis.c] = 'H'; // Ce qu'on touche
    
};