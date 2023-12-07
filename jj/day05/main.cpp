#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in{"test"};

    for (std::string line; std::getline(in, line);)
	    std::cout << line << '\n';

    // std::cout << "part 1: " << res1 << '\n';
    // std::cout << "part 2: " << res2 << '\n';

    return 0;
}
