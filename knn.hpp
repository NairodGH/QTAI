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

        void findKnearest(Data *queryPoint);
        void setK(int val);
        int findMostFrequentClass();
        double distance(Data* queryPoint, Data* input);
        double validate();
        double test();
};