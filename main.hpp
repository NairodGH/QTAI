#include <vector>
#include "stdint.h" // uint8_t 
#include "stdio.h"
#include <string>
#include <map>
#include <unordered_set>
#include <random>
#include "fstream"
#include <limits>
#include <cstdlib>
#include <cmath>
#include <iostream>

typedef struct {
    int k;
    int actualNumber;
    int guessedNumber;
    std::vector<uint8_t>* image;
    double performance;
} knn_t;

typedef struct {
    int k;
    double performance;
} kmc_t;