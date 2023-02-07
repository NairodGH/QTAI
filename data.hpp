#pragma once

#include "main.hpp"

class Data
{
    std::vector<uint8_t> *featureVector;
    uint8_t label;
    uint8_t enumeratedLabel; // A -> 1
    double distance;

public:
    void setDistance(double);
    void setFeatureVector(std::vector<uint8_t> *);
    void appendToFeatureVector(uint8_t);
    void setLabel(uint8_t);
    void setEnumeratedLabel(uint8_t);

    double getDistance();
    int getFeatureVectorSize();
    uint8_t getLabel();
    uint8_t getEnumeratedLabel();

    std::vector<uint8_t> *getFeatureVector();

};