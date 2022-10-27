#include "EvalPerf.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>


// EXERICE 4 : Créer la structure de test, ma fonction et flop_ma_foncton
void ma_fonction(int x){ //Exo4

unsigned int a = 0;
    for (size_t i= 0; i<x; i++){
        for (size_t j= 0; j<x; j++){
            a= a + 1 ;
        }

    }
};

int flop_ma_fonction1(int x){ // 3* pour chaque addition en se basant sur l'architecture Haswell.
return 3*x*3*x*2*3;
};
/* Resultat exo 4 avec n = 10 000
nbr cycle : 636 012 279
nbr secondes : 0.303427
nbr millisecond : 303.427
CPI : 0.57556
IPC : 1.73744

 Performance counter stats for './tp':

       709 807 272      cycles

       0,306336548 seconds time elapsed

       0,303663000 seconds user

Meme ordre de grandeur dans les cycle et même 
*/
// EXERCICE 4 - FIN

///////////////////////////////////////////////////////////////////////////////


// EXERCICE 5 : Préfixe 

std::vector<double> pref(std::vector<double> &T){
    size_t taille = T.size();
    for(size_t i=1; i<taille; i++){
        T[i]+= T[i-1];
    }
    return T;
}

int flop_prefixe(int x){
return x*x*3;
};
/*  Moyenne de 3 resultat pour chaque test

    Resultat moyen préfixe avec n=10 000 000 sans option 
CPI : 0.35297
IPC : 2.84473

    Resultat moyen préfixe avec n=10 000 000 avec O0
CPI : 0.364577
IPC : 2.74291

    Resultat moyen préfixe avec n=10 000 000 avec O1
CPI : 0.37803
IPC : 2.64529

    Resultat moyen préfixe avec n=10 000 000 avec O2
CPI : 0.424149
IPC : 2.35766

    Resultat moyen préfixe avec n=10 000 000 avec O3
CPI : 0.431129
IPC : 2.31949

// On constate que O0 est le meilleur pour avoir un bon IPC la ou O3 est le pire (même si sans les moyennes les resultats se contredisent)
*/
// EXERCICE 5 : FIN


/////////////////////////////////////////////////////////////

//  EXERCICE 6 : Polynome

//Fonction puissance 

int power(int x, int n)
{
    if (n != 0)
        return (x*power(x, n-1));
    else
        return 1;
};

double eval_poly1(std::vector<double> &T, double x){
    size_t taille = T.size();
    double res = 0;
    for(size_t i=0; i<taille; i++){
        res  += T[i] * power(x,i);
    }
    std::cout<<"respoly1 : "<< res<<std::endl;

    return res; 
}
/* Nbr operation 
(taille T * 2) car i++ et une multiplication  et dans power on a i multiplication supplémentaire

Test avece taille de T = 10 000 et x= 9
CPI : 0.680555
IPC : 1.46939

J'ai essayé avec 100 000 pour taille de T mais ça rame trop pour me donner un resultat en moins de 50 sec
*/
int flop_eval_poly1(int x){
return x*x*3*3;
};


double eval_poly2(std::vector<double> &T, double x){
    size_t taille = T.size();
    double Q = T[0];
    for(size_t i=1; i<taille; i++){
        Q=(Q*x)+T[i];
    }
    std::cout<<"respoly2 : "<< Q<<std::endl;
    return Q; 
}
/* Nbr operation 
((taille T -2 )* mult * add*2 
*/
int flop_eval_poly2(int x){
return (x-2)*5*3*2;
};

/*
Test avec taille de T = 10 000 et x =9

CPI : 0.38409
IPC : 2.60356

Test avec taille de T = 100 000 et x =9
CPI : 0.822698
IPC : 1.21551


Comparaison des resultat pour taille T = 100 : 
respoly2 : 8.71696e+08
respoly1 : 8.71696e+08
Donc la fonction fait bien ce qu'on attend d'elle 

*/


 //   EXERCICE 6 : FIN

 // EXERCICE 7 : Reduce +/x 



#define OP * // +
#define START 1 // 0

// Version 0

template <typename T>
void reduce0 (const std::vector <T> &V , T &res) {
res = START;
for (size_t i = 0; i < V.size();i++){
    res = res OP V[i];
}
};
// OP : i++, V.size(),res*V[i] a chaque iteration -->  (1+(taille)+1) * taille
int flop_reduce0(int x){
return (2+x)*x;
};
/* Test avec taille T 10 000 000
CPI : 0.474616
IPC : 2.10696
*/

// Version 1 : Sans recalcul de size()
template <typename T>
void reduce1 (const std::vector <T> &V , T &res) {
res = START;
unsigned int taille = V.size(); //Empecher le recalcul a chaque fois de size dans la boucle for

for (size_t i = 0; i < taille;i++){ 
    res = res OP V[i];
}
};
// OP : i++,res*V[i] a chaque iteration et V.size()--> 2* taille + taille
int flop_reduce1(int x){
return  x + 2*x;
};
/*Test avec taille T 10 000 000 
CPI : 0.471194
IPC : 2.12227
Resultat obtenu avec flopt_reduce0 car avec flop_reduce1 j'ai des resultat incohérent et je ne sais pas pourquoi..
*/


