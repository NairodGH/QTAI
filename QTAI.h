#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTAI.h"

class QTAI : public QMainWindow
{
    Q_OBJECT

    public:
        QTAI(QWidget *parent = nullptr);
        ~QTAI();

    private:
        Ui::QTAIClass ui;
};
