#include "ClubManager.hpp"
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: make run <input_file>\n";
        return 1;
    }

    std::string input = argv[1];
    ClubManager manager(input);
    manager.run();
    return 0;
}