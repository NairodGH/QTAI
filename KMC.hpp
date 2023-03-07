#pragma once

#include "Cluster.hpp"

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
    void kmcProgress(kmc_t infos);

private:
    int k;
    std::vector<Cluster *> *clusters;
    std::unordered_set<int>* usedIndexes;
};