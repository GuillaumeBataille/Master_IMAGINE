#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib> // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"

int32_t reduceAdd(std::vector<int32_t> tab){
    
    int32_t res = 0;
    
    for(int32_t val : tab){
        res += val;
    }

    return res;
}

double reduceMul(std::vector<double> tab){
    
    double res = 1;
    
    for(double val : tab){
        res *= val;
    }

    return res;
}

double doubleReduceMulSIMD(std::vector<double> tab){
    __m256d vec1 = _mm256_loadu_pd(tab.data());
    __m256d vec2;
    double storedRes[4];
    double res;

    size_t i = 4;
    for(;i < tab.size() - 3; i = i+4){
        vec2 = _mm256_loadu_pd(tab.data() + i);

        vec1 = _mm256_mul_pd(vec1, vec2);
    }

    _mm256_storeu_pd(storedRes, vec1);
    res = storedRes[0] * storedRes[1] * storedRes[2] * storedRes[3];

    for(;i < tab.size(); i++){
        res *= tab[i];
    }
    
    return res;
}

   


int32_t int32ReduceAddSIMD(std::vector<int32_t> tab){
    __m256i vec1 = _mm256_loadu_si256((__m256i *)tab.data());
    __m256i vec2;
    int32_t storedRes[8];
    int32_t res;

    size_t i = 8;
    for(;i < tab.size() - 7; i = i+8 ){
        vec2 = _mm256_loadu_si256((__m256i *)tab.data() + i);

        vec1 = _mm256_add_epi32(vec1, vec2);
    }

    _mm256_storeu_si256((__m256i *)storedRes, vec1);
    res = storedRes[0] + storedRes[1] + storedRes[2] + storedRes[3]
    + storedRes[4] + storedRes[5] + storedRes[6] + storedRes[7];

    for(;i < tab.size(); i++){
        res += tab[i];
    }
    
    return res;
}

unsigned int flop_reduce(int n, int taille){
 return n*taille;
};



/*
Question 10. Pour un reduce 10 000 000 fois

Mult double classique avec fno tree vectorize:
CPI: 38.9547
IPC: 0.0256708

Mult double classique avec les options SIMD:
CPI: 0.214308
IPC: 4.66618

Mult double SIMD avec les options SIMD:
CPI: 0.0762697
IPC: 13.1114

Add int32 classique avec fno tree vectorize:
CPI: 315.638
IPC: 0.00316819

Add int32 classique avec les options SIMD:
CPI: 0.966603
IPC: 1.03455

Add int32 SIMD avec les options SIMD:
CPI: 0.735934
IPC: 1.35882

*/

float min(std::vector<float> tab){
    float res;
    res = tab[0];
    for (size_t i = 0; i< tab.size() ; i++){
        if(res < tab[i]) res = tab[i];
    }
    return res;
}

float min_withlib(std::vector<float> tab){

    return *std::min_element(tab.data(),tab.data()+tab.size());
}

float floatMinSIMD(std::vector<float> tab){
    // Pour float type = __m256 et load = _mm256_loadu_ps
    __m256 vec1 = _mm256_loadu_ps(tab.data());
    __m256 vec2;
    float storedRes[8];
    float res;

    size_t i = 8;
    for(;i < tab.size() - 7; i = i+8){
        vec2 = _mm256_loadu_ps(tab.data() + i);

        vec1 = _mm256_min_ps(vec1, vec2); // Compare les pairs vec1/vec2 et on mets dans vec1 les min
    }

    _mm256_storeu_ps(storedRes, vec1);
     
    res = storedRes[0];
    
    for (size_t j=1; j < 8 ; j++){
        if(res > storedRes[j]) res = storedRes[j];
    }
   
    for(;i < tab.size(); i++){
        if(res > storedRes[i]) res = storedRes[i];
    }
    
    return res;
}

std::vector<float> generate_rand_float_tab(unsigned int taille){
    std::vector<float> res;
    res.resize(taille,0.);
    for (size_t i = 0; i< res.size(); i++){
        res[i]= rand()%50000000;
    }
    return res;
}
/* Question 11 avec des vecteur float de taille 1 000 000

min float classique avec fno tree vectorize:
CPI: 0.255545
IPC: 3.9132

min float classique avec les options SIMD:
CPI: 0.0193462
IPC: 51.6896

min SIMD avec les options SIMD:
CPI: 0.018452
IPC: 54.1947

min avec std::min_element + options SIMD:
CPI: 0.0195798
IPC: 51.073

*/


