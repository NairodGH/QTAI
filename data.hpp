#ifndef __DATA_H
#define __DATA_H

#include <vector>
#include "stdint.h" // uint8_t 
#include "stdio.h"

class data
{
    std::vector<uint8_t> *m_feature_vector;
    uint8_t m_label;
    uint8_t m_enumerated_label; // A -> 1
    double m_distance;

public:
    void set_distance(double);
    void set_feature_vector(std::vector<uint8_t> *);
    void append_to_feature_vector(uint8_t);
    void set_label(uint8_t);
    void set_enumerated_label(uint8_t);
    void print_vector();

    double get_distance();
    int get_feature_vector_size();
    uint8_t get_label();
    uint8_t get_enumerated_label();

    std::vector<uint8_t> *get_feature_vector();

};

#endif