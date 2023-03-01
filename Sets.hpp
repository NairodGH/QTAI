#pragma once

#include "Data.hpp"

class Sets
{
    protected:
        std::vector<Data *> *trainingData;
        std::vector<Data *> *testData;
        std::vector<Data *> *validationData;
    public:
        std::vector<Data*>* getTrainingData();
        std::vector<Data*>* getTestData();
        std::vector<Data*>* getValidationData();
        void setTrainingData(std::vector<Data *> *vect);
        void setTestData(std::vector<Data *> *vect);
        void setValidationData(std::vector<Data *> *vect);
};