#include "etl.hpp"

void ETL::run()
{
    uint32_t imageWidth = 0, imageHeight = 0, classCount = 0;

    // training images
    QFile trainImagesFile(":/data/mnist/train-images.idx3-ubyte");
    if (trainImagesFile.open(QIODevice::ReadOnly)) {
        unsigned char bytes[4];
        if (trainImagesFile.read((char*)bytes, sizeof(bytes)) == sizeof(bytes)) {
            uint32_t header = fixEndianness(bytes);
            if (header == 2051) { // images magic number header
                readImages(trainImagesFile, trainingData, imageHeight, imageWidth);
            }
        }
        trainImagesFile.close();
    }

    // training label
    QFile trainLabelsFile(":/data/mnist/train-labels.idx1-ubyte");
    if (trainLabelsFile.open(QIODevice::ReadOnly)) {
        unsigned char bytes[4];
        if (trainLabelsFile.read((char*)bytes, sizeof(bytes)) == sizeof(bytes)) {
            uint32_t header = fixEndianness(bytes);
            if (header == 2049) { // labels magic number header
                readLabels(trainLabelsFile, trainingData);
            }
        }
        trainLabelsFile.close();
    }

    // validation from training data, stop if training empty or image too big (for SIMD distances)
    if (trainingData.empty() || (uint64_t)imageHeight * imageWidth > 66051)
        return;
    size_t valSize = trainingData.size() * 0.2;
    validationData.reserve(valSize);
    validationData.assign(trainingData.end() - valSize, trainingData.end());
    trainingData.resize(trainingData.size() - valSize);

    // test images
    QFile testImagesFile(":/data/mnist/t10k-images.idx3-ubyte");
    if (testImagesFile.open(QIODevice::ReadOnly)) {
        unsigned char bytes[4];
        if (testImagesFile.read((char*)bytes, sizeof(bytes)) == sizeof(bytes)) {
            uint32_t header = fixEndianness(bytes);
            if (header == 2051) {
                uint32_t dummyH, dummyW;
                readImages(testImagesFile, testData, dummyH, dummyW);
            }
        }
        testImagesFile.close();
    }

    // test labels
    QFile testLabelsFile(":/data/mnist/t10k-labels.idx1-ubyte");
    if (testLabelsFile.open(QIODevice::ReadOnly)) {
        unsigned char bytes[4];
        if (testLabelsFile.read((char*)bytes, sizeof(bytes)) == sizeof(bytes)) {
            uint32_t header = fixEndianness(bytes);
            if (header == 2049) {
                classCount = readLabels(testLabelsFile, testData);
            }
        }
        testLabelsFile.close();
    }

    emit loadingComplete(
        { trainingData.size(), validationData.size(), testData.size(), imageWidth, imageHeight, classCount });
}

void ETL::readImages(QFile& f, std::vector<Data>& target, uint32_t& rows, uint32_t& cols)
{
    uint32_t header[3]; // count, rows, cols
    unsigned char bytes[4];

    for (size_t i = 0; i < 3; i++) {
        if (f.read((char*)bytes, sizeof(bytes)) == sizeof(bytes))
            header[i] = fixEndianness(bytes);
    }

    rows = header[1];
    cols = header[2];
    uint32_t imageSize = rows * cols;
    std::vector<uint8_t> buffer(imageSize);

    for (size_t i = 0; i < header[0]; i++) {
        Data d;
        if (f.read((char*)buffer.data(), imageSize) == (qint64)imageSize) {
            for (uint32_t j = 0; j < imageSize; j++) {
                d.featureVector.push_back(buffer[j]);
            }
        }
        target.push_back(d);
    }
}

int ETL::readLabels(QFile& f, std::vector<Data>& target)
{
    unsigned char bytes[4];
    if (f.read((char*)bytes, sizeof(bytes)) != sizeof(bytes))
        return 0;
    std::unordered_set<uint8_t> labels;

    uint32_t numImages = fixEndianness(bytes);
    for (size_t j = 0; j < numImages; j++) {
        uint8_t label[1];
        if (f.read((char*)label, 1) == 1) {
            if (j < target.size()) {
                labels.insert(target[j].label = label[0]);
            }
        }
    }
    return labels.size();
}

// regardless of the host system's endianness, convert big endian to uint32_t
uint32_t ETL::fixEndianness(const unsigned char* bytes)
{
    return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}