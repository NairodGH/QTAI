#include "knn.hpp"
#include "qtai.hpp"
#if defined(__AVX2__)
#include <immintrin.h>
#endif

#pragma region GUI Thread

KNN::KNN(QObject* parent)
    : Algorithm(parent)
    , k(kValues[0])
    , visualizationWidget(nullptr)
    , statusLabel(nullptr)
    , kValueLabel(nullptr)
    , currentImage(nullptr)
    , neighborsGrid(nullptr)
    , skipButton(nullptr)
    , pauseButton(nullptr)
    , resultsLayout(nullptr)
    , restartButton(nullptr)
    , resultsContainer(nullptr)
{
    QTAI* qtai = qobject_cast<QTAI*>(parent);
    if (!qtai) {
        qDebug() << "Error: KNN parent is not QTAI!";
        return;
    }
    // copy ETL's datas locally for use and shuffle them once per instance
    // that means when we'll search for the best k by increasing it, it'll always use the same order so we know accuracy
    // difference is purely due to k, not randomness, it'll still get randomized on restart ofc
    trainingData = qtai->etl->trainingData;
    validationData = qtai->etl->validationData;
    testData = qtai->etl->testData;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(trainingData.begin(), trainingData.end(), g);
    std::shuffle(validationData.begin(), validationData.end(), g);
    std::shuffle(testData.begin(), testData.end(), g);

    // vector of pointers to training data that will get sorted for caching
    sortedTrainingData.reserve(trainingData.size());
    for (Data& d : trainingData) {
        sortedTrainingData.push_back(&d);
    }

    setupKNNPage(qtai->widgets[0]);
    connect(this, &KNN::progress, this, &KNN::onProgress);
    connect(this, &KNN::trainComplete, this, &KNN::onTrainComplete);
    connect(this, &KNN::testComplete, this, &KNN::onTestComplete);
}

