#include "QTAI.h"

QTAI::QTAI(QWidget *parent) : QMainWindow(parent)
{
    etl = new ETL();
    ui.setupUi(this);

    setAcceptDrops(true);
    labels = new QLabel("Waiting for the labels file (only for KNN)", this);
    labels->setGeometry(QRect(QPoint(width() / 2 - width() / 3 / 2, 0), QSize(width() / 3, width() / 3)));
    labels->setAlignment(Qt::AlignCenter);
    labels->setWordWrap(true);
    data = new QLabel("Waiting for the data file", this);
    data->setGeometry(QRect(QPoint(width() / 2 - width() / 3 / 2, width() / 3), QSize(width() / 3, width() / 3)));
    data->setAlignment(Qt::AlignCenter);
    data->setWordWrap(true);
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
        if (file == LABELS)
            labels->setText(QString::fromStdString("Labels file:\n" + url.toLocalFile().toStdString()));
        else if (file == DATA)
            data->setText(QString::fromStdString("Data file:\n" + url.toLocalFile().toStdString()));
    }
}