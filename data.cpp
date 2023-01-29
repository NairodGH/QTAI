#include "Data.hpp"

void Data::set_distance(double dist)
{
    m_distance = dist;
}

void Data::set_feature_vector(std::vector<uint8_t> *vect)
{
    m_feature_vector = vect;
}

void Data::append_to_feature_vector(uint8_t val)
{
    m_feature_vector->push_back(val);
}

void Data::set_label(uint8_t val)
{
    m_label = val;
}

void Data::set_enumerated_label(uint8_t val)
{
    m_enumerated_label = val;
}

void Data::print_vector()
{
    printf("[ ");
    for(uint8_t val : *m_feature_vector)
        printf("%u ", val);
    printf("]\n");
}

double Data::get_distance()
{
    return m_distance;
}

int Data::get_feature_vector_size()
{
    return m_feature_vector->size();
}

uint8_t Data::get_label()
{
    return m_label;
}

uint8_t Data::get_enumerated_label()
{
    return m_enumerated_label;
}

std::vector<uint8_t> *Data::get_feature_vector()
{
    return m_feature_vector;
}
