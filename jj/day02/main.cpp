#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream input("input");
    for (std::string line; std::getline(input, line);)
        std::cout << line << '\n';

    return 0;
}