void KNN::setupKNNPage(QWidget* page)
{
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(page->layout());
    visualizationWidget = new QWidget();
    auto* visualizationLayout = new QVBoxLayout(visualizationWidget);
    visualizationLayout->setContentsMargins(20, 20, 20, 20);

    // results grid
    auto* resultsContainer = new QWidget();
    resultsContainer->setMaximumHeight(90);
    auto* resultsContainerLayout = new QVBoxLayout(resultsContainer);
    resultsContainerLayout->setContentsMargins(0, 0, 10, 0);
    resultsContainerLayout->setSpacing(0);
    resultsLayout = new QGridLayout();
    resultsLayout->setSpacing(12);
    resultsLayout->setContentsMargins(0, 0, 0, 0);
    resultsLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    auto* kRowLabel = new QLabel("k");
    kRowLabel->setStyleSheet("color: #00aaff; font-size: 11px; font-weight: bold;");
    kRowLabel->setAlignment(Qt::AlignCenter);
    kRowLabel->setMinimumWidth(45);
    kRowLabel->setVisible(false);
    resultsLayout->addWidget(kRowLabel, 0, 0);
    auto* accuracyRowLabel = new QLabel("Accuracy");
    accuracyRowLabel->setStyleSheet("color: #00aaff; font-size: 11px; font-weight: bold;");
    accuracyRowLabel->setAlignment(Qt::AlignCenter);
    accuracyRowLabel->setMinimumWidth(45);
    accuracyRowLabel->setVisible(false);
    resultsLayout->addWidget(accuracyRowLabel, 1, 0);

    for (size_t i = 0; i < kValues.size(); i++) {
        auto* kLabel = new QLabel(QString::number(kValues[i]));
        kLabel->setStyleSheet("color: #aaa; font-size: 12px;");
        kLabel->setAlignment(Qt::AlignCenter);
        kLabel->setMinimumWidth(30);
        kLabel->setVisible(false);
        resultsLayout->addWidget(kLabel, 0, i + 1);

        auto* accuracyLabel = new QLabel("");
        accuracyLabel->setStyleSheet("color: #aaa; font-size: 12px;");
        accuracyLabel->setAlignment(Qt::AlignCenter);
        accuracyLabel->setMinimumWidth(30);
        accuracyLabel->setVisible(false);
        resultsLayout->addWidget(accuracyLabel, 1, i + 1);
    }
    resultsLayout->update();
    resultsContainerLayout->addLayout(resultsLayout);
    resultsContainer = resultsContainer;
    resultsContainer->update();
    visualizationLayout->addWidget(resultsContainer);

    // status and buttons
    auto* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->setSpacing(10);

    statusLabel = new QLabel(QString("Starting validation with k=%1...").arg(k));
    statusLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    topBarLayout->addWidget(statusLabel, 1);

    pauseButton = new QPushButton("Pause");
    pauseButton->setFixedSize(90, 30);
    pauseButton->setStyleSheet("QPushButton { background-color: #404040; color: white; border: 1px solid #5d5d5d; "
                               "border-radius: 3px; font-weight: bold; }"
                               "QPushButton:hover { background-color: #505050; }"
                               "QPushButton:disabled { background-color: #2d2d2d; color: #666; }");
    connect(pauseButton, &QPushButton::clicked, this, &KNN::onPause);
    topBarLayout->addWidget(pauseButton);

    skipButton = new QPushButton("Skip");
    skipButton->setFixedSize(120, 30);
    skipButton->setStyleSheet("QPushButton { background-color: #404040; color: white; border: 1px solid #5d5d5d; "
                              "border-radius: 3px; font-weight: bold; }"
                              "QPushButton:hover { background-color: #505050; }"
                              "QPushButton:disabled { background-color: #2d2d2d; color: #666; }");
    connect(skipButton, &QPushButton::clicked, this, &KNN::onSkip);
    topBarLayout->addWidget(skipButton);

    restartButton = new QPushButton("Restart");
    restartButton->setFixedSize(90, 30);
    restartButton->setStyleSheet("QPushButton { background-color: #404040; color: white; border: 1px solid #5d5d5d; "
                                 "border-radius: 3px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #505050; }");
    restartButton->setVisible(false);
    connect(restartButton, &QPushButton::clicked, this, &KNN::onRestart);
    topBarLayout->addWidget(restartButton);

    visualizationLayout->addLayout(topBarLayout);

    // main content
    auto* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(30);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setAlignment(Qt::AlignVCenter);

    // left: neighbors grid
    auto* leftContainer = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);
    leftLayout->setAlignment(Qt::AlignCenter);

    kValueLabel
        = new QLabel(QString("Nearest %1 neighbor%2:").arg(k == 1 ? "" : QString::number(k)).arg(k == 1 ? "" : "s"));
    kValueLabel->setStyleSheet("color: #00aaff; font-size: 14px; font-weight: bold;");
    kValueLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(kValueLabel);

    auto* neighborsWidget = new QWidget();
    neighborsWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    neighborsGrid = new QGridLayout(neighborsWidget);
    neighborsGrid->setSpacing(10);
    neighborsGrid->setContentsMargins(0, 0, 0, 0);
    neighborsGrid->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < kValues.back(); i++) {
        auto* neighborContainer = new QWidget();
        neighborContainer->setFixedSize(80, 100);
        auto* neighborLayout = new QVBoxLayout(neighborContainer);
        neighborLayout->setContentsMargins(0, 0, 0, 0);
        neighborLayout->setSpacing(3);
        neighborLayout->setAlignment(Qt::AlignCenter);

        auto* label = new QLabel("-");
        label->setStyleSheet("color: white; font-size: 11px; font-weight: bold;");
        label->setAlignment(Qt::AlignCenter);
        label->setFixedHeight(16);
        neighborLayout->addWidget(label);

        auto* imageLabel = new QLabel();
        imageLabel->setFixedSize(80, 80);
        imageLabel->setStyleSheet("border: 1px solid #3d3d3d;");
        imageLabel->setScaledContents(false);
        imageLabel->setAlignment(Qt::AlignCenter);
        neighborLayout->addWidget(imageLabel);

        neighborContainer->setVisible(false);
        neighborsGrid->addWidget(neighborContainer, i / 5, i % 5);
    }

    leftLayout->addWidget(neighborsWidget);
    contentLayout->addWidget(leftContainer, 3);

    // right: current image
    auto* rightContainer = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setAlignment(Qt::AlignCenter);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(15);
    auto* currentImageLabel = new QLabel("Current Image:");
    currentImageLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
    currentImageLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(currentImageLabel);
    currentImage = new QLabel();
    currentImage->setFixedSize(200, 200);
    currentImage->setStyleSheet("border: 2px solid #00aaff;");
    currentImage->setScaledContents(false);
    currentImage->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(currentImage, 0, Qt::AlignCenter);
    contentLayout->addWidget(rightContainer, 2);

    visualizationLayout->addLayout(contentLayout, 1);

    mainLayout->addWidget(visualizationWidget);
}

// gui shall be modifed by those functions as they're connected from constructor which lives on main thread,
// such connections are queued to avoid race condition
void KNN::onProgress(const QVariantMap& info)
{
    skipButton->setDisabled(false);
    const int k = info["k"].toInt();
    kValueLabel->setText(
        QString("Nearest %1 neighbor%2:").arg(k == 1 ? "" : QString::number(k)).arg(k == 1 ? "" : "s"));
    statusLabel->setText(QString("%1 | Acc: %2% | Image %3/%4")
            .arg(info["isValidation"].toBool() ? "Validation" : "Test")
            .arg(info["accuracy"].toDouble(), 0, 'f', 2)
            .arg(info["currentIndex"].toInt())
            .arg(info["totalCount"].toInt()));

    QVariantList currentImgData = info["currentImage"].toList();
    if (currentImgData.size() == 784) {
        QImage img(28, 28, QImage::Format_Grayscale8);
        for (int i = 0; i < 784; i++) {
            uint8_t val = currentImgData[i].toUInt();
            img.setPixel(i % 28, i / 28, qRgb(val, val, val));
        }
        currentImage->setPixmap(QPixmap::fromImage(img).scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation));
    }

    QVariantList neighborsData = info["neighbors"].toList();
    int displayCount = std::min<int>(kValues.back(), neighborsData.size());

    for (int i = 0; i < kValues.back(); i++) {
        QLayoutItem* item = neighborsGrid->itemAtPosition(i / 5, i % 5);
        if (!item)
            continue;
        QWidget* neighborWidget = item->widget();
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(neighborWidget->layout());

        if (i < displayCount) {
            QVariantMap nMap = neighborsData[i].toMap();

            QLabel* labelWidget = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
            labelWidget->setText(QString::number(nMap["label"].toInt()));

            QLabel* imageWidget = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
            QVariantList nImgData = nMap["image"].toList();
            if (nImgData.size() == 784) {
                QImage img(28, 28, QImage::Format_Grayscale8);
                for (int j = 0; j < 784; j++) {
                    uint8_t val = nImgData[j].toUInt();
                    img.setPixel(j % 28, j / 28, qRgb(val, val, val));
                }
                imageWidget->setPixmap(
                    QPixmap::fromImage(img).scaled(80, 80, Qt::KeepAspectRatio, Qt::FastTransformation));
            }
            neighborWidget->setVisible(true);
        } else {
            neighborWidget->setVisible(false);
        }
    }
}

void KNN::onTrainComplete(const QVariantMap& result)
{
    auto updateCell = [&](int row, int col, const QString& text = "") {
        if (auto* item = resultsLayout->itemAtPosition(row, col)) {
            if (auto* label = qobject_cast<QLabel*>(item->widget())) {
                label->setVisible(true);
                if (!text.isEmpty())
                    label->setText(text);
            }
        }
    };

    int col = result["i"].toInt();
    if (col == 0) {
        updateCell(0, col);
        updateCell(1, col);
    }
    updateCell(0, col + 1);
    updateCell(1, col + 1, QString::number(result["accuracy"].toDouble(), 'f', 1) + "%");
}

void KNN::onTestComplete(int bestK, double bestAccuracy, double accuracy)
{
    statusLabel->setText(QString("Test Complete! Using k=%1 (best validation: %2%)\nTest accuracy: %3%")
            .arg(bestK)
            .arg(bestAccuracy, 0, 'f', 2)
            .arg(accuracy, 0, 'f', 2));
    skipButton->setVisible(false);
    pauseButton->setVisible(false);
    restartButton->setVisible(true);
}

void KNN::onSkip()
{
    skipButton->setDisabled(true);
    skip = true;
    if (paused) {
        onPause();
    }
}

void KNN::onPause()
{
    QMutexLocker locker(&pauseMutex);
    paused = !paused;
    emit pausedChanged();
    pauseButton->setText(paused ? "Resume" : "Pause");
    if (!paused) {
        pauseCondition.wakeAll();
    }
}

#pragma endregion GUI Thread
#pragma region Worker Thread

// main loop that runs for all k values
void KNN::run()
{
    int bestK = 0;
    double bestAccuracy = 0;

    for (size_t i = 0; i < kValues.size(); i++) {
        if (isInterruptionRequested())
            return;

        k = kValues[i];
        skip = false;
        double accuracy = getPrecision(true);

        if (accuracy > bestAccuracy) {
            bestK = k;
            bestAccuracy = accuracy;
        }

        emit trainComplete({ { "i", i }, { "accuracy", accuracy } });
    }
    if (isInterruptionRequested())
        return;
    k = bestK;
    skip = false;
    emit testComplete(bestK, bestAccuracy, getPrecision(false));
}

