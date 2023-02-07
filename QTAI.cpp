#include "QTAI.h"

QTAI::QTAI(QWidget *parent) : QMainWindow(parent)
{
    etl = new ETL();
    ui.setupUi(this);

    setAcceptDrops(true);
    label = new QLabel("Drop files here", this);
    label->setGeometry(QRect(QPoint(width() / 2 - width() / 3, 0), QSize(width() / 3, width() / 3)));
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
}

QTAI::~QTAI()
{
}

void QTAI::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) {
        e->accept();
    } else {
        e->ignore();
    }
}

void QTAI::dropEvent(QDropEvent* e) {
    for (const QUrl& url : e->mimeData()->urls()) {
        std::ifstream file(url.toLocalFile().toStdString());
        if (file.is_open()) {
            label->setText(QString::fromStdString("Data file:\n" + url.toLocalFile().toStdString()));
        }
        else {

        }
    }
}