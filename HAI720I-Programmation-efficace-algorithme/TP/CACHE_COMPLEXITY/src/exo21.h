
#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib>   // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"


void fillMatrix(size_t n, std::vector<double> &A){
   for (size_t i = 0; i < n; ++i) { // row
        for (size_t j = 0; j < n; ++j) { // column
        A[i * n + j]=rand() % 100;
        }
    }
}


void printMatrix(size_t n, std::vector<double> &A){
    std::cout<<"Matrice : "<<std::endl;
   for (size_t i = 0; i < n; ++i) { // row
        for (size_t j = 0; j < n; ++j) { // column
        std::cout<< " " << A[i*n+j];
        if(j == n-1) std::cout<<std::endl<<std::endl;
        }
    }
}
void NaiveTranspose(size_t n, std::vector<double> &A,  std::vector<double> &B)
{
   for (size_t i = 0; i < n; ++i) { // row
      for (size_t j = 0; j < n; ++j) { // column
         B[j * n + i] = A[i * n + j]; // Transposing
        }
    }
}

void BlockingTranspose(size_t n, std::vector<double> &A,  std::vector<double> &B, size_t blocksize)
{
for (size_t i = 0; i < n; i += blocksize) { //row
    for (size_t j = 0; j < n; j += blocksize) { // column
        // transpose the block beginning at [i,j]

        for (size_t k = i; k < i + blocksize; ++k) {// rowblock
            for (size_t l = j; l < j + blocksize; ++l) {//columnbloc
                B[k + l*n] = A[l + k*n];
            }
        }
    }
}
}
//--------------------- Cache Oblivious --------------------------//

    void print_submatrix1(const double * M, size_t m, size_t n, size_t s){

        for (size_t i = 0; i < m; i++)
        {
           for (size_t j = 0; j < n; j++)
           {
            std::cout<<M[i*s+j]<<" ";
           }
           std::cout<<std::endl;
           
        }
        
    }


void swap(double&a,double &b){
    double temp = a;
    a = b;
    b = temp;

}

void CacheObl_Transpose(double *a, int n, int N) {
    //Regner
    if (n <= 4) { // si on a 4*4
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                swap(a[i * N + j], a[j * N + i]);
    } else {
        //Diviser
        int k = n / 2;

        CacheObl_Transpose(a, k, N);
        CacheObl_Transpose(a + k, k, N);
        CacheObl_Transpose(a + k * N, k, N);
        CacheObl_Transpose(a + k * N + k, k, N);
        //Propager/Reconstuire
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                swap(a[i * N + (j + k)], a[(i + k) * N + j]);
    }
}




size_t NaiveTransposeFlop(size_t n,size_t B){
    return n*n/B;
}

/*

void mat2vec_t(size_t n, std::vector<float> &A, std::vector<float> &v, std::vector<float> &y, std::vector<float> tmp, size_t k)
{

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j += k)
        {
            tmp[i] += A[j * n + i] * v[j]; // Inversion de boucle
        }
    }
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j += k)
        {
            y[i] += A[i * n + j] * tmp[j];
        }
    }
}

unsigned long int mat2vec_flop(unsigned long int n) { return 4 * pow(n, 2) - 2 * n; }
*/