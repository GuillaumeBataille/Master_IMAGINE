#include <vector>

std::vector<double> prefixe(std::vector<double> &T){

    for(size_t i=1; i<T.size(); i++){
        T[i]+= T[i-1];
    }
    return T;
}

