#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int opt;
    int integerValue = 0; // default value

    // Parse command-line options
    while ((opt = getopt(argc, argv, "i:h")) != -1) {
        switch (opt) {
            case 'i':
                integerValue = std::stoi(optarg);
                break;
            case 'h':
                std::cout << "Usage: " << argv[0] << " -i integer_value" << std::endl;
                return 0;
            default:
                std::cerr << "Invalid option" << std::endl;
                return 1;
        }
    }

    // Perform the rest of your program logic using integerValue

    std::cout << "Integer value: " << integerValue << std::endl;

    return 0;
}

