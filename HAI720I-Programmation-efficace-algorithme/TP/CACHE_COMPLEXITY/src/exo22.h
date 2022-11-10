
#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib>   // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"


size_t get_max(size_t size, size_t* T){
int max = T[0];
for (int i = 1; i < size; i++) {
    if (T[i] > max)
      max = T[i];
  }
} 

size_t get_min(size_t size, size_t* T){
int min = T[0];
for (int i = 1; i < size; i++) {
    if (T[i] < min)
      min = T[i];
  }
} 

void counting_sort_naif(size_t* R, size_t* T, size_t n, size_t min, size_t max){

size_t occurence[max+1];
for (size_t i = 0; i < max+1; i++)
{
    occurence[i] = 0; // Set tab d'occurence de taille max+1;
}

  for (size_t i=0; i<n; i++) {  
    occurence[R[i]]++; // Ajout des occurences 
  }

    for (size_t i = 1; i < max+1 ; i++){
        occurence[i] += occurence[i-1]; // Compute les somme prefixes
    }


    for (size_t i = 0; i < n; i++)
{
    T[i] = 0; // Set tab d'occurence de taille max+1;
}
     for (size_t i = 0; i < n; i++)
     {
        T[occurence[i]] = R[i];
        occurence[i]++;
     }
     
  }; 
