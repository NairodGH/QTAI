#include "Data.hpp"

Data::Data()
{
    label = 10;
}

void Data::setDistance(double dist)
{
    distance = dist;
}

void Data::setFeatureVector(std::vector<uint8_t> *vect)
{
    featureVector = vect;
}

void Data::appendToFeatureVector(uint8_t val)
{
    featureVector->push_back(val);
}

void Data::setLabel(uint8_t val)
{
    label = val;
}

void Data::setEnumeratedLabel(uint8_t val)
{
    enumeratedLabel = val;
}

double Data::getDistance()
{
    return distance;
}

uint8_t Data::getLabel()
{
    return label;
}

uint8_t Data::getEnumeratedLabel()
{
    return enumeratedLabel;
}

std::vector<uint8_t> *Data::getFeatureVector()
{
    return featureVector;
}
