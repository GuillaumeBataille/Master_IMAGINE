#include "projet.cpp"

int main()
{	
	initscr();

	int tailleSalle = 6;
	int lignesCouloir = 5;
	int colonnesCouloir = 5;
	int lignesCouloir2 = 5;
	int colonnesCouloir2 = 3;


	int tailleNewSalle = tailleSalle + 1;
	char **M3 = genererNewSalle(tailleNewSalle);
	remplirNewSalle(M3, tailleNewSalle);

	char **Mtest = genererCouloir(lignesCouloir2, colonnesCouloir2);

	char **M7 = fusionHautCouloirSalle(M3, tailleNewSalle, Mtest, lignesCouloir2, colonnesCouloir2);

	char **M8 = fusionBasCouloirSalle(M3, tailleNewSalle, Mtest, lignesCouloir2, colonnesCouloir2);

	char **Mtest2 = genererCouloir(3, 4);

	char **M9 = fusionDroiteNewCouloirSalle(M8, tailleNewSalle + lignesCouloir2 - 1, tailleNewSalle, Mtest2, 3, 4, 1);

	char **M11 = fusionGaucheNewCouloirSalle(M9, tailleNewSalle + lignesCouloir2 - 1, tailleNewSalle + 4 - 1, Mtest2, 3, 4, 1);

	char **M12 = fusionHautNewCouloirSalle(M11, tailleNewSalle + lignesCouloir2 - 1, tailleNewSalle + 4 + 4 - 1, Mtest2, 3, 4, 1);

	char **M13 = fusionHautNewSalleCouloir(M12, tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, tailleNewSalle + 4 + 4 - 1 - 1, M3, tailleNewSalle, tailleNewSalle);

	char **M14 = fusionDroiteNewSalleCouloir(M13, 2 * tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, tailleNewSalle + 4 + 4 - 1 - 1, M3, tailleNewSalle, tailleNewSalle);

	char **M15 = fusionBasNewSalleCouloir(M14, 2 * tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, 2 * tailleNewSalle + 4 + 4 - 1 - 1, M3, tailleNewSalle, tailleNewSalle);

	char **M16 = fusionGaucheNewSalleCouloir(M15, 3 * tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, 2 * tailleNewSalle + 4 + 4 - 1 - 1, M3, tailleNewSalle, tailleNewSalle);

	robot my_robot = robot(Position{9,10}, M16);

    //Affichage initial
    afficherCouloir(M16, 3 * tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, 3 * tailleNewSalle + 4 + 4 - 1 - 1);
	
    while(true){
	my_robot.next_move(M16);
	afficherCouloir(M16, 3 * tailleNewSalle + lignesCouloir2 - 1 + 3 - 1, 3 * tailleNewSalle + 4 + 4 - 1 - 1);
    
    std::string behavior;
    behavior.push_back(my_robot.behavior+48);
    mvprintw(2,0,behavior.c_str());
    refresh();
	usleep(1000000/2);
	/*std::cout<< " Position du robot : "<< my_robot.current_pos.r<<"," <<my_robot.current_pos.c <<std::endl;	    Position current_pos; // Position courante du robot
	std::cout<<" Comportement de type : "<< my_robot.behavior <<std::endl;
	std::cout <<" Front : "<< my_robot.front_trigger(M16)<<" en "<<my_robot.Front.r<<","<<my_robot.Front.c<<std::endl;
	std::cout <<" Current direction id : "<< my_robot.current_direction_id <<std::endl;
	std::cout <<" Chair : "<< my_robot.nb_chairs_grabbed <<std::endl;*/
	}

	getch(); 
	endwin();
	return 0;
}