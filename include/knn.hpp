#pragma once

#include "main.hpp"

class KNN : public QThread {
    Q_OBJECT

public:
    KNN(QObject* parent);

    Q_INVOKABLE void onSkip();
    Q_INVOKABLE void onPause();

signals:
    void progress(QVariantMap info);
    void trainComplete(QVariantMap result);
    void testComplete(int bestK, double bestAccuracy, double accuracy);
    void restart(QThread* self);

private:
    void onProgress(const QVariantMap& info);
    void onTrainComplete(const QVariantMap& result);
    void onTestComplete(int bestK, double bestAccuracy, double accuracy);

    void setupKNNPage(QWidget* page);

    void run() override;
    double getPrecision(bool isValidation);
    void computeDistances(const Data &queryPoint);
    
    static constexpr std::array<int, 8> kValues = {1, 3, 5, 7, 9, 11, 13, 15}; // k values we train on, ascending order
    int k;
    bool skip;
    bool paused;
    QMutex pauseMutex;
    QWaitCondition pauseCondition;
    std::vector<Data> trainingData;
    std::vector<Data> validationData;
    std::vector<Data> testData;
    std::vector<Data*> sortedTrainingData;

    QWidget* visualizationWidget;
    QLabel* statusLabel;
    QLabel* kValueLabel;
    QLabel* currentImage;
    QGridLayout* neighborsGrid;
    QPushButton* skipButton;
    QPushButton* pauseButton;
    QPushButton* restartButton;
    QGridLayout* resultsLayout;
    QWidget* resultsContainer;
};