#pragma once

#include "Data.hpp"

class ETL
{
    private:
        std::vector<Data *> *data_array; // all of the Data
        std::vector<Data *> *training_data;
        std::vector<Data *> *test_data;
        std::vector<Data *> *validation_data;
        int class_counts;
        int feature_vector_size;
        std::map<uint8_t, int> class_map;

    public:
        const double TRAIN_SET_PERCENT = 0.75;
        const double TEST_SET_PERCENT = 0.20;
        const double VALID_SET_PERCENT = 0.05;

        ETL();
        ~ETL();

        void read_input_data(std::string path);
        void read_label_data(std::string path);
        void split_data();
        void count_classes();

        int get_class_counts();
        int get_data_array_size();
        int get_training_data_size();
        int get_test_data_size();
        int get_validation_size();

        uint32_t fix_endianness(const unsigned char* bytes);

        std::vector<Data *> * get_training_data();
        std::vector<Data *> * get_test_data();
        std::vector<Data *> * get_validation_data();
};
