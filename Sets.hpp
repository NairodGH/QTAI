#pragma once

#include "Data.hpp"

class Sets
{
    protected:
        std::vector<Data *> *training_data;
        std::vector<Data *> *test_data;
        std::vector<Data *> *validation_data;
    public:
        void set_training_data(std::vector<Data *> *vect);
        void set_test_data(std::vector<Data *> *vect);
        void set_validation_data(std::vector<Data *> *vect);
};