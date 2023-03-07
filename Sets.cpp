#include "Sets.hpp"

void Sets::setTrainingData(std::vector<Data *> * vect)
{
    trainingData = vect;
}

void Sets::setTestData(std::vector<Data *> * vect)
{
    testData = vect;
}

void Sets::setValidationData(std::vector<Data *> * vect)
{
    validationData = vect;
}

std::vector<Data *> *Sets::getTrainingData()
{
    return trainingData;
}

std::vector<Data *> *Sets::getTestData()
{
    return testData;
}

std::vector<Data *> *Sets::getValidationData()
{
    return validationData;
}
