#include "Sets.hpp"

void Sets::set_training_data(std::vector<Data *> * vect)
{
    training_data = vect;
}

void Sets::set_test_data(std::vector<Data *> * vect)
{
    test_data = vect;
}

void Sets::set_validation_data(std::vector<Data *> * vect)
{
    validation_data = vect;
}
