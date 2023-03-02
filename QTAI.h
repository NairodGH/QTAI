#pragma once

#include "KNN.hpp"
#include "Kmeans.hpp"

class KmeansThread : public QThread {
    Q_OBJECT
    void run() override {
        /*double performance = 0;
        double best_performance = 0;
        int best_k = 1;
        for (int k = 1; k < etl->getTrainingData()->size() * 0.1; k++) {
            Kmeans* km = new Kmeans(k);
            km->setTrainingData(etl->getTrainingData());
            km->setTestData(etl->getTestData());
            km->setValidationData(etl->getValidationData());
            km->initClusters();
            km->train();
            performance = km->validate();
            printf("Current Perforamnce @ K = %d: %.2f\n", k, performance);
            if (performance > best_performance) {
                best_performance = performance;
                best_k = k;
            }
        }
        Kmeans* km = new Kmeans(best_k);
        km->setTrainingData(etl->getTrainingData());
        km->setTestData(etl->getTestData());
        km->setValidationData(etl->getValidationData());
        km->initClusters();
        km->train();
        printf("Overall Performance: %.2f\n", km->test());
        emit progressChanged(0);*/
    }

signals:
    void progressChanged(int data);
};

class QTAI : public QMainWindow
{
    Q_OBJECT

public:
    QTAI(QWidget *parent = nullptr);
    ~QTAI();

private:
    QLabel* data;
    QLabel* dataInfo;
    QLabel* labels;
    QLabel* labelsInfo;

    QLabel* knnInfos1;
    QLabel* knnCurentNumber;
    QLabel* knnInfos2;
    ETL *etl;
    void startKNN();
    void startKmeans();
    void handleKNN(knn_t infos);

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
};