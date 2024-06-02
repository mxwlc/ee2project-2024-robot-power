#include <fstream>
#include <iostream>
#include <map>
#include <string>

typedef enum {
    STOP = 0,
    START = 1,
} test;


int main()
{
    std::string filename("marker_dict");
    int a=1;
    std::cout << (test)a << std::endl;

    std::ifstream input_file(filename);
    if (!input_file)
    {
        std::cerr << "Error : File not opened\n";
        return 1;
    }
    std::string key, value;
    while (input_file >> key >> value)
    {
        std::cout << key << " " << value << "\n";
    }
}