// loop for each k, returns accuracy
double KNN::getPrecision(bool isValidation)
{
    std::vector<Data>& set = isValidation ? validationData : testData;
    int correct = 0;

    for (int i = 0; i < (int)set.size(); i++) {
        if (isInterruptionRequested())
            return 0.0;

        Data& queryPoint = set[i];
        computeDistances(queryPoint);

        int freq[10] = {};
        for (int j = 0; j < k; j++)
            freq[sortedTrainingData[j]->label]++;

        int prediction = std::max_element(freq, freq + 10) - freq;
        correct += (prediction == queryPoint.label);
        {
            QMutexLocker locker(&pauseMutex);
            while (paused && !isInterruptionRequested())
                pauseCondition.wait(&pauseMutex);
        }
        if (skip)
            return (i + 1 == 0) ? 0.0 : (double)correct * 100.0 / (i + 1);

        // build currentImage
        QVariantList currentImg;
        currentImg.reserve(queryPoint.featureVector.size());
        for (uint8_t val : queryPoint.featureVector)
            currentImg.append(val);
        // build neighborsList
        QVariantList neighborsList;
        for (int j = 0; j < k; j++) {
            QVariantList nImg;
            nImg.reserve(sortedTrainingData[j]->featureVector.size());
            for (uint8_t val : sortedTrainingData[j]->featureVector)
                nImg.append(val);
            neighborsList.append(QVariantMap { { "label", sortedTrainingData[j]->label }, { "image", nImg } });
        }
        emit progress(QVariantMap {
            { "k", k },
            { "actual", queryPoint.label },
            { "predicted", prediction },
            { "accuracy", (double)correct * 100.0 / (i + 1) },
            { "currentIndex", i + 1 },
            { "totalCount", (int)set.size() },
            { "isValidation", isValidation },
            { "currentImage", currentImg },
            { "neighbors", neighborsList },
        });
    }

    return (double)correct * 100.0 / set.size();
}

void KNN::computeDistances(const Data& queryPoint)
{
    const std::vector<uint8_t>& queryVector = queryPoint.featureVector;
    const size_t vecSize = queryVector.size();
    const size_t trainingSize = sortedTrainingData.size();

#if defined(__AVX2__)
    // SIMD optimized version, process 32 uint8_t pixels at once (full AVX2 width), max 128x128 images
    for (size_t j = 0; j < trainingSize; j++) {
        std::vector<uint8_t>& trainingVector = sortedTrainingData[j]->featureVector;
        const uint8_t* qptr = queryVector.data();
        const uint8_t* tptr = trainingVector.data();
        __m256i sum_sq = _mm256_setzero_si256(); // sum of squared differences
        const __m256i zero = sum_sq;

        // process 32 pixels at once
        size_t i = 0;
        for (; i + 31 < vecSize; i += 32) {
            // Load 32 uint8_t pixels
            __m256i q_pixels = _mm256_loadu_si256((__m256i*)(qptr + i));
            __m256i t_pixels = _mm256_loadu_si256((__m256i*)(tptr + i));
            // Compute absolute difference with |a-b| (saturating subtract trick)
            __m256i diff = _mm256_or_si256(_mm256_subs_epu8(q_pixels, t_pixels), _mm256_subs_epu8(t_pixels, q_pixels));
            // need to split into low/high 16 bytes
            __m256i diff_lo = _mm256_unpacklo_epi8(diff, zero);
            __m256i diff_hi = _mm256_unpackhi_epi8(diff, zero);
            // square diff * diff (madd computes a*b + c*d for pairs, we use a*a + b*b)
            __m256i sq_lo = _mm256_madd_epi16(diff_lo, diff_lo); // 8x uint32
            __m256i sq_hi = _mm256_madd_epi16(diff_hi, diff_hi); // 8x uint32
            // accumulate
            sum_sq = _mm256_add_epi32(sum_sq, _mm256_add_epi32(sq_lo, sq_hi));
        }

        // add upper and lower 128-bit halves
        __m128i sum128 = _mm_add_epi32(_mm256_castsi256_si128(sum_sq), _mm256_extracti128_si256(sum_sq, 1));
        // add 64-bit pairs
        sum128 = _mm_add_epi32(sum128, _mm_srli_si128(sum128, 8));
        // add 32-bit pairs
        sum128 = _mm_add_epi32(sum128, _mm_srli_si128(sum128, 4));
        // horizontal sum of 8x32-bit integers + scalar tail
        double dist = (double)_mm_cvtsi128_si32(sum128);

        // handle remaining elements (0-31)
        for (; i < vecSize; i++) {
            int diff = (int)qptr[i] - (int)tptr[i];
            dist += diff * diff;
        }
        sortedTrainingData[j]->distance = dist;
    }
#else
    // scalar fallback version
    for (size_t j = 0; j < trainingSize; j++) {
        const std::vector<uint8_t>& trainingVector = sortedTrainingData[j]->featureVector;
        double dist = 0;
        for (size_t i = 0; i < vecSize; i++) {
            // pixel is uint8_t for memory optim but distance requires signed type else overflow so int
            int diff = (int)queryVector[i] - (int)trainingVector[i];
            dist += diff * diff;
        }
        sortedTrainingData[j]->distance = dist;
    }
#endif

    // sort ONCE by distance, this sorted order is reused for all k values
    std::sort(sortedTrainingData.begin(), sortedTrainingData.end(),
        [](const Data* a, const Data* b) { return a->distance < b->distance; });
}