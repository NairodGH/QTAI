#include "qtai.hpp"
#include "knn.hpp"

QTAI::QTAI(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("QTAI");
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    createWidgets();
    createQML();

    etl = new ETL();
    connect(etl, &ETL::loadingComplete, this, &QTAI::onLoadingComplete);
    etl->start();
}

QTAI::~QTAI()
{
    // no need for deletes since QThreads are childs of qtai so will get auto claned up
    for (auto& algorithm : algorithms) {
        if (algorithm) {
            algorithm->forceWake(); // in case it was paused, would cause error
            algorithm->requestInterruption();
            algorithm->quit();
            if (!algorithm->wait(2000)) {
                algorithm->terminate();
                algorithm->wait();
            }
        }
    }

    if (etl) {
        etl->requestInterruption();
        etl->quit();
        if (!etl->wait(2000)) {
            etl->terminate();
            etl->wait();
        }
    }
}

void QTAI::setCurrentIndex(int index)
{
    if (currentIndex == index)
        return;
    contentStack->setCurrentIndex(currentIndex = index);
    tabButtons[currentIndex]->setChecked(true);
    emit currentIndexChanged();
}

void QTAI::onLoadingComplete(DatasetInfo info)
{
    for (int i = 0; i < algorithms.size(); ++i) {
        if (!algorithms[i]) {
            loadingStatus
                = QString("Dataset Loaded Successfully\n\nTraining samples: %1\nValidation samples: %2\nTest samples: "
                          "%3\nDimensions: %4x%5\nClasses: %6\n\nClick to start training %7")
                      .arg(info.trainCount)
                      .arg(info.valCount)
                      .arg(info.testCount)
                      .arg(info.imageWidth)
                      .arg(info.imageHeight)
                      .arg(info.classCount);
            statusLabels[i]->setText(loadingStatus.arg(names[i]));
        }
    }
    emit loadingStatusChanged();
}

void QTAI::onContentClicked()
{
    if (etl->testData.empty() || currentIndex > 0)
        return;

    // delete clickable area, easier than bothering with widget stretching shenanigans
    for (QObject* child : widgets[currentIndex]->children())
        if (QWidget* widget = qobject_cast<QWidget*>(child))
            widget->deleteLater();

    if (currentIndex == 0) {
        algorithms[0] = new KNN(this);
        emit algorithmsChanged();
        connect(static_cast<KNN*>(algorithms[0]), &KNN::restart, this, &QTAI::onRestart);
        algorithms[0]->start();
    }
}

void QTAI::createWidgets()
{
    widgetsPage = new QWidget();
    auto* mainLayout = new QVBoxLayout(widgetsPage);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // top bar
    auto* topBar = new QWidget();
    topBar->setStyleSheet("background-color: #2d2d2d;");
    topBar->setFixedHeight(40);
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(5, 5, 5, 5);
    topLayout->setSpacing(0);

    switchButton = new QPushButton("Switch to QML");
    switchButton->setFixedSize(120, 30);
    connect(switchButton, &QPushButton::clicked, this, &QTAI::onSwitchUI);
    topLayout->addWidget(switchButton);
    topLayout->addSpacing(5);

    QString btnStyle = "QPushButton { background-color: #1e1e1e; color: white; border: 1px solid #3d3d3d; "
                       "border-radius: 3px; font-weight: bold; }"
                       "QPushButton:checked { background-color: #404040; border: 1px solid #5d5d5d; }";
    switchButton->setStyleSheet(btnStyle);

    auto* group = new QButtonGroup(this);
    for (int i = 0; i < names.size(); ++i) {
        auto* btn = new QPushButton(names[i]);
        btn->setFixedHeight(30);
        btn->setCheckable(true);
        btn->setChecked(i == currentIndex);
        btn->setStyleSheet(btnStyle);
        tabButtons.push_back(btn);
        group->addButton(btn, i);
        topLayout->addWidget(btn, 1);
        connect(btn, &QPushButton::clicked, [this, i]() { setCurrentIndex(i); });
    }
    mainLayout->addWidget(topBar);

    // content
    contentStack = new QStackedWidget();
    contentStack->setStyleSheet("background-color: #1e1e1e;");

    for (int i = 0; i < names.size(); i++) {
        widgets.push_back(new QWidget());
        widgets[i]->setStyleSheet("background-color: #1e1e1e;");
        auto* layout = new QVBoxLayout(widgets[i]);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        auto* clickableArea = new QWidget();
        clickableArea->setStyleSheet("background-color: transparent;");
        clickableArea->setCursor(Qt::PointingHandCursor);
        clickableArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        clickableArea->installEventFilter(this);

        auto* clickLayout = new QVBoxLayout(clickableArea);
        clickLayout->setAlignment(Qt::AlignCenter);

        auto* container = new QWidget();
        container->setFixedWidth(600);
        auto* vbox = new QVBoxLayout(container);
        auto* label = new QLabel("Initializing...");
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color: white; font-size: 14px;");
        label->setWordWrap(true);
        statusLabels.push_back(label);
        vbox->addWidget(label);
        clickLayout->addWidget(container);

        layout->addWidget(clickableArea);

        contentStack->addWidget(widgets[i]);
    }

    mainLayout->addWidget(contentStack, 1);
    stackedWidget->addWidget(widgetsPage);
}

bool QTAI::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        onContentClicked();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void QTAI::createQML()
{
    qmlPage = new QWidget();
    auto* layout = new QVBoxLayout(qmlPage);
    layout->setContentsMargins(0, 0, 0, 0);
    quickWidget = new QQuickWidget();
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->rootContext()->setContextProperty("QTAI", this);
    quickWidget->setSource(QUrl("qrc:/qt/qml/QTAI/qml/qtai.qml"));
    layout->addWidget(quickWidget);
    stackedWidget->addWidget(qmlPage);
}

void QTAI::onSwitchUI()
{
    isQMLMode = !isQMLMode;
    stackedWidget->setCurrentWidget(isQMLMode ? qmlPage : widgetsPage);
    switchButton->setText(isQMLMode ? "Switch to Widgets" : "Switch to QML");
}

void QTAI::onRestart(QThread* algorithm)
{
    algorithm->requestInterruption();
    algorithm->quit();
    if (!algorithm->wait(2000)) {
        algorithm->terminate();
        algorithm->wait();
    }

    // clean all items from the widget since they'll get recreated
    QWidget* currentWidget = widgets[currentIndex];
    QLayout* layout = currentWidget->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }

    if (qobject_cast<KNN*>(algorithm)) {
        delete algorithm;
        algorithms[0] = nullptr; // equivalent of above's ui clean but for qml with its "active" property on loader
        emit algorithmsChanged();
        algorithms[0] = new KNN(this);
        connect(static_cast<KNN*>(algorithms[0]), &KNN::restart, this, &QTAI::onRestart);
        emit algorithmsChanged();
        algorithms[0]->start();
    }
}