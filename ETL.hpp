#pragma once

#include "Data.hpp"

enum type {
    INVALID,
    DATA,
    LABELS
};

class ETL
{
    private:
        std::vector<Data *> *dataArray; // all of the Data
        std::vector<Data *> *trainingData;
        std::vector<Data *> *testData;
        std::vector<Data *> *validationData;
        int classCounts;
        int featureVectorSize;
        std::map<uint8_t, int> classMap;

    public:
        const double TRAIN_SET_PERCENT = 0.75;
        const double TEST_SET_PERCENT = 0.20;
        const double VALID_SET_PERCENT = 0.05;

        ETL();
        ~ETL();

        void readData(FILE *f);
        void readLabels(FILE *f);
        void splitData();
        void countClasses();

        int getClassCounts();
        int getDataArraySize();
        int getTrainingDataSize();
        int getTestDataSize();
        int getValidationSize();

        uint32_t fixEndianness(const unsigned char* bytes);

        std::vector<Data *> * getTrainingData();
        std::vector<Data *> * getTestData();
        std::vector<Data *> * getValidationData();

        type getFileType(std::string path);
};
