#include "Cluster.hpp"

Cluster::Cluster(Data *initialPoint)
{
    centroid = new std::vector<double>;
    clusterPoints = new std::vector<Data *>;
    for(auto val : *(initialPoint->getFeatureVector()))
        centroid->push_back(val);
    clusterPoints->push_back(initialPoint);
    classCounts[initialPoint->getLabel()] = 1;
    mostFrequentClass = initialPoint->getLabel();
}

void Cluster::add_to_cluster(Data *point)
{
    int previousSize = clusterPoints->size();

    clusterPoints->push_back(point);
    for(int i = 0; i < centroid->size(); i++) {
        double val = centroid->at(i);
        val *= previousSize;
        val += point->getFeatureVector()->at(i);
        val /= (double)clusterPoints->size();
        centroid->at(i) = val;
    }
    if(classCounts.find(point->getLabel()) == classCounts.end())
        classCounts[point->getLabel()] = 1;
    else
        classCounts[point->getLabel()]++;
    set_mostFrequentClass();
}

void Cluster::set_mostFrequentClass()
{
    int bestClass;
    int freq = 0;

    for(auto kv : classCounts) {
        if(kv.second > freq) {
            freq = kv.second;
            bestClass = kv.first;
        }
    }
    mostFrequentClass = bestClass;
}

std::vector<double>* Cluster::getCentroid()
{
    return centroid;
}

int Cluster::getMostFrequentClass()
{
    return mostFrequentClass;
}