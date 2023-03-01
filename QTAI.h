#pragma once

#include <QtWidgets>
#include "ui_QTAI.h"
#include "KNN.hpp"
#include "ETL.hpp"
#include "Kmeans.hpp"

class MyThread : public QThread {
    Q_OBJECT
    void run() override {
        for (;;) {
            msleep(1000);
            emit progressChanged(0);
        }
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

public slots:
    void startThread() {
        MyThread* thread = new MyThread();
        QObject::connect(thread, &MyThread::progressChanged, this, &QTAI::handleProgress);
        QObject::connect(thread, &MyThread::finished, thread, &QTAI::deleteLater);
        thread->start();
    }

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