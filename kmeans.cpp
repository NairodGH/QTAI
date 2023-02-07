#include "Kmeans.hpp"

Kmeans::Kmeans(int k)
{
    numClusters = k;
    clusters = new std::vector<cluster_t *>;
    usedIndexes = new std::unordered_set<int>;
}

void Kmeans::initClusters()
{
    for(int i = 0, index = 0; i < numClusters; i++) {
        while(usedIndexes->find(index) != usedIndexes->end())
            index++;
        clusters->push_back(new cluster_t(trainingData->at(index)));
        usedIndexes->insert(index);
    }
}

void Kmeans::initClustersForEachClass()
{
    std::unordered_set<int> classes_used;

    for(int i = 0; i < trainingData->size(); i++) {
        if(classes_used.find(trainingData->at(i)->getLabel()) == classes_used.end()) {
            clusters->push_back(new cluster_t(trainingData->at(i)));
            classes_used.insert(trainingData->at(i)->getLabel());
            usedIndexes->insert(i);
        }
    }
}

void Kmeans::train()
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

double Kmeans::distance(std::vector<double> *centroid, Data *queryPoint)
{
    double dist = 0.0;

    for(int i = 0; i < centroid->size(); i++)
        dist += pow(centroid->at(i) - queryPoint->getFeatureVector()->at(i), 2);
    return sqrt(dist);
}

double Kmeans::validate()
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

double Kmeans::test()
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

//int
//main()
//{
//    ETL *dh = new ETL();
//    dh->readInputData("../train-images-idx3-ubyte");
//    dh->readLabelData("../train-labels-idx1-ubyte");
//    dh->countClasses();
//    dh->splitData();
//    double performance = 0;
//    double best_performance = 0;
//    int best_k = 1;
//    for(int k = dh->getClassCounts(); k < dh->getTrainingData()->size()*0.1; k++)
//    {
//        Kmeans *km = new Kmeans(k);
//        km->setTrainingData(dh->getTrainingData());
//        km->setTestData(dh->getTestData());
//        km->setValidationData(dh->getValidationData());
//        km->initClusters();
//        km->train();
//        performance = km->validate();
//        printf("Current Perforamnce @ K = %d: %.2f\n", k, performance);
//        if(performance > best_performance)
//        {
//            best_performance = performance;
//            best_k = k;
//        }
//    }
//    Kmeans *km = new Kmeans(best_k);
//    km->setTrainingData(dh->getTrainingData());
//    km->setTestData(dh->getTestData());
//    km->setValidationData(dh->getValidationData());
//    km->initClusters();
//    km->train();
//    printf("Overall Performance: %.2f\n",km->test());
//
//}
