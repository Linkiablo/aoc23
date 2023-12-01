#include <fstream>
#include <iostream>
#include <map>

const std::map<std::string, int> DIGITS{
    {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"1", 1},
    {"2", 2},   {"3", 3},     {"4", 4},     {"5", 5},    {"6", 6},
    {"7", 7},   {"8", 8},     {"9", 9}};

int part_one(std::string &line) {
    int res = 0;
    std::string num;

    for (auto i = line.begin(); i < line.end(); ++i) {
        if (std::isdigit(*i)) {
            num += *i;
        }
    }

    res = (num.at(0) - 48) * 10 + (num.at(num.length() - 1) - 48);

    return res;
}

int part_two(std::string &line) {
    int res[2];

    auto first = line.length();
    auto last = 0;

    for (const auto &[k, v] : DIGITS) {
        auto first_of = line.find(k);
        auto last_of = line.rfind(k);

        if (first_of != std::string::npos && first_of <= first) {
            res[0] = v;
            first = first_of;
        }

        if (last_of != std::string::npos && last_of >= (unsigned)last) {
            res[1] = v;
            last = last_of;
        }
    }

    return res[0] * 10 + res[1];
}

int main() {
    int res_one = 0;
    int res_two = 0;

    std::ifstream input_one("input");

    for (std::string line; std::getline(input_one, line);) {
        res_one += part_one(line);
        res_two += part_two(line);
    }

    std::cout << "part 1: " << res_one << '\n';
    std::cout << "part 2: " << res_two << '\n';

    return 0;
}
