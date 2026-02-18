#pragma once

#include "main.hpp"

struct DatasetInfo {
    size_t trainCount;
    size_t valCount;
    size_t testCount;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t classCount;
};

struct Data {
    std::vector<uint8_t> featureVector;
    uint8_t label;
    double distance;
};

class ETL : public QThread {
    Q_OBJECT

public:
    void run() override;
    
    std::vector<Data> trainingData;
    std::vector<Data> validationData;
    std::vector<Data> testData;

signals:
    void loadingComplete(DatasetInfo info);

private:
    void readImages(QFile &f, std::vector<Data> &target, uint32_t& rows, uint32_t& cols);
    int readLabels(QFile &f, std::vector<Data> &target);
    uint32_t fixEndianness(const unsigned char* bytes);
};