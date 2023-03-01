#include "QTAI.h"

QTAI::QTAI(QWidget *parent) : QMainWindow(parent)
{
    etl = new ETL();
    ui.setupUi(this);

    setAcceptDrops(true);
    data = new QLabel("Waiting for the data file", this);
    data->setGeometry(QRect(QPoint(width() / 2 - width() / 3 / 2, 0), QSize(width() / 3, width() / 3)));
    data->setAlignment(Qt::AlignCenter);
    data->setWordWrap(true);

    labels = new QLabel("Waiting for the labels file (only for KNN)", this);
    labels->setGeometry(QRect(QPoint(width() / 2 - width() / 3 / 2, width() / 3), QSize(width() / 3, width() / 3)));
    labels->setAlignment(Qt::AlignCenter);
    labels->setWordWrap(true);

    QPushButton *leftButton = new QPushButton("KNN", this);
    leftButton->setGeometry(QRect(QPoint(0, 0), QSize(width() / 3, width() / 3)));
    connect(leftButton, &QPushButton::clicked, this, &QTAI::startKNN);
    QLabel *knnCurentNumber = new QLabel(this);
    knnCurentNumber->setGeometry(QRect(QPoint(0, width() / 3), QSize(width() / 3, 2 * width() / 3)));

    QPushButton *rightButton = new QPushButton("KMeans", this);
    rightButton->setGeometry(QRect(QPoint(width() - width() / 3, 0), QSize(width() / 3, width() / 3)));
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
        KNNThread* thread = new KNNThread(etl);
        QObject::connect(thread, &KNNThread::progress, this, &QTAI::handleProgress);
        QObject::connect(thread, &KNNThread::finished, thread, &QTAI::deleteLater);
        thread->start();
    }
}

void QTAI::startKmeans()
{
    if (data->text()[0] == 'D') {
        
    }
}