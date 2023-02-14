#include "ETL.hpp"

ETL::ETL()
{
    dataArray = new std::vector<Data *>;
    trainingData = new std::vector<Data *>;
    testData = new std::vector<Data *>;
    validationData = new std::vector<Data *>;
}

ETL::~ETL()
{
}

void ETL::readData(FILE *f)
{
    uint32_t header[3];
    unsigned char bytes[4];

    for (size_t i = 0; i < 3; i++)
        if (fread(bytes, sizeof(bytes), 1, f))
            header[i] = fixEndianness(bytes);
    printf("Done getting file header.\n");
    uint32_t imageSize = header[1] * header[2];
    for(size_t i = 0; i < header[0]; i++) {
        Data *d = new Data();
        d->setFeatureVector(new std::vector<uint8_t>());
        uint8_t element[1];
        for(int j = 0; j < imageSize; j++)
            if(fread(element, sizeof(element), 1, f))
                d->appendToFeatureVector(element[0]);
        dataArray->push_back(d);
    }
    featureVectorSize = dataArray->at(0)->getFeatureVector()->size();
    printf("Successfully read %zu Data entries.\n", dataArray->size());
}

void ETL::readLabels(FILE *f)
{
    uint32_t numImages = 0;
    unsigned char bytes[4];

    fread(bytes, sizeof(bytes), 1, f);
    numImages = fixEndianness(bytes);
    for(size_t j = 0; j < numImages; j++) {
        uint8_t element[1];
        if(fread(element, sizeof(element), 1, f))
            dataArray->at(j)->setLabel(element[0]);
    }
    printf("Done getting Label header.\n");
}

void ETL::splitData()
{
    int trainSize = dataArray->size() * TRAIN_SET_PERCENT;
    int testSize = dataArray->size() * TEST_SET_PERCENT;
    int validationSize = dataArray->size() * VALID_SET_PERCENT;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(dataArray->begin(), dataArray->end(), g);
    int count = 0;
    int index = 0;

    for (; count < trainSize; count++)
        trainingData->push_back(dataArray->at(index++));
    count = 0;
    for (; count < testSize; count++)
        testData->push_back(dataArray->at(index++));
    count = 0;
    for (; count < validationSize; count++)
        validationData->push_back(dataArray->at(index++));
    printf("Training Data Size: %lu.\n", trainingData->size());
    printf("Test Data Size: %lu.\n", testData->size());
    printf("Validation Data Size: %lu.\n", validationData->size());
}

void ETL::countClasses()
{
    int count = 0;
    for(unsigned i = 0; i < dataArray->size(); i++) {
        if(classMap.find(dataArray->at(i)->getLabel()) == classMap.end()) {
            classMap[dataArray->at(i)->getLabel()] = count;
            dataArray->at(i)->setEnumeratedLabel(count);
            count++;
        }
    }
    classCounts = count;
    printf("Successfully Extracted %d Unique Classes.\n", classCounts);
}

int ETL::getClassCounts()
{
    return classCounts;
}

int ETL::getDataArraySize()
{
    return dataArray->size();
}

int ETL::getTrainingDataSize()
{
    return trainingData->size();
}

int ETL::getTestDataSize()
{
    return testData->size();
}

int ETL::getValidationSize()
{
    return validationData->size();
}

uint32_t ETL::fixEndianness(const unsigned char *bytes)
{
    return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

std::vector<Data *> *ETL::getTrainingData()
{
    return trainingData;
}

std::vector<Data *> *ETL::getTestData()
{
    return testData;
}

std::vector<Data *> *ETL::getValidationData()
{
    return validationData;
}

type ETL::getFileType(std::string path)
{
    FILE *file = fopen(path.c_str(), "rb");
    unsigned char bytes[4];
    uint32_t header;

    if (file) {
        if (fread(bytes, sizeof(bytes), 1, file))
            header = fixEndianness(bytes);
        else
            return INVALID;
        if (header == 2049) {
            if (!dataArray->empty())
                readLabels(file);
            return LABELS;
        }
        else if (header == 2051) {
            readData(file);
            countClasses();
            splitData();
            return DATA;
        }
        else
            return INVALID;
    } else
        return INVALID;
}