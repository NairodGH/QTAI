#include "QTAI.h"

QTAI::QTAI(QWidget *parent) : QMainWindow(parent)
{
    //create ETL
    etl = new ETL();
    QObject::connect(etl, &ETL::etlDataFinished, this, &QTAI::handleData);
    QObject::connect(etl, &ETL::etlLabelsFinished, this, &QTAI::handleLabels);

    //setup ui
    resize(1280, 720);
    setPalette(QPalette("#f0f0ff"));

    //drag & drop
    setAcceptDrops(true);
    data = new QLabel("Waiting for the data file (first)", this);
    data->setGeometry(QRect(QPoint(width() / 3, 0), QSize(width() / 3, height() / 4)));
    data->setAlignment(Qt::AlignCenter);
    data->setWordWrap(true);
    dataInfo = new QLabel("", this);
    dataInfo->setGeometry(QRect(QPoint(width() / 3, height() / 4), QSize(width() / 3, height() / 4)));
    dataInfo->setAlignment(Qt::AlignCenter);
    dataInfo->setWordWrap(true);
    labels = new QLabel("Waiting for the labels file (second)", this);
    labels->setGeometry(QRect(QPoint(width() / 3, height() / 2), QSize(width() / 3, height() / 4)));
    labels->setAlignment(Qt::AlignCenter);
    labels->setWordWrap(true);
    labelsInfo = new QLabel("", this);
    labelsInfo->setGeometry(QRect(QPoint(width() / 3, height() - height() / 4), QSize(width() / 3, height() / 4)));
    labelsInfo->setAlignment(Qt::AlignCenter);
    labelsInfo->setWordWrap(true);

    //KNN
    QPushButton *leftButton = new QPushButton("KNN", this);
    leftButton->setGeometry(QRect(QPoint(0, 0), QSize(width() / 3, height() / 4)));
    connect(leftButton, &QPushButton::clicked, this, &QTAI::startKNN);
    knnInfos1 = new QLabel("K-Nearest Neighbors (KNN) is a supervised learning machine learning alorithm. It tries to guess the current image's correct number by comparing it to it's k most similar neighbors (and their labels).", this);
    knnInfos1->setGeometry(QRect(QPoint(0, height() / 4), QSize(width() / 3, height() / 4)));
    knnInfos1->setAlignment(Qt::AlignCenter);
    knnInfos1->setWordWrap(true);
    knnCurentNumber = new QLabel(this);
    knnCurentNumber->setGeometry(QRect(QPoint(0, height() / 2), QSize(width() / 3, height() / 4)));
    knnCurentNumber->setAlignment(Qt::AlignCenter);
    knnInfos2 = new QLabel("", this);
    knnInfos2->setGeometry(QRect(QPoint(0, height() - height() / 4), QSize(width() / 3, height() / 4)));
    knnInfos2->setAlignment(Qt::AlignCenter);
    knnInfos2->setWordWrap(true);

    //KMC
    QPushButton *rightButton = new QPushButton("KMC", this);
    rightButton->setGeometry(QRect(QPoint(width() - width() / 3, 0), QSize(width() / 3, height() / 4)));
    connect(rightButton, &QPushButton::clicked, this, &QTAI::startKMC);
    kmcInfos1 = new QLabel("K-Means Clustering (KMC) is an unsupervised learning machine learning algorithm. It tries to put together similar images (expecting them to have the same numbers) in k clusters", this);
    kmcInfos1->setGeometry(QRect(QPoint(width() - width() / 3, height() / 4), QSize(width() / 3, height() / 4)));
    kmcInfos1->setAlignment(Qt::AlignCenter);
    kmcInfos1->setWordWrap(true);
    kmcCurentNumber = new QLabel(this);
    kmcCurentNumber->setGeometry(QRect(QPoint(width() - width() / 3, height() / 2), QSize(width() / 3, height() / 4)));
    kmcCurentNumber->setAlignment(Qt::AlignCenter);
    kmcInfos2 = new QLabel("", this);
    kmcInfos2->setGeometry(QRect(QPoint(width() - width() / 3, height() - height() / 4), QSize(width() / 3, height() / 4)));
    kmcInfos2->setAlignment(Qt::AlignCenter);
    kmcInfos2->setWordWrap(true);
}

void QTAI::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void QTAI::dropEvent(QDropEvent* event)
{
    for (const QUrl& url : event->mimeData()->urls()) {
        etl->setPath(url.toLocalFile().toStdString());
        etl->start();
        etl->wait();
    }
}

void QTAI::startKNN()
{
    if (!etl->getValidationData()->empty() && 
        etl->getValidationData()->at(0)->getLabel() != 10) {
        KNN* knn = new KNN(1, etl);
        QObject::connect(knn, &KNN::knnProgress, this, &QTAI::handleKNN);
        QObject::connect(knn, &KNN::finished, knn, &QTAI::deleteLater);
        knn->start();
    }
}

void QTAI::startKMC()
{
    if (!etl->getValidationData()->empty() && 
        etl->getValidationData()->at(0)->getLabel() != 10) {
        KMC* kmc = new KMC(1, etl);
        QObject::connect(kmc, &KMC::kmcProgress, this, &QTAI::handleKMC);
        QObject::connect(kmc, &KMC::finished, kmc, &QTAI::deleteLater);
        kmc->start();
    }
}

void QTAI::handleKNN(knn_t infos)
{
    knnCurentNumber->setPixmap(QPixmap::fromImage(QImage(infos.image->data(), 28, 28, QImage::Format_Grayscale8).scaled(168, 168, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    knnInfos2->setText("Current number: " + QString::number(infos.actualNumber) + "\nGuessed number: " + QString::number(infos.guessedNumber) + "\nCurrent performance for k = " + QString::number(infos.k) + ": " + QString::number(infos.performance, 10, 0) + "%");
}

void QTAI::handleKMC(kmc_t infos)
{
    kmcInfos2->setText("Current performance for k = " + QString::number(infos.k) + ": " + QString::number(infos.performance, 10, 0) + "%");
}

void QTAI::handleData(data_t infos)
{
    data->setText(QString::fromStdString("Data file path:\n" + infos.path));
    dataInfo->setText("Total size of dataset:\n" + QString::number(infos.size) + "\nWidth of each image:\n" + QString::number(infos.width) + "\nHeight of each image:\n" + QString::number(infos.height));
}

void QTAI::handleLabels(labels_t infos)
{
    labels->setText(QString::fromStdString("Labels file path:\n" + infos.path));
    labelsInfo->setText("Number of classes:\n" + QString::number(infos.classCounts) + "\nSize of training dataset:\n" + QString::number(infos.trainingSize) + "\nSize of test dataset:\n" + QString::number(infos.testSize) + "\nSize of validation dataset:\n" + QString::number(infos.validationSize));
}