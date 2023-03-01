#pragma once

#include "Sets.hpp"

// Complexity: O(k*n) where k is the number of neighbors and N is the size of training Data

class KNN : public Sets
{
    public:
        KNN(int);

        void findKnearest(Data *queryPoint);
        void setK(int val);
        int findMostFrequentClass();
        double distance(Data* queryPoint, Data* input);

    private:
        int k;
        std::vector<Data*>* neighbors;
};