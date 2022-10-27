
#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib>   // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"

void mat2vec(size_t n, std::vector<float> &A, std::vector<float> &v, std::vector<float> &y, std::vector<float> tmp, size_t k)
{
    std::cout << "start mat2vec" << std::endl;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j += k)
        {
            tmp[i] += A[i * n + j] * v[j];
        }
    }
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j += k)
        {
            y[i] += A[i * n + j] * tmp[j];
        }
    }
    std::cout << "end mat2vec" << std::endl;
}

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
