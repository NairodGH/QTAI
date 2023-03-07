#include "ETL.hpp"

ETL::ETL()
{
    dataArray = new std::vector<Data *>;
    trainingData = new std::vector<Data *>;
    testData = new std::vector<Data *>;
    validationData = new std::vector<Data *>;
}

void ETL::run() {
    FILE* file = fopen(path.c_str(), "rb");
    unsigned char bytes[4];
    uint32_t header;

    if (file) {
        std::cout << path << " is accessible" << std::endl;
        if (fread(bytes, sizeof(bytes), 1, file))
            header = fixEndianness(bytes);
        else
            return;
        std::cout << path << " could be read" << std::endl;
        if (header == 2049 && !dataArray->empty()) {
            std::cout << "labels start" << std::endl;
            readLabels(file);
            std::cout << "readLabels" << std::endl;
            countClasses();
            std::cout << "countClasses" << std::endl;
            splitData();
            std::cout << "splitData" << std::endl;
            std::cout << "emit" << std::endl;
        }
        if (header == 2051) {
            std::cout << "data start" << std::endl;
            readData(file);
            std::cout << "readData" << std::endl;
            std::cout << "emit" << std::endl;
        }
    }
}

void ETL::readData(FILE *f)
{
    uint32_t header[3];
    unsigned char bytes[4];

    for (size_t i = 0; i < 3; i++)
        if (fread(bytes, sizeof(bytes), 1, f))
            header[i] = fixEndianness(bytes);
    uint32_t imageSize = header[1] * header[2];
    std::cout << header[0] << " " << header[1] << " " << header[2] << " " << std::endl;
    for(size_t i = 0; i < header[0]; i++) {
        Data *d = new Data();
        d->setFeatureVector(new std::vector<uint8_t>());
        uint8_t element[1];
        for(int j = 0; j < imageSize; j++)
            if(fread(element, sizeof(element), 1, f))
                d->appendToFeatureVector(element[0]);
        dataArray->push_back(d);
    }
    emit etlDataFinished(data_t{ path, header[0], header[1], header[2] });
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
}

void ETL::splitData()
{
    size_t trainSize = dataArray->size() * TRAIN_SET_PERCENT;
    size_t testSize = dataArray->size() * TEST_SET_PERCENT;
    size_t validationSize = dataArray->size() * VALID_SET_PERCENT;
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
    emit etlLabelsFinished(labels_t{ path, classCounts, trainSize, testSize, validationSize });
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
}

uint32_t ETL::fixEndianness(const unsigned char *bytes)
{
    return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

void ETL::setPath(std::string path)
{
    this->path = path;
}