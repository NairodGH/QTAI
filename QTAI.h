#pragma once

#include <QtWidgets>
#include "ui_QTAI.h"
#include "KNN.hpp"
#include "ETL.hpp"
#include "Kmeans.hpp"

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

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
};