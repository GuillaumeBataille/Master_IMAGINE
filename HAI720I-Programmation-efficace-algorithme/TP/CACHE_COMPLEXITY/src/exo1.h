
#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib> // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"

void mat2vec (size_t n,std::vector<float> &A, std::vector<float> &v, std::vector<float> &y, std::vector<float> tmp, size_t k){
for (size_t i = 0 ; i < n ;i++){
    for(size_t j = 0; j < n; j+=4){
        tmp[i] += A[i*n + j] * v[j];
    }
}
for (size_t i = 0 ; i < n ;i++){
    for(size_t j = 0; j < n; j++){
        y[i] += A[i*n + j] * tmp[j];
    }
}
}

void mat2vec_transp(size_t n,std::vector<float> &A, std::vector<float> &v, std::vector<float> &y, std::vector<float> tmp, size_t k){

for (size_t i = 0 ; i < n ;i++){
    for(size_t j = 0; j < n; j+=4){
        tmp[i] += A[j*n + i] * v[j]; // Inversion de bouble
    }
}
for (size_t i = 0 ; i < n ;i++){
    for(size_t j = 0; j < n; j++){
        y[i] += A[i*n + j] * tmp[j];
    
}


}
}
