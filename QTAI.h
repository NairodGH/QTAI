#pragma once

#include "KNN.hpp"
#include "KMC.hpp"

class QTAI : public QMainWindow
{
    Q_OBJECT

public:
    QTAI(QWidget *parent = nullptr);

private:
    QLabel* data;
    QLabel* dataInfo;
    QLabel* labels;
    QLabel* labelsInfo;

    ETL* etl;

    void startKNN();
    void handleKNN(knn_t infos);
    QLabel* knnInfos1;
    QLabel* knnCurentNumber;
    QLabel* knnInfos2;

    void startKMC();
    void handleKMC(kmc_t infos);
    QLabel* kmcInfos1;
    QLabel* kmcCurentNumber;
    QLabel* kmcInfos2;

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
};