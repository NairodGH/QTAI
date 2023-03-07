#pragma once

#include <QtWidgets>
#include "Sets.hpp"

class ETL : public QThread, public Sets {
    Q_OBJECT

public:
    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALID_SET_PERCENT = 0.05;

    ETL();
    void run() override;
    void readData(FILE* f);
    void readLabels(FILE* f);
    void splitData();
    void countClasses();
    uint32_t fixEndianness(const unsigned char* bytes);
    void setPath(std::string path);

signals:
    void etlDataFinished(data_t infos);
    void etlLabelsFinished(labels_t infos);

private:
    std::vector<Data*>* dataArray; // all of the Data
    int classCounts;
    std::map<int, int> classMap;
    std::string path;
};