#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

const std::map<std::string_view, int> DIGITS{
    {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"1", 1},
    {"2", 2},   {"3", 3},     {"4", 4},     {"5", 5},    {"6", 6},
    {"7", 7},   {"8", 8},     {"9", 9}};

int32_t part_one(std::string &line) {
    std::pair<int32_t, int32_t> res;

    auto first = line.find_first_of("123456789");
    auto last = line.find_last_of("123456789");

    res.first = line.at(first) - 48;
    res.second = line.at(last) - 48;

    return res.first * 10 + res.second;
}

int32_t part_two(std::string &line) {
    std::pair<int32_t, int32_t> res;

    auto first = line.length();
    auto last = 0;

    for (const auto &[k, v] : DIGITS) {
        auto first_of = line.find(k);
        auto last_of = line.rfind(k);

        if (first_of != std::string::npos && first_of <= first) {
            res.first = v;
            first = first_of;
        }

        if (last_of != std::string::npos && last_of >= (unsigned)last) {
            res.second = v;
            last = last_of;
        }
    }

    return res.first * 10 + res.second;
}

int main() {
    auto res_one = 0;
    auto res_two = 0;

    std::ifstream input_one("input");

    for (std::string line; std::getline(input_one, line);) {
	    res_one += part_one(line);
	    res_two += part_two(line);
    }

    std::cout << "part 1: " << res_one << '\n';
    std::cout << "part 2: " << res_two << '\n';

    return 0;
}