std::vector<double> generate_rand_double_tab(unsigned int taille){
    std::vector<double> res;
    res.resize(taille,0.);
    for (size_t i = 0; i< res.size(); i++){
        res[i]= rand()%50000000;
    }
    return res;
}

// Ordre : i j k  
std::vector<double> Matrix_double_mult_1(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){
        for(size_t i=0 ; i < n; i++){
            for(size_t j = 0; j < n; j++){
                for (size_t k = 0; k < n; k++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}

// Ordre : i k j // MEILLEUR 
std::vector<double> Matrix_double_mult_2(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

        for(size_t i = 0; i < n ; i++){
            for(size_t k = 0; k < n; k++){
                for (size_t j = 0; j < n; j++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}

// Ordre : k j i
std::vector<double> Matrix_double_mult_3(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

        for(size_t k = 0; k < n; k++){
            for(size_t j = 0; j < n; j++){
                for (size_t i = 0; i < n; i++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}

// Ordre : j i k
std::vector<double> Matrix_double_mult_4(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

        for(size_t j = 0; j < n; j++){
            for(size_t i = 0; i < n; i++){
                for (size_t k = 0; k < n; k++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}

// Ordre : k i j 
std::vector<double> Matrix_double_mult_5(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

        for(size_t k = 0; k < n; k++){
            for(size_t i = 0; i < n; i++){
                for (size_t j = 0; j < n; j++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}

// Ordre : j k i
std::vector<double> Matrix_double_mult_6(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

        for(size_t j = 0; j < n; j++){
            for(size_t k = 0; k < n; k++){
                for (size_t i = 0; i < n; i++){
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
                    
                }
            }
        }
    return C;
}


std::vector<double> Matrix_double_mult_SIMD(std::vector<double> A,std::vector<double> B, std::vector<double> C, unsigned int n){

    __m256d vecA;
    __m256d vecB;
    __m256d vecC;
    size_t i = 0;
    size_t j = 0;
           for(i=0; i < n; i++){
               std::cout<<"IN i "<< i << " j "<< j <<std::endl;
            for(j = 0; j < n-3; j+=4){
                 std::cout<<"   IN j" << j <<std::endl;
                vecC = _mm256_loadu_pd(C.data()+i*n+j); // On récupère le vecteur de taille 4 resultant courant C[i*n+j];
                for (size_t k=0; k < n; k++){
                    std::cout<<"        IN k "<< k<<std::endl;
                   //std::cout << "i :" <<i <<" j : "<< j << " k : "<< k << " A[i*n+k] = "<< A[i*n+k] <<std::endl;
                        vecA = _mm256_set1_pd(A[i*n+k]); // On recupère le vecteur set avec les valeurs de A[i*n,k] partout
                        vecB = _mm256_loadu_pd(B.data()+k*n+j); // On récupère le vecteur de taille 4 courant débutant a B[k*n+j];
                    vecC = _mm256_fmadd_pd(vecA,vecB,vecC);
                   _mm256_storeu_pd(C.data()+i*n+j,vecC); // On ajoute notre somme FMA dans le vect resultat C;

                }
            
            }
            std::cout << "///////////////////////"<<std::endl;
                        // Si la taille max de j n'était pas un mult de 4 alors on reboucle 
            for (; j< n; j++){
                 std::cout<<"out i " << i << " out j " << j << "et n "<< n << " case non touché "<< C[i*n +j] <<std::endl;
                for(size_t k = 0; k < n; k++){
                C[i*n+j] += A[i*n+k] * B[k*n+j];
                }
            }
        }
    return C;

}


// Question 12 

double evalPolySIMD(std::vector<double> coefTab, double a){

    __m256d vec1 = _mm256_loadu_pd(coefTab.data());
    __m256d vec2;
    double storedRes[4];

    if(coefTab.size() < 4){
        double res;
        for(int i = 0; i < coefTab.size(); i++){
            res += coefTab[i] * pow(a,i);
        }
        
        return res;
    }
    
    size_t i = 4;
    for(;i < coefTab.size() - 3; i = i+4){
        vec2 = _mm256_loadu_pd(coefTab.data() + i);
        __m256d firstIndice = _mm256_set1_pd(pow(a, i));
        vec2 = _mm256_mul_pd(vec2, firstIndice);

        vec1 = _mm256_add_pd(vec1, vec2);
    }

    _mm256_storeu_pd(storedRes, vec1);
    
    long multiplyRet = i;
    for(;i < coefTab.size(); i++){
        storedRes[i] += coefTab[i]*multiplyRet;
    }

    return storedRes[0] + storedRes[1]*a + storedRes[2]*pow(a,2) + storedRes[3]*pow(a,3);
}


// Question 13 
/*
13.1. On a une matrice (i,j) de taille m lignes *n colonnes avec i = ligne et j = colonnes
    et on a notre vecteur(x) unidimentionnel de taille n*m.
    Pour retrouver un élément a l'indice i,j, il faut :
    - parcourir n*i (n fois le nombre d'élément d'une ligne)
    - parcourir j éléménet pour accéder à la colonne souhaité 

    Ce qui nous donne M[i,j] = V[i*n + j];

13.2. test avec deux matrices (1,2,3,4) de taille n*n (n = 2)
 mat_carré 0 : 1
 mat_carré 1 : 2
 mat_carré 2 : 3
 mat_carré 3 : 4

 mat_res 0 : 7
 mat_res 1 : 10
 mat_res 2 : 15
 mat_res 3 : 22 // Valide

// Pour n = 1000 sans optimisation () et N = 2*(n*n*n)
1:
CPI: 14.4197
IPC: 0.0693494

2:
CPI: 13.8366
IPC: 0.072272

3:
CPI: 19.6217
IPC: 0.0509639

4:
CPI: 20.338
IPC: 0.049169

5:
CPI: 17.5249
IPC: 0.0570615

6:
CPI: 17.8895
IPC: 0.0558987


Le meilleur semble être le 2 (Ordre i k j) avec le 1 qui s'en rapproche (Ordre k i j)
Ce qui semble cohérent car J c'est le plus profond dans le parcours des matrices;

13.3.
MultMat version SIMD et FMA sans l'option SIMD:
CPI: 10.4749
IPC: 0.0954664

Donc bien plus performant que le reste au dessus.

13.4. avec le SIMD option activé , on constate que tout ce vaut plus ou moins:
    Donc cela signifie que le compilateur parallelise naturellement.

*/

//Question 14

std::vector<int64_t> int64t_transpose (std::vector<int64_t> mat,unsigned int n){
    std::vector<int64_t> mat_res;
    mat_res.resize(n*n,0.);

    for(size_t i = 0; i< n;i++){
         for(size_t j = 0; j< n;j++){
            mat_res[i*n+j] = mat[j*n+i];
         }
    }

    return mat_res;
}

/*
14.1 Transpose naïf : 

 vec initial : 
8 , 2 , 4 , 2
6 , 10 , 8 , 4
11 , 2 , 12 , 9
1 , 0 , 6 , 8
 vec transposed : 
8 , 6 , 11 , 1
2 , 10 , 2 , 0
4 , 8 , 12 , 6
2 , 4 , 9 , 8

*/


std::vector<int64_t> SIMD_4X4_transpose(std::vector<int64_t> mat, unsigned int n){

    /* Trouvé sur un forum 
__m256i a;    // L1 H1
__m256i b;    // L2 H2
__m256i c;    // L3 H3
__m256i d;    // L4 H4

__m256i A = _mm256_permute2x128_si256(a, b, 0x20);  // L1 L2
__m256i B = _mm256_permute2x128_si256(a, b, 0x31);  // H1 H2
__m256i C = _mm256_permute2x128_si256(c, d, 0x20);  // L3 L4
__m256i D = _mm256_permute2x128_si256(c, d, 0x31);  // H3 H4
    
    */

    std::vector<int64_t> res;
    res.resize(n*n,0);

    __m256i r0 = _mm256_loadu_si256((__m256i *)mat.data());
    __m256i r1 = _mm256_loadu_si256((__m256i *)(mat.data()+n));
    __m256i r2 = _mm256_loadu_si256((__m256i *)(mat.data()+n*2));
    __m256i r3 = _mm256_loadu_si256((__m256i *)(mat.data()+n*3));

    __m256i v0 = _mm256_unpacklo_epi64(r0, r1);  
    __m256i v1 = _mm256_unpackhi_epi64(r0, r1); 
    __m256i v2 = _mm256_unpacklo_epi64(r2, r3); 
    __m256i v3 = _mm256_unpackhi_epi64(r2, r3); 

    r0 = _mm256_permute2x128_si256(v0,v2,0x20);
    r2 = _mm256_permute2x128_si256(v0,v2,0x31);
    r1 = _mm256_permute2x128_si256(v1,v3,0x20);
    r3 = _mm256_permute2x128_si256(v1,v3,0x31);

    _mm256_storeu_si256((__m256i *)res.data(), r0);
    _mm256_storeu_si256((__m256i *)(res.data()+n), r1);
    _mm256_storeu_si256((__m256i *)(res.data()+n*2), r2);
    _mm256_storeu_si256((__m256i *)(res.data()+n*3), r3);

    return res;
}

// 14.2
/* 
 vec transposed : 
8 , 6 , 11 , 1
2 , 10 , 2 , 0
4 , 8 , 12 , 6
2 , 4 , 9 , 8
 vec SIMD transposed : 
8 , 6 , 11 , 1
2 , 10 , 2 , 0
4 , 8 , 12 , 6
2 , 4 , 9 , 8


*/
std::vector<int64_t> generate_rand_int64_tab(unsigned int taille){
    std::vector<int64_t> res;
    res.resize(taille,0.);
    for (size_t i = 0; i< res.size(); i++){
        res[i]= rand()%15;
    }
    return res;
}

std::vector<int64_t> SIMD_transpose(std::vector<int64_t> mat, unsigned int n){

    /* Trouvé sur un forum 
__m256i a;    // L1 H1
__m256i b;    // L2 H2
__m256i c;    // L3 H3
__m256i d;    // L4 H4

__m256i A = _mm256_permute2x128_si256(a, b, 0x20);  // L1 L2
__m256i B = _mm256_permute2x128_si256(a, b, 0x31);  // H1 H2
__m256i C = _mm256_permute2x128_si256(c, d, 0x20);  // L3 L4
__m256i D = _mm256_permute2x128_si256(c, d, 0x31);  // H3 H4
    
    */

    std::vector<int64_t> res;
    __m256i r0,r1,r2,r3,v0,v1,v2,v3;
    unsigned int taille = n;

    res.resize(taille*taille,0);

    for (size_t i = 0; i < n; i += 4){
        for (size_t j = 0; j < n; j += 4){
            r0 = _mm256_loadu_si256((__m256i *)(mat.data()+i*taille+j+taille*0));
            r1 = _mm256_loadu_si256((__m256i *)(mat.data()+i*taille+j+taille*1));
            r2 = _mm256_loadu_si256((__m256i *)(mat.data()+i*taille+j+taille*2));
            r3 = _mm256_loadu_si256((__m256i *)(mat.data()+i*taille+j+taille*3));

            v0 = _mm256_unpacklo_epi64(r0, r1);
            v1 = _mm256_unpackhi_epi64(r0, r1); 
            v2 = _mm256_unpacklo_epi64(r2, r3); 
            v3 = _mm256_unpackhi_epi64(r2, r3); 

            r0 = _mm256_permute2x128_si256(v0,v2,0x20);
            r2 = _mm256_permute2x128_si256(v0,v2,0x31);
            r1 = _mm256_permute2x128_si256(v1,v3,0x20);
            r3 = _mm256_permute2x128_si256(v1,v3,0x31);

            // Attention : inverser i et j pour la version transposed
            _mm256_storeu_si256((__m256i *)(res.data()+j*taille+i+taille*0),r0);
            _mm256_storeu_si256((__m256i *)(res.data()+j*taille+i+taille*1),r1);
            _mm256_storeu_si256((__m256i *)(res.data()+j*taille+i+taille*2),r2);
            _mm256_storeu_si256((__m256i *)(res.data()+j*taille+i+taille*3),r3);

        }
    }

    
    return res;
}
//14.3
/* 
 initial : 
13 , 1 , 12 , 10 , 8 , 10 , 1 , 12
9 , 1 , 2 , 7 , 5 , 4 , 8 , 1
0 , 6 , 7 , 1 , 11 , 8 , 12 , 9
2 , 5 , 2 , 13 , 7 , 10 , 14 , 12
12 , 3 , 14 , 12 , 13 , 1 , 1 , 7
9 , 3 , 6 , 14 , 14 , 7 , 8 , 14
5 , 0 , 8 , 1 , 1 , 5 , 11 , 3
2 , 5 , 1 , 1 , 0 , 0 , 14 , 12
 transposed standard : 
13 , 9 , 0 , 2 , 12 , 9 , 5 , 2
1 , 1 , 6 , 5 , 3 , 3 , 0 , 5
12 , 2 , 7 , 2 , 14 , 6 , 8 , 1
10 , 7 , 1 , 13 , 12 , 14 , 1 , 1
8 , 5 , 11 , 7 , 13 , 14 , 1 , 0
10 , 4 , 8 , 10 , 1 , 7 , 5 , 0
1 , 8 , 12 , 14 , 1 , 8 , 11 , 14
12 , 1 , 9 , 12 , 7 , 14 , 3 , 12
 transposed SIMD : 
13 , 9 , 0 , 2 , 12 , 9 , 5 , 2
1 , 1 , 6 , 5 , 3 , 3 , 0 , 5
12 , 2 , 7 , 2 , 14 , 6 , 8 , 1
10 , 7 , 1 , 13 , 12 , 14 , 1 , 1
8 , 5 , 11 , 7 , 13 , 14 , 1 , 0
10 , 4 , 8 , 10 , 1 , 7 , 5 , 0
1 , 8 , 12 , 14 , 1 , 8 , 11 , 14
12 , 1 , 9 , 12 , 7 , 14 , 3 , 12

ça a l'air de fonctionner 

Version naive avec N = n*n (fno tree vectorize): 
CPI: 116.252
IPC: 0.008602

Version SIMD avec  N = n*n :
CPI: 32.1858
IPC: 0.0310696

4* plus interessant d'avoir le SIMD.

*/

template<typename T> 
void tri_select(std::vector<T> & tab){
    unsigned int taille = tab.size();
    size_t min;
    T swap;
    for(size_t i = 0; i < taille - 1 ;i++){ // Pour tout les ele sauf le dernier
        min = i;
        for (size_t j= i+1;j < taille ; j++ ){ // Pour tout les elements compris entre le ieme et le dernier
            if(tab[j] < tab[min])
                min = j;
        }
        if (min != i){
            swap = tab[min];
            tab[min] = tab[i];
            tab[i] = swap;
        }
    }
}

template<typename T> 
void print_tab(std::vector<T> tab){
    for(size_t i=0; i <tab.size() ; i++){
        std::cout<<tab[i]<< " - ";
    }
    std::cout<<std::endl;
}

template<typename T> 
void generate_rand_tab(std::vector<T> & tab, unsigned int taille, unsigned int max){
    tab.resize(taille,0.);
    for (size_t i = 0; i< taille; i++){
        tab[i]= rand()%max;
    }
}

template<typename T> 
void tri_fusion (std::vector<T> & tab){
    unsigned int taille = tab.size();
    size_t min;
    T swap;
    for(size_t i = 0; i < taille - 1 ;i++){ // Pour tout les ele sauf le dernier
        min = i;
        for (size_t j= i+1;j < taille ; j++ ){ // Pour tout les elements compris entre le ieme et le dernier
            if(tab[j] < tab[min])
                min = j;
        }
        if (min != i){
            swap = tab[min];
            tab[min] = tab[i];
            tab[i] = swap;
        }
    }
}


// Fonction H

//Question 15

/*
Pour les test, j'ai décidé de faire des fonctions avec Template

POur ma fonction On2 j'ai choisi le tri par selection avec comme N = (n * n-1) / 2

Pour ma fonction Onlogn j'ai choisi le tri fusion/dichotomique avec comme N = n * log (n)

On2:

2^17 : nb secondes: 52.6463
2^18 : ne repond pas après + 1 min

Pas eu le temps d'implémenter le tru fustion :



A partir de 2^12, des secondes se font remarquer avant d'avoir le resultat.

Pour 2^

*/

int main(){
    std::vector<double> vec = {8, 2, 4, 2, 6, 10, 8, 4, 6, 1, 6};
    std::vector<int32_t> vec_int = {8, 2, 4, 2, 6, 10, 8, 4, 6, 1, 6};
    std::vector<int64_t> vec_int64 = {8, 2, 4, 2,
                                      6, 10, 8, 4,
                                      11, 2, 12, 9,
                                       1, 0, 6, 8};

    std::vector<int64_t> vec_res_int64;
    std::vector<int64_t> vec_res_SIMDint64;
    std::vector<int8_t> tab8;
    std::vector<int16_t> tab16;
    std::vector<int32_t> tab32;
    std::vector<ino64_t> tab64;

    //int n = 10;
    size_t taille = pow(2.,15);
    //vec_int64 = generate_rand_int64_tab(n);
    //int nbBoucle = 10;
    //std::vector<float> vec_rand = generate_rand_float_tab(100000000);
    //std::vector<double> mat_A;
    //mat_A = generate_rand_double_tab(n*n);
    //std::vector<double> mat_B; 
    //mat_B= generate_rand_double_tab(n*n);
    //std::vector<double> mat_res;
    //mat_A.resize(n*n,2.);
    //mat_B.resize(n*n,2.);
    //std::vector<double> mat_C; mat_C.resize(n*n,0.);

    //generate_rand_tab(tab8,taille,taille);
    //generate_rand_tab(tab16,taille,taille);
    //generate_rand_tab(tab32,taille,taille);
    generate_rand_tab(tab64,taille,taille);


    int N;

    EvalPerf PE;

    PE.start();
    //Q10
    /*for(size_t i = 0; i < nbBoucle; i++){
        //doubleReduceMulSIMD(vec);
        //int32ReduceAddSIMD(vec_int);
        //reduceMul(vec);   
        //reduceAdd(vec_int);
        
    }*/
    // Q11 
    /*//min(vec_rand);
    //floatMinSIMD(vec_rand);
    //min_withlib(vec_rand);*/

    //Q12
    //evalPolySIMD(mat_A,mat_A[0]);

    //Q13
    //13.2
    //Matrix_double_mult_1(mat_A,mat_B,mat_C,n);
    //Matrix_double_mult_2(mat_A,mat_B,mat_C,n);
    //Matrix_double_mult_3(mat_A,mat_B,mat_C,n);
    //Matrix_double_mult_4(mat_A,mat_B,mat_C,n);
    //Matrix_double_mult_5(mat_A,mat_B,mat_C,n);
    //Matrix_double_mult_6(mat_A,mat_B,mat_C,n);
    // 13.3
    //mat_res = Matrix_double_mult_SIMD(mat_A,mat_B,mat_C,n);
    
    //Q14
    //14.1
    // vec_res_int64 =  int64t_transpose(vec_int64,n);
    //14.2
    // vec_res_SIMDint64 =  SIMD_4X4_transpose(vec_int64,n);
    //14.3
    //vec_res_int64 =  int64t_transpose(vec_int64,n);

    //Q15

    //tri_select(tab8);
    //tri_select(tab16);
    //tri_select(tab32);
    tri_select(tab64);



    PE.stop();
    
    N = (taille*taille-1)/2;
    //N = 2*(n*n*n);
    //N = PE.Gflops(100000000);
    //N = flop_reduce(nbBoucle,vec.size());
    //N = flop_reduce(nbBoucle,vec_int.size());
    std::cout << "nb cycles: " << PE.nb_cycle() << std::endl;
    std::cout << "nb secondes: " << PE.second() << std::endl;
    std::cout << "nb millisecondes: " << PE.millisecond() << std::endl;
    std::cout << "CPI: " << PE.CPI(N) << std::endl;
    std::cout << "IPC: " << PE.IPC(N) << std::endl;
    //std::cout << int32ReduceAddSIMD(vec) << std::endl;
}