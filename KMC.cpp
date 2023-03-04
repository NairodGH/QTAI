#include "KMC.hpp"

KMC::KMC(int k, ETL *etl)
{
    this->k = k;
    setTrainingData(etl->getTrainingData());
    setTestData(etl->getTestData());
    setValidationData(etl->getValidationData());
}

void KMC::run() {
    double performance = 0, best_performance = 0;

    for (int best_k = k; k < validationData->size() * 0.1; k++) {
        clusters = new std::vector<cluster_t*>;
        usedIndexes = new std::unordered_set<int>;
        initClusters();
        train();
        performance = validate();
        emit progress(kmc_t{ k, performance });
        if (performance > best_performance) {
            best_performance = performance;
            best_k = k;
        }
        delete clusters;
        delete usedIndexes;
    }
    train();
}

void KMC::initClusters()
{
    for(int i = 0, index = 0; i < k; i++) {
        while(usedIndexes->find(index) != usedIndexes->end())
            index++;
        clusters->push_back(new cluster_t(trainingData->at(index)));
        usedIndexes->insert(index);
    }
}

void KMC::train()
{
    for (int index = 0; usedIndexes->size() < trainingData->size(); ) {
        while(usedIndexes->find(index) != usedIndexes->end())
            index++;
        double minDist = std::numeric_limits<double>::max();
        int bestCluster = 0;
        for(int j = 0; j < clusters->size(); j++) {
            double dist = distance(clusters->at(j)->centroid, trainingData->at(index));
            if(dist < minDist) {
                minDist = dist;
                bestCluster = j;
            }
        }
        clusters->at(bestCluster)->add_to_cluster(trainingData->at(index));
        usedIndexes->insert(index);
    }
}

double KMC::distance(std::vector<double> *centroid, Data *queryPoint)
{
    double dist = 0.0;

    for(int i = 0; i < centroid->size(); i++)
        dist += pow(centroid->at(i) - queryPoint->getFeatureVector()->at(i), 2);
    return sqrt(dist);
}

double KMC::validate()
{
    double numCorrect = 0.0;

    for(auto queryPoint : *validationData) {
        double minDist = std::numeric_limits<double>::max();
        int best = 0;
        for(int i = 0; i < clusters->size(); i++) {
            double currentDist = distance(clusters->at(i)->centroid, queryPoint);
            if(currentDist < minDist) {
                minDist = currentDist;
                best = i;
            }
        }
        if(clusters->at(best)->mostFrequentClass == queryPoint->getLabel())
            numCorrect++;
    }
    return 100.0 * (numCorrect / (double) validationData->size());
}

double KMC::test()
{
    double numCorrect = 0.0;

    for(auto queryPoint : *testData) {
        double minDist = std::numeric_limits<double>::max();
        int best = 0;
        for(int i = 0; i < clusters->size(); i++) {
            double currentDist = distance(clusters->at(i)->centroid, queryPoint);
            if(currentDist < minDist) {
                minDist = currentDist;
                best = i;
            }
        }
        if(clusters->at(best)->mostFrequentClass == queryPoint->getLabel()) numCorrect++;
    }
    return 100.0 * (numCorrect / (double) testData->size());
}