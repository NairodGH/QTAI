#include "KNN.hpp"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "ETL.hpp"


KNN::KNN(int val)
{
    k = val;
}

KNN::KNN()
{

}

KNN::~KNN()
{
    // NOTHING TO DO 
}

void KNN::findKnearest(Data *queryPoint)
{
    neighbors = new std::vector<Data *>;
    double min = std::numeric_limits<double>::max();
    double previousMin = min;
    
    for(int i = 0, index; i < k; i++) {
        if(i == 0) {
            for(int j = 0; j < trainingData->size(); j++) {
                double dist = distance(queryPoint, trainingData->at(j));
                trainingData->at(j)->setDistance(dist);
                if(dist < min) {
                    min = dist;
                    index = j;
                }
            }
            neighbors->push_back(trainingData->at(index));
            previousMin = min;
            min = std::numeric_limits<double>::max();
        } else {
            for(int j = 0; j < trainingData->size(); j++) {
                double dist = trainingData->at(j)->getDistance();
                if(dist > previousMin && dist < min) {
                    min = dist;
                    index = j;
                }
            }
            neighbors->push_back(trainingData->at(index));
            previousMin = min;
            min = std::numeric_limits<double>::max();
        }
    }
}

void KNN::setK(int val)
{
    k = val;
}

int KNN::findMostFrequentClass()
{
    std::map<uint8_t, int> freqMap;

    for(int i = 0; i < neighbors->size(); i++)
    {
        if(freqMap.find(neighbors->at(i)->getLabel()) == freqMap.end())
            freqMap[neighbors->at(i)->getLabel()] = 1;
        else 
            freqMap[neighbors->at(i)->getLabel()]++;
    }

    int best = 0;
    int max = 0;

    for(auto kv : freqMap) {
        if(kv.second > max) {
            max = kv.second;
            best = kv.first;
        }
    }
    delete neighbors;
    return best;

}

double KNN::distance(Data* queryPoint, Data* input)
{
    double value = 0;

    if(queryPoint->getFeatureVectorSize() != input->getFeatureVectorSize()) {
        printf("Vector size mismatch.\n");
        exit(1);
    }
    for(unsigned i = 0; i < queryPoint->getFeatureVectorSize(); i++)
        value += pow(queryPoint->getFeatureVector()->at(i) - input->getFeatureVector()->at(i),2);
    return sqrt(value);
}

double KNN::validate()
{
    double currentPerformance = 0;
    int count = 0;
    int dataIndex = 0;

    for(Data *queryPoint : *validationData) {
        findKnearest(queryPoint);
        int prediction = findMostFrequentClass();
        dataIndex++;
        if(prediction == queryPoint->getLabel())
            count++;
        printf("Current Performance: %.3f %%\n", ((double)count)*100.0 / ((double)dataIndex));
    }
    currentPerformance = ((double)count)*100.0/((double)validationData->size());
    printf("Validation Performance for K = %d: %.3f\n", k, currentPerformance);
    return currentPerformance;
}

double KNN::test()
{
    double currentPerformance = 0;
    int count = 0;
    for(Data *queryPoint : *testData) {
        findKnearest(queryPoint);
        int prediction = findMostFrequentClass();
        if(prediction == queryPoint->getLabel())
            count++;
    }
    currentPerformance = ((double)count)*100.0/((double)testData->size());
    printf("Validation Performance for K = %d: %.3f\n", k, currentPerformance);
    return currentPerformance;
}

//int
//main()
//{
//    ETL *dh = new ETL();
//    dh->readInputData("../train-images-idx3-ubyte");
//    dh->readLabelData("../train-labels-idx1-ubyte");
//    dh->countClasses();
//    dh->splitData();
//    KNN *nearest = new KNN();
//    nearest->setK(1);
//    nearest->setTrainingData(dh->getTrainingData());
//    nearest->setTestData(dh->getTestData());
//    nearest->setValidationData(dh->getValidationData());
//    double performance = 0;
//    double best_performance = 0;
//    int best_k = 1;
//    for(int k = 1; k <= 3; k++)
//    {
//        if(k == 1)
//        {
//            performance = nearest->validate();
//            best_performance = performance;
//        } else 
//        {
//            nearest->setK(k);
//            performance = nearest->validate();
//            if(performance > best_performance)
//            {
//                best_performance = performance;
//                best_k = k;
//            }
//        }
//    }
//    nearest->setK(best_k);
//    nearest->test();
//}