// Version 2 : double parcours (supposons taille paire pour simplifier)
template <typename T>
void reduce2 (const std::vector <T> &V , T &res) {
T res_pair = START;
T res_impair = START;
unsigned int taille = V.size();

for (size_t i = 0; i < taille;i+=2){ //i prends +2 a chaque fois 
    res_pair = res OP V[i];
    res_impair = res OP V[i+1];
}
res = res_pair + res_impair;
};
// OP : iteration / 2 et i+=2,res_pair, res_impair pour chaque iteration et hors iteration res et V.size() --> (taille/2) * (1 + 1 + 1) +  taille + 1 
int flop_reduce2(int x){
return (x/2) *3 + x+1;
};
/*Test avec taille T 10 000 000
CPI : 0.263199
IPC : 3.7994
Presque x2 ! 
Resultat obtenu avec flopt_reduce0 car avec flop_reduce2 j'ai des resultat incohérent et je ne sais pas pourquoi..
*/


// Version 3 : quadruple parcours (supposons taille paire pour simplifier)
template <typename T>
void reduce3 (const std::vector <T> &V , T &res) {
T res_0 = START;
T res_1 = START;
T res_2 = START;
T res_3 = START;
unsigned int taille = V.size();

for (size_t i = 0; i < taille;i+=4){ //i prends +2 a chaque fois 
    res_0 = res OP V[i];
    res_1 = res OP V[i+1];
    res_2 = res OP V[i+2];
    res_3 = res OP V[i+3];

}
res = res_0+res_1+res_2+res_3;
};
// OP : iteration / 4 et i+=4,res_0+res_1+res_2+res_3 pour chaque iteration et hors iteration res_0+res_1+res_2+res_3 et V.size() --> (taille/4) * (1 + 1 + 1 + 1 +1) +  taille + 1 
int flop_reduce3(int x){
return (x/4) *5 + x+1;
};
/*
IPC : compris entre 3.2 et 4.1 
Resultat obtenu avec flopt_reduce0 car avec flop_reduce3 j'ai des resultat incohérent et je ne sais pas pourquoi..
*/

// Version 3 : quadruple parcours (supposons taille paire pour simplifier)
template <typename T>
void reduce4 (const std::vector <T> &V , T &res) {
T res_0 = START;
T res_1 = START;
T res_2 = START;
T res_3 = START;
unsigned int taille = V.size();

for (size_t i = 0; i < taille;i+=4){ //i prends +2 a chaque fois 
    res_0 = res OP V[i];
    res_1 = res OP V[i+1];
    res_2 = res OP V[i+2];
    res_3 = res OP V[i+3];

}
res = res_0+res_1+res_2+res_3;
};
// OP : iteration / 4 et i+=4,res_0+res_1+res_2+res_3 pour chaque iteration et hors iteration res_0+res_1+res_2+res_3 et V.size() --> (taille/4) * (1 + 1 + 1 + 1 +1) +  taille + 1 
int flop_reduce4(int x){
return (x/4) *5 + x+1;
};
/*
IPC : compris entre 3.2 et 4.1 
Resultat obtenu avec flopt_reduce0 car avec flop_reduce3 j'ai des resultat incohérent et je ne sais pas pourquoi..
*/




 // EXERCICE 7 : FIN 


// EXERCICE 8 : SLOW PERFORMANCE
#define C1 0.2f
#define C2 0.3f

void slowperformance1(float *x, const float *y, const float *z, int n){
    for (int i = 0; i < n -2 ; i++){
        x[i] = x[i] / M_SQRT2 + y[i] * C1;
        x[i+1] += z[(i%4)*10] * C2;
        x[i+2] += sin((2*M_PI*i)/3) * y[i+2];
    }
}

// Generateur de float

void generate(float *x, float *y, float *z, unsigned int n){
    x = new float [n];
    y = new float [n];
    z = new float [n];

    for ( size_t i = 0; i < n;i++){
        x[i]= rand() % 100 + 1; 
        y[i]= rand() % 100 + 1; 
        z[i]= rand() % 100 + 1; 
}
}


// EXERCICE 8 : FIN 




// JE RENDS MON CODE AVEC COMME DERNIER EXO DEBUT DE L' EXO 8








int main(){
int n,N;
double x,y;
n = 10000000;
x = 9.;
double res;
std::vector<double> T;
T.resize(n,2.);
float *x;
float *y;
float *z;
//generate(x,y,z,n);
EvalPerf PE;
PE.start();
//reduce0(T,res); //EX07.0
//reduce1(T,res); //EX07.1
//reduce2(T,res); //EX07.2
//reduce3(T,res); //EX07.3
//eval_poly2(T,x); // EXO6.2
//eval_poly1(T,x); // EXO6.1
//ma_fonction(n); EXO4
//pref(T); // EXO5
PE.stop();
//N=flop_ma_fonction1(n); EXO4
//N=flop_prefixe(n); // EXO5
//N = flop_eval_poly1(n); // EX06.1
//N = flop_eval_poly2(n); // EX06.2
N = flop_reduce0(n); // EX07.0
//N = flop_reduce1(n); // EX07.1
//N = flop_reduce2(n); // EX07.2
 //PE.Gflops(n);
std::cout<<"nbr cycle : "<< PE.cycles() <<std::endl;
std::cout<<"nbr secondes : "<< PE.seconds() <<std::endl;
std::cout<<"nbr millisecond : "<<PE.milliseconds()  <<std::endl;
std::cout<<"CPI : "<<PE.CPI(N)  <<std::endl;
std::cout<<"IPC : "<<PE.IPC(N)  <<std::endl;


}