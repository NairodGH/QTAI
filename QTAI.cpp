#include "QTAI.h"

QTAI::QTAI(QWidget *parent) : QMainWindow(parent)
{
    //create ETL
    etl = new ETL();

    //setup ui
    this->resize(1280, 720);

    //drag & drop
    setAcceptDrops(true);
    data = new QLabel("Waiting for the data file", this);
    data->setGeometry(QRect(QPoint(width() / 3, 0), QSize(width() / 3, height() / 4)));
    data->setAlignment(Qt::AlignCenter);
    data->setWordWrap(true);
    dataInfo = new QLabel("", this);
    dataInfo->setGeometry(QRect(QPoint(width() / 3, height() / 4), QSize(width() / 3, height() / 4)));
    dataInfo->setAlignment(Qt::AlignCenter);
    dataInfo->setWordWrap(true);
    labels = new QLabel("Waiting for the labels file (only for KNN)", this);
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
    knnInfos1 = new QLabel("K-Nearest Neighbors (KNN) is a supervised learning machine learning alorithm. It needs both the data and labels files as it tries to guess the current image's correct number by comparing it to it's k most similar neighbors (and their labels).", this);
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

    //Kmeans
    QPushButton *rightButton = new QPushButton("KMeans", this);
    rightButton->setGeometry(QRect(QPoint(width() - width() / 3, 0), QSize(width() / 3, height() / 4)));
    connect(rightButton, &QPushButton::clicked, this, &QTAI::startKmeans);
    QLabel *kmeansCurentNumber = new QLabel(this);
    kmeansCurentNumber->setGeometry(QRect(QPoint(width() - width() / 3, width() / 3), QSize(width() / 3, 2 * width() / 3)));
}

QTAI::~QTAI()
{
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
    type file;

    for (const QUrl& url : event->mimeData()->urls()) {
        file = etl->getFileType(url.toLocalFile().toStdString());
        if (file == DATA)
            data->setText(QString::fromStdString("Data file:\n" + url.toLocalFile().toStdString()));
        else if (file == LABELS)
            labels->setText(QString::fromStdString("Labels file:\n" + url.toLocalFile().toStdString()));
        std::cout << etl->getDataArraySize() << std::endl;
    }
}

void QTAI::startKNN()
{
    if (data->text()[0] == 'D' && labels->text()[0] == 'L') {
        KNN* knn = new KNN(1, etl);
        QObject::connect(knn, &KNN::progress, this, &QTAI::handleKNN);
        QObject::connect(knn, &KNN::finished, knn, &QTAI::deleteLater);
        knn->start();
    }
}

void QTAI::startKmeans()
{
    if (data->text()[0] == 'D') {
        
    }
}

void QTAI::handleKNN(knn_t infos)
{
    knnInfos1->setText("Current number: " + QString::number(infos.actualNumber) + "\nGuessed number: " + QString::number(infos.guessedNumber));
    knnCurentNumber->setPixmap(QPixmap::fromImage(QImage(infos.image->data(), 28, 28, QImage::Format_Grayscale8).scaled(168, 168, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    knnInfos2->setText("Current performance for k = " + QString::number(infos.k) + ": " + QString::number(infos.performance, 10, 0) + "%");
}