#pragma once

#include "Sets.hpp"
#include <QtWidgets>
#include "ETL.hpp"

// Complexity: O(k*n) where k is the number of neighbors and N is the size of training Data

class KNN : public QThread, public Sets {
    Q_OBJECT

public:
    KNN(int k, ETL *etl);
    void run() override;
    double getPrecision(bool isValidation);
    void findKnearest(Data* queryPoint);
    int findMostFrequentClass();
    double distance(Data* queryPoint, Data* input);

signals:
    void progress(knn_t infos);

private:
    int k;
    std::vector<Data*>* neighbors;
};