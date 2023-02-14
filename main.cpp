#include <QtWidgets/QApplication>
#include "QTAI.h"

#define DEBUG

#ifdef DEBUG
#include <windows.h>
#include <iostream>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTAI qtai;

    #ifdef DEBUG
        if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole())
            freopen("CONOUT$", "w", stdout);
    #endif
    
    ETL *dh = new ETL();
    dh->getFileType("train-images-idx3-ubyte");
    dh->getFileType("train-labels-idx1-ubyte");
    KNN *nearest = new KNN();
    nearest->setK(1);
    nearest->setTrainingData(dh->getTrainingData());
    nearest->setTestData(dh->getTestData());
    nearest->setValidationData(dh->getValidationData());
    double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for(int k = 1; k <= 3; k++)
    {
        if(k == 1) {
            performance = nearest->validate();
            best_performance = performance;
        } else {
            nearest->setK(k);
            performance = nearest->validate();
            if(performance > best_performance) {
                best_performance = performance;
                best_k = k;
            }
        }
    }
    nearest->setK(best_k);
    nearest->test();

    /*double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for(int k = 1; k < dh->getTrainingData()->size()*0.1; k++) {
        Kmeans *km = new Kmeans(k);
        km->setTrainingData(dh->getTrainingData());
        km->setTestData(dh->getTestData());
        km->setValidationData(dh->getValidationData());
        km->initClusters();
        km->train();
        performance = km->validate();
        printf("Current Perforamnce @ K = %d: %.2f\n", k, performance);
        if(performance > best_performance) {
            best_performance = performance;
            best_k = k;
        }
    }
    Kmeans *km = new Kmeans(best_k);
    km->setTrainingData(dh->getTrainingData());
    km->setTestData(dh->getTestData());
    km->setValidationData(dh->getValidationData());
    km->initClusters();
    km->train();
    printf("Overall Performance: %.2f\n",km->test());*/

    qtai.show();
    return app.exec();
}
