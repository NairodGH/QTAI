#pragma once

#include <QtWidgets>
#include "ui_QTAI.h"
#include "KNN.hpp"
#include "ETL.hpp"
#include "Kmeans.hpp"

class KNNThread : public QThread {
Q_OBJECT

public:
    KNNThread(ETL* etl) {
        this->etl = etl;
        knn = new KNN(1);
        knn->setTrainingData(etl->getTrainingData());
        knn->setTestData(etl->getTestData());
        knn->setValidationData(etl->getValidationData());
    }

    void run() override {
        double performance = 0;
        double best_performance = 0;
        int best_k = 1;
        for(int k = 1; k <= 3; k++) {
            if(k == 1) {
                performance = getPrecision(1);
                best_performance = performance;
            } else {
                knn->setK(k);
                performance = getPrecision(1);
                if(performance > best_performance) {
                    best_performance = performance;
                    best_k = k;
                }
            }
            std::cout << "Validation performance for k =  " << k << ": " << performance << std::endl;
        }
        knn->setK(best_k);
        getPrecision(0);
    }

    double getPrecision(bool isValidation) {
        double currentPerformance = 0;
        int count = 0;
        int dataIndex = 0;
        std::vector<Data *> *set = isValidation ? knn->getValidationData() : knn->getTestData();

        for (Data *queryPoint : *set) {
            knn->findKnearest(queryPoint);
            int prediction = knn->findMostFrequentClass();
            dataIndex++;
            if(prediction == queryPoint->getLabel())
                count++;
            std::cout << "Guessed " << prediction << " for " << +queryPoint->getLabel() << std::endl;
            std::cout << "Current " << (isValidation ? "validation" : "test") << " performance: " << (double)count * 100. / (double)dataIndex << "%" << std::endl;
        }
        currentPerformance = (double)count * 100. / (double)(set->size());
        return currentPerformance;
    }

signals:
    void progress(int data);

private:
    ETL* etl;
    KNN* knn;
};

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
    Ui::QTAIClass ui;
    QLabel* labels;
    QLabel* data;
    ETL *etl;
    void startKNN();
    void startKmeans();
    void handleProgress(int data) {
        std::cout << data << "\n";
    }

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
};