#pragma once

#include "Sets.hpp"
#include <QtWidgets>
#include "ETL.hpp"

class Cluster
{
    public:
        Cluster(Data *initialPoint);
        void add_to_cluster(Data *point);
        void set_mostFrequentClass();
        std::vector<double>* getCentroid();
        int getMostFrequentClass();
    private:
        std::vector<double> *centroid;
        std::vector<Data *> *clusterPoints;
        std::map<int, int> classCounts;
        int mostFrequentClass;
};