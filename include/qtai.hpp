#pragma once

#include "etl.hpp"

class QTAI : public QMainWindow {
    // unique (in memory, cant be copied) class with dynamic properties, signals, and slots
    Q_OBJECT
    // exposes to qml bindings for reactive UI updates
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit QTAI(QWidget* parent = nullptr);
    ~QTAI() override;

    // can be called from QML
    Q_INVOKABLE void onSwitchUI();
    Q_INVOKABLE void onContentClicked();

    int getCurrentIndex() const { return currentIndex; }
    void setCurrentIndex(int index);
    ETL* etl;
    std::vector<QWidget*> widgets;
    std::array<QThread*, 2> algorithms {};

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void currentIndexChanged();

private slots:
    void onLoadingComplete(DatasetInfo info);

private:
    void onRestart(QThread* algo);

    void createWidgets();
    void createQML();

    static constexpr std::array<const char*, 2> names{ "KNN", "KMC" };
    DatasetInfo datasetInfo;
    bool dataLoaded = false;
    bool isQMLMode = false;
    int currentIndex = 0;

    // everything Qt type inherit from QObject, which deletes its copy and move constructors so must be pointer
    QStackedWidget* stackedWidget;
    QWidget* widgetsPage;
    QWidget* qmlPage;
    QQuickWidget* quickWidget;
    QStackedWidget* contentStack;
    QPushButton* switchButton;
    std::vector<QPushButton*> tabButtons;
    std::vector<QLabel*> statusLabels;
};