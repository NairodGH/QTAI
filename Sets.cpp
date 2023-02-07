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
