#include <QtWidgets/QApplication>
#include "QTAI.h"
#include "KNN.hpp"
#include "ETL.hpp"
#include "Kmeans.hpp"

#define DEBUG

#ifdef DEBUG
#include <windows.h>
#include <iostream>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTAI qtai;
    ETL *dh = new ETL();

    #ifdef DEBUG
        if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole())
            freopen("CONOUT$", "w", stdout);
    #endif
    dh->read_input_data("train-images-idx3-ubyte");
    dh->read_label_data("train-labels-idx1-ubyte");
    dh->count_classes();
    dh->split_data();
    KNN *nearest = new KNN();
    nearest->set_k(1);
    nearest->set_training_data(dh->get_training_data());
    nearest->set_test_data(dh->get_test_data());
    nearest->set_validation_data(dh->get_validation_data());
    double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for(int k = 1; k <= 3; k++)
    {
        if(k == 1) {
            performance = nearest->validate_perforamnce();
            best_performance = performance;
        } else {
            nearest->set_k(k);
            performance = nearest->validate_perforamnce();
            if(performance > best_performance) {
                best_performance = performance;
                best_k = k;
            }
        }
    }
    nearest->set_k(best_k);
    nearest->test_performance();

    /*double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for(int k = 1; k < dh->get_training_data()->size()*0.1; k++) {
        Kmeans *km = new Kmeans(k);
        km->set_training_data(dh->get_training_data());
        km->set_test_data(dh->get_test_data());
        km->set_validation_data(dh->get_validation_data());
        km->init_clusters();
        km->train();
        performance = km->validate();
        printf("Current Perforamnce @ K = %d: %.2f\n", k, performance);
        if(performance > best_performance) {
            best_performance = performance;
            best_k = k;
        }
    }
    Kmeans *km = new Kmeans(best_k);
    km->set_training_data(dh->get_training_data());
    km->set_test_data(dh->get_test_data());
    km->set_validation_data(dh->get_validation_data());
    km->init_clusters();
    km->train();
    printf("Overall Performance: %.2f\n",km->test());*/

    qtai.show();
    return app.exec();
}
