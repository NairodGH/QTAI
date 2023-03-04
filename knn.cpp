#include "KNN.hpp"


KNN::KNN(int k, ETL *etl)
{
    this->k = k;
    setTrainingData(etl->getTrainingData());
    setTestData(etl->getTestData());
    setValidationData(etl->getValidationData());
}

void KNN::run()
{
    int bestK = 1, tmpK = k;

    for (double performance = 0, bestPerformance = 0; k <= tmpK + 2; k++) {
        if (k == tmpK) {
            performance = getPrecision(true);
            bestPerformance = performance;
        }
        else {
            performance = getPrecision(true);
            if (performance > bestPerformance) {
                bestPerformance = performance;
                bestK = k;
            }
        }
    }
    k = bestK;
    getPrecision(false);
}

double KNN::getPrecision(bool isValidation)
{
    int count = 0, dataIndex = 0;
    std::vector<Data*>* set = isValidation ? validationData : testData;

    for (Data* queryPoint : *set) {
        findKnearest(queryPoint);
        int prediction = findMostFrequentClass();
        dataIndex++;
        if (prediction == queryPoint->getLabel())
            count++;
        emit progress(knn_t{ k, queryPoint->getLabel(), prediction, queryPoint->getFeatureVector(), (double)count * 100. / (double)dataIndex });
    }
    return (double)count * 100. / (double)(set->size());
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