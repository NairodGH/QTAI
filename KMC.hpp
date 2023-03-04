#pragma once

#include "Sets.hpp"
#include <QtWidgets>
#include "ETL.hpp"

typedef struct cluster 
{
    std::vector<double> *centroid;
    std::vector<Data *> *clusterPoints;
    std::map<int, int> classCounts;
    int mostFrequentClass;

    cluster(Data *initialPoint)
    {
        centroid = new std::vector<double>;
        clusterPoints = new std::vector<Data *>;
        for(auto val : *(initialPoint->getFeatureVector()))
            centroid->push_back(val);
        clusterPoints->push_back(initialPoint);
        classCounts[initialPoint->getLabel()] = 1;
        mostFrequentClass = initialPoint->getLabel();
    }

    void add_to_cluster(Data *point)
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

    void set_mostFrequentClass()
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
} cluster_t;

class KMC : public QThread, public Sets {
    Q_OBJECT

public:
    KMC(int k, ETL *etl);
    void run() override;
    void initClusters();
    void initClustersForEachClass();
    void train();
    double distance(std::vector<double>*, Data*);
    double validate();
    double test();

signals:
    void progress(kmc_t infos);

private:
    int k;
    std::vector<cluster_t*>* clusters;
    std::unordered_set<int>* usedIndexes;
};