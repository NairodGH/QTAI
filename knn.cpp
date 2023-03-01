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

void KNN::setK(int val)
{
    k = val;
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

    for (unsigned i = 0; i < queryPoint->getFeatureVectorSize(); i++)
        value += pow(queryPoint->getFeatureVector()->at(i) - input->getFeatureVector()->at(i), 2);
    return sqrt(value);
}