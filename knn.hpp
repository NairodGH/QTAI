#pragma once

#include "Sets.hpp"

// O(k*n) where k is the number of neighbors and N is the size of training Data
// O(n) + O(k*n) + k

class KNN : public Sets
{
    private:
        int k;
        std::vector<Data *> * neighbors;

    public:
        KNN(int);
        KNN();
        ~KNN();

        void find_knearest(Data *query_point);
        void set_k(int val);
        int find_most_frequent_class();
        double calculate_distance(Data* query_point, Data* input);
        double validate_perforamnce();
        double test_performance();
};