#include <cstddef>
#include <vector>

#include <ncurses.h>
#include <cmath>
#include <iostream>
#include <immintrin.h>
using namespace std;
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <iomanip>
#include <algorithm>
#include <cfloat>
#include "robot.cpp"
#include <string>

char **genererSalle(int tailleSalle)
{
	char **M = new char *[tailleSalle];
	for (int i = 0; i < tailleSalle; i++)
		M[i] = new char[tailleSalle];

	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M[i][j] = 'x';
		}
	}
	return M;
}
void afficherSalle(char **M, int tailleSalle)
{
	for (int i = 0; i < tailleSalle; i++)
	{
		cout << "[ ";
		for (int j = 0; j < tailleSalle; j++)
		{
			if (j != tailleSalle - 1)
				cout << M[i][j] << " ";
			else
				cout << M[i][j];
		}
		cout << " ]" << endl;
	}
}
char **genererNewSalle(int tailleSalle)
{
	char **M = new char *[tailleSalle];
	for (int i = 0; i < tailleSalle; i++)
		M[i] = new char[tailleSalle];

	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			if (i == 0 || i == tailleSalle - 1)
				M[i][j] = 'M';
			else if (j == 0 || j == tailleSalle - 1)
				M[i][j] = 'M';
			else
				M[i][j] = ' ';
		}
	}
	return M;
}
void afficherNewSalle(char **M, int tailleSalle)
{
	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			std::string s;
			s.push_back(M[i][j]);

			mvprintw(i, j * 2, s.c_str());
		}
	}
	refresh();
}
char **genererCouloir(int lignesCouloir, int colonnesCouloir)
{
	char **M = new char *[lignesCouloir];
	for (int i = 0; i < lignesCouloir; i++)
		M[i] = new char[colonnesCouloir];

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			if (i == 0 || i == lignesCouloir - 1)
				M[i][j] = 'M';
			else if (j == 0 || j == colonnesCouloir - 1)
				M[i][j] = 'M';
			else
				M[i][j] = ' ';
		}
	}
	return M;
}
void afficherCouloir(char **M, int lignesCouloir, int colonnesCouloir)
{
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			std::string s;
			s.push_back(M[i][j]);
			mvprintw(i, j * 2, s.c_str());
		}
	}
	refresh();
}
void creerChaise(char **M, int tailleSalle, int x, int y)
{
	M[x][y] = 'c';
	// afficherSalle(M,tailleSalle);
}
void remplirSalle(char **M, int tailleSalle)
{
	srand(time(NULL));
	int random3 = rand() % tailleSalle + 1;
	for (int i = 0; i < random3; i++)
	{
		int random = rand() % tailleSalle + 1;
		int random2 = rand() % tailleSalle + 1;
		if (random == tailleSalle)
			random--;
		if (random2 == tailleSalle)
			random2--;
		// cout<<"Je cree une chaise aux coordonnees: ("<<random<<","<<random2<<")"<<endl;
		creerChaise(M, tailleSalle, random, random2);
	}
}
void remplirNewSalle(char **M, int tailleSalle)
{
	srand(time(NULL));
	int random3 = rand() % tailleSalle + 1;
	for (int i = 0; i < random3; i++)
	{
		int random = rand() % tailleSalle;
		int random2 = rand() % tailleSalle;
		if (random == tailleSalle - 1)
			random--;
		if (random2 == tailleSalle - 1)
			random2--;
		if (random == 0)
			random++;
		if (random2 == 0)
			random2++;
		// cout<<"Je cree une chaise aux coordonnees: ("<<random<<","<<random2<<")"<<endl;
		creerChaise(M, tailleSalle, random, random2);
	}
}

char **fusionDroiteCouloirSalle(char **S, int tailleSalle, char **C, int lignesCouloir, int colonnesCouloir)
{
	int lignesFusion = tailleSalle;
	int colonnesFusion = tailleSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];
	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}
	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = tailleSalle; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	int fus = (lignesCouloir - lignesCouloir % 2) / 2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 1; j < colonnesCouloir; j++)
		{
			M[i + fus][j + tailleSalle - 1] = C[i][j];
		}
	}
	for (int i = 0; i < lignesCouloir - 2; i++)
	{
		M[fus + 1 + i][tailleSalle - 1] = 'P';
	}
	return M;
}

char **fusionGaucheCouloirSalle(char **S, int tailleSalle, char **C, int lignesCouloir, int colonnesCouloir)
{
	int lignesFusion = tailleSalle;
	int colonnesFusion = tailleSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	int fus = (lignesCouloir - lignesCouloir % 2) / 2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i + fus][j] = C[i][j];
		}
	}

	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M[i][j + colonnesCouloir - 1] = S[i][j];
		}
	}
	for (int i = 0; i < lignesCouloir - 2; i++)
	{
		M[fus + 1 + i][colonnesCouloir - 1] = 'P';
	}

	return M;
}
char **fusionHautCouloirSalle(char **S, int tailleSalle, char **C, int lignesCouloir, int colonnesCouloir)
{
	int lignesFusion = tailleSalle + lignesCouloir - 1;
	int colonnesFusion = tailleSalle;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}

	// int fus = (colonnesCouloir-colonnesCouloir%2)/2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i][j + 1] = C[i][j]; // 1 arbitraire
		}
	}

	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M[i + lignesCouloir - 1][j] = S[i][j];
		}
	}

	for (int i = 0; i < colonnesCouloir - 2; i++)
	{
		M[lignesCouloir - 1][i + colonnesCouloir - 1] = 'P';
	}

	return M;
}
char **fusionBasCouloirSalle(char **S, int tailleSalle, char **C, int lignesCouloir, int colonnesCouloir)
{
	int lignesFusion = tailleSalle + lignesCouloir - 1;
	int colonnesFusion = tailleSalle;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i + tailleSalle - 1][j + 1] = C[i][j]; //1 arbitraire
		}
	}

	for (int i = 0; i < colonnesCouloir - 2; i++)
	{
		M[tailleSalle - 1][i + colonnesCouloir - 1] = 'P';
	}

	return M;
}
char **fusionDroiteNewCouloirSalle(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir, int indice)
{
	int lignesFusion = lignesSalle;
	int colonnesFusion = colonnesSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];
	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}
	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = colonnesSalle; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	// int fus = (lignesCouloir-lignesCouloir%2)/2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 1; j < colonnesCouloir; j++)
		{
			M[i + indice][j + colonnesSalle - 1] = C[i][j];
		}
	}
	for (int i = 0; i < lignesCouloir - 2; i++)
	{
		M[indice + 1 + i][colonnesSalle - 1] = 'P';
	}
	return M;
}
char **fusionGaucheNewCouloirSalle(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir, int indice)
{
	int lignesFusion = lignesSalle;
	int colonnesFusion = colonnesSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	// int fus = (lignesCouloir-lignesCouloir%2)/2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i + indice][j] = C[i][j];
		}
	}

	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j + colonnesCouloir - 1] = S[i][j];
		}
	}

	int indice2 = 0;
	int nbindice = 0;
	for (int i = 0; i < lignesSalle - 1; i++)
	{
		if (M[i][colonnesCouloir - 2] == 'M')
		{
			if (nbindice == 0)
				indice2 = i;
			nbindice++;
		}
	}
	M[indice2 + 1][colonnesCouloir - 1] = 'P';

	return M;
}
char **fusionHautNewCouloirSalle(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir, int indice)
{
	int lignesFusion = lignesSalle + lignesCouloir - 1;
	int colonnesFusion = colonnesSalle;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}

	// int fus = (colonnesCouloir-colonnesCouloir%2)/2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i][j + (colonnesSalle / 3) + indice - 1] = C[i][j]; // colonnesSalle/3 arbitraire
		}
	}

	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i + lignesCouloir - 1][j] = S[i][j];
		}
	}

	int indice2 = 0;
	int nbindice = 0;
	for (int i = 0; i < colonnesFusion - 1; i++)
	{
		if (M[lignesCouloir - 2][i] == 'M')
		{
			if (nbindice == 0)
				indice2 = i;
			nbindice++;
		}
	}
	M[lignesCouloir - 1][indice2 + 1] = 'P';

	return M;
}
char **fusionBasNewCouloirSalle(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir, int indice)
{
	int lignesFusion = lignesSalle + lignesCouloir - 1;
	int colonnesFusion = colonnesSalle;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i + lignesSalle - 1][j + indice] = C[i][j]; //indice (arbitraire)
		}
	}

	M[lignesSalle - 1][colonnesCouloir - 1] = 'P';

	return M;
}
char **fusionHautNewSalleCouloir(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir)
{
	int lignesFusion = lignesSalle + lignesCouloir - 1;
	int colonnesFusion = colonnesSalle;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}

	// int fus = (colonnesCouloir-colonnesCouloir%2)/2;
	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			M[i][j + (colonnesSalle / 3)] = C[i][j]; // colonnesSalle/3 arbitraire
		}
	}

	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			if (M[i + lignesCouloir - 1][j] == ' ')
				M[i + lignesCouloir - 1][j] = S[i][j];
		}
	}

	int indice = 0;
	int nbindice = 0;
	for (int i = 0; i < colonnesCouloir - 1; i++)
	{
		if (M[lignesCouloir - 1][i] == 'M')
		{
			if (nbindice == 0)
				indice = i;
			nbindice++;
		}
	}
	M[lignesCouloir - 1][indice + 1] = 'P';

	return M;
}
char **fusionDroiteNewSalleCouloir(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir)
{

	int lignesFusion = lignesSalle;
	int colonnesFusion = colonnesSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	for (int i = 0; i < lignesSalle - 1; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}

	int indice = 0;
	int nbindice = 0;
	for (int i = 0; i < lignesSalle - 1; i++)
	{
		if (S[i][colonnesSalle - 1] == 'M')
		{
			if (nbindice == 0)
				indice = i;
			nbindice++;
		}
	}

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			if (M[i + indice][j + colonnesSalle - 1] == ' ')
				M[i + indice][j + colonnesSalle - 1] = C[i][j];
		}
	}
	M[indice + 1][colonnesSalle - 1] = 'P';

	return M;
	/*
	for(int i = 0 ; i < lignesFusion; i++)
	{
		for(int j = 0 ; j < colonnesFusion ; j++)
		{
			M[i][j]='y';
		}
	}

	int indice=0;
	while(S[indice][colonnesSalle-1]!='M') {
		indice++;
	}
	indice--;
	for(int i = 0 ; i < lignesCouloir; i++)
	{
		for(int j = 0 ; j < colonnesCouloir ; j++)
		{
			M[i+indice-1][j+colonnesSalle-1]=C[i][j]; //indice = position nouvelle salle
		}
	}
	cout<<"Affichage Test 2"<<endl;
	afficherCouloir(M,lignesFusion,colonnesFusion);
	cout<<endl;
	cout<<"yep1"<<endl;
	for(int i = 0 ; i < lignesSalle ; i++)
	{
		for(int j = 0 ; j < colonnesSalle ; j++)
		{
			if(M[i][j]=='y') M[i][j]='z';//M[i][j]=S[i][j];
		}
	}
	cout<<"yep2"<<endl;
	int nbPortes=lignesCouloir-2;
	cout<<"yep3 "<<nbPortes<<endl;
	cout<<"Affichage Test 3"<<endl;
	afficherCouloir(M,lignesFusion,colonnesFusion);
	cout<<endl;
	for(int i = 0 ; i < nbPortes - 2 ; i++){
		M[i+indice][colonnesSalle-1]='P';
	}

	return M;
	*/
}
char **fusionBasNewSalleCouloir(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir)
{

	int lignesFusion = lignesSalle + lignesCouloir - 1;
	int colonnesFusion = max(colonnesSalle, colonnesCouloir);
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}
	for (int i = 0; i < lignesSalle - 1; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}

	int indice = 0;
	int nbindice = 0;
	for (int i = 0; i < colonnesSalle - 1; i++)
	{
		if (M[lignesSalle - 2][i] == 'M')
		{
			if (nbindice == 0)
				indice = i;
			nbindice++;
		}
	}

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			if (M[i + lignesSalle - 2][j + indice] == ' ')
				M[i + lignesSalle - 2][j + indice] = C[i][j];
		}
	}

	M[lignesSalle - 2][indice + 1] = 'P';

	return M;
}
char **fusionGaucheNewSalleCouloir(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir)
{

	int lignesFusion = max(lignesSalle, lignesCouloir);
	int colonnesFusion = colonnesSalle + colonnesCouloir - 1;
	char **M = new char *[lignesFusion];
	for (int i = 0; i < lignesFusion; i++)
		M[i] = new char[colonnesFusion];

	for (int i = 0; i < lignesFusion; i++)
	{
		for (int j = 0; j < colonnesFusion; j++)
		{
			M[i][j] = ' ';
		}
	}

	for (int i = 0; i < lignesSalle - 1; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j + colonnesCouloir - 1] = S[i][j];
		}
	}

	int indice = 0;
	int nbindice = 0;
	for (int i = 0; i < lignesSalle - 1; i++)
	{
		if (M[i][colonnesCouloir - 1] == 'M')
		{
			if (nbindice == 0)
				indice = i;
			nbindice++;
		}
	}

	for (int i = 0; i < lignesCouloir; i++)
	{
		for (int j = 0; j < colonnesCouloir; j++)
		{
			if (M[i + indice][j] == ' ')
				M[i + indice][j] = C[i][j];
		}
	}

	M[indice + 1][colonnesCouloir - 1] = 'P';
	return M;
}
char **fusionNewFinal(char **S, int lignesSalle, int colonnesSalle, char **C, int lignesCouloir, int colonnesCouloir, int orientation, int indice)
{
	if (orientation == 0)
		return fusionDroiteNewCouloirSalle(S, lignesSalle, colonnesSalle, C, lignesCouloir, colonnesCouloir, indice); // fusion droite
	if (orientation == 1)
		return fusionGaucheNewCouloirSalle(S, lignesSalle, colonnesSalle, C, lignesCouloir, colonnesCouloir, indice); // fusion gauche
	if (orientation == 2)
		return fusionHautNewCouloirSalle(S, lignesSalle, colonnesSalle, C, lignesCouloir, colonnesCouloir, indice); // fusion haut
	if (orientation == 3)
		return fusionBasNewCouloirSalle(S, lignesSalle, colonnesSalle, C, lignesCouloir, colonnesCouloir, indice); // fusion bas
	else
		return S;
}
char **testNewCarre(char **S, int lignesSalle, int colonnesSalle)
{
	int newDim = max(lignesSalle, colonnesSalle);
	char **M = new char *[newDim];
	for (int i = 0; i < newDim; i++)
		M[i] = new char[newDim];

	for (int i = 0; i < newDim; i++)
	{
		for (int j = 0; j < newDim; j++)
		{
			M[i][j] = 'y';
		}
	}

	for (int i = 0; i < lignesSalle; i++)
	{
		for (int j = 0; j < colonnesSalle; j++)
		{
			M[i][j] = S[i][j];
		}
	}
	return M;
}
void parcoursNaif(char **M, int tailleSalle)
{
	M[0][0] = 'r';
	afficherSalle(M, tailleSalle);
	char **M2 = new char *[tailleSalle];
	for (int i = 0; i < tailleSalle; i++)
		M2[i] = new char[tailleSalle];

	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			M2[i][j] = M[i][j];
		}
	}
	for (int i = 0; i < tailleSalle; i++)
	{
		for (int j = 0; j < tailleSalle; j++)
		{
			if (i % 2 == 0)
			{
				M[i][j] = 'r';
				if (j != 0)
					M[i][j - 1] = 'x';
				if (j == 0 && i != 0)
					M[i - 1][0] = 'x';
				cout << "robot aux coordonnees (" << i << "," << j << ")" << endl;
				afficherSalle(M, tailleSalle);
				cout << endl;
			}
			else
			{
				M[i][tailleSalle - 1 - j] = 'r';
				if (j != 0)
					M[i][tailleSalle - j] = 'x';
				if (j == 0)
					M[i - 1][tailleSalle - 1] = 'x';
				cout << "robot aux coordonnees (" << i << "," << j << ")" << endl;
				afficherSalle(M, tailleSalle);
				cout << endl;
			}
		}
	}
	cout << "Robot fin parcours";
	cout << endl;

	for (int i = tailleSalle - 1; i > 0; i--)
	{
		M[i][tailleSalle - 1] = 'x';
		M[i - 1][tailleSalle - 1] = 'r';
	}
	// cout<<"Robot mi retour"<<endl;
	// afficherSalle(M,tailleSalle);
	// cout<<endl;
	for (int i = tailleSalle - 1; i > 0; i--)
	{
		M[0][i] = 'x';
		if (i == 1)
			M[0][i] = 'c';
		M[0][i - 1] = 'r';
	}
	cout << "Robot fin retour" << endl;
	afficherSalle(M, tailleSalle);
	cout << endl;
}
