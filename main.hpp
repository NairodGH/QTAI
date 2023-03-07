#include <vector>
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
    std::string path;
    size_t size;
    size_t height;
    size_t width;
} data_t;

typedef struct {
    std::string path;
    int classCounts;
    size_t trainingSize;
    size_t testSize;
    size_t validationSize;
} labels_t;

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