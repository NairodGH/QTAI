#include "DataHandler.hpp"

data_handler::data_handler()
{
    data_array = new std::vector<data *>;
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}
data_handler::~data_handler()
{
    // FIX ME
}

void data_handler::read_input_data(std::string path)
{
    uint32_t header[4];
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");

    if(f) {
        for (size_t i = 0; i < 4; i++)
            if (fread(bytes, sizeof(bytes), 1, f))
                header[i] = fix_endianness(bytes);
        printf("Done getting file header.\n");
        uint32_t image_size = header[2] * header[3];
        for(size_t i = 0; i < header[1]; i++) {
            data *d = new data();
            d->set_feature_vector(new std::vector<uint8_t>());
            uint8_t element[1];
            for(int j = 0; j < image_size; j++)
            {
                if(fread(element, sizeof(element), 1, f))
                {
                    d->append_to_feature_vector(element[0]);
                }
            }
            data_array->push_back(d);
        }
        feature_vector_size = data_array->at(0)->get_feature_vector()->size();
        printf("Successfully read %zu data entries.\n", data_array->size());
        printf("The Feature Vector Size is: %d\n", feature_vector_size);
    } else
    {
        printf("Invalid Input File Path\n");
        exit(1);
    }
}
void data_handler::read_label_data(std::string path)
{
    uint32_t magic = 0;
    uint32_t num_images = 0;
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");
    if(f)
    {
        int i = 0;
        while(i < 2)
        {
            if(fread(bytes, sizeof(bytes), 1, f))
            {
                switch(i)
                {
                case 0:
                    magic = fix_endianness(bytes);
                    i++;
                    break;
                case 1:
                    num_images = fix_endianness(bytes);
                    i++;
                    break;
                }
            }
        }

        for(unsigned j = 0; j < num_images; j++)
        {
            uint8_t element[1];
            if(fread(element, sizeof(element), 1, f))
            {
                data_array->at(j)->set_label(element[0]);
            }
        }

        printf("Done getting Label header.\n");
    } 
    else
    {
        printf("Invalid Label File Path\n");
        exit(1);
    }
}
void data_handler::split_data()
{
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int valid_size = data_array->size() * VALID_SET_PERCENT;
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(data_array->begin(), data_array->end(), g);

    // Training Data

    int count = 0;
    int index = 0;
    while(count < train_size)
    {
        training_data->push_back(data_array->at(index++));
        count++;
    }

    // Test Data
    count = 0;
    while(count < test_size)
    {
        test_data->push_back(data_array->at(index++));
        count++;
    }

    // Test Data

    count = 0;
    while(count < valid_size)
    {
        validation_data->push_back(data_array->at(index++));
        count++;
    }

    printf("Training Data Size: %lu.\n", training_data->size());
    printf("Test Data Size: %lu.\n", test_data->size());
    printf("Validation Data Size: %lu.\n", validation_data->size());
}

void data_handler::count_classes()
{
    int count = 0;
    for(unsigned i = 0; i < data_array->size(); i++)
    {
        if(class_map.find(data_array->at(i)->get_label()) == class_map.end())
        {
            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enumerated_label(count);
            count++;
        }
    }
    class_counts = count;
    printf("Successfully Extraced %d Unique Classes.\n", class_counts);
}

int data_handler::get_class_counts()
{
    return class_counts;
}

int data_handler::get_data_array_size()
{
    return data_array->size();
}
int data_handler::get_training_data_size()
{
    return training_data->size();
}
int data_handler::get_test_data_size()
{
    return test_data->size();
}
int data_handler::get_validation_size()
{
    return validation_data->size();
}

uint32_t data_handler::fix_endianness(const unsigned char* bytes)
{
    return (uint32_t)((bytes[0] << 24) |
                      (bytes[1] << 16)  |
                      (bytes[2] << 8)   |
                      (bytes[3]));
}

std::vector<data *> * data_handler::get_training_data()
{
    return training_data;
}
std::vector<data *> * data_handler::get_test_data()
{
    return test_data;
}
std::vector<data *> * data_handler::get_validation_data()
{
    return validation_data;
}
