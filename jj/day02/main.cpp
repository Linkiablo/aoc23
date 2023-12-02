#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

const int MAX_RED = 12, MAX_GREEN = 13, MAX_BLUE = 14;

bool part_one(std::string &line) {
    auto rem_line = line.substr(line.find(':') + 2);

    for (const auto set : std::views::split(rem_line, "; ")) {
        u_int32_t cred = 0, cgreen = 0, cblue = 0;
        for (const auto item : std::views::split(set, ", ")) {
            auto item_string = std::string(std::string_view(item));

            auto count =
                std::stoi(item_string.substr(0, item_string.find(' ')));

            if (item_string.find("red") != std::string::npos)
                cred += count;
            if (item_string.find("green") != std::string::npos)
                cgreen += count;
            if (item_string.find("blue") != std::string::npos)
                cblue += count;
        }

        if (cred > MAX_RED || cgreen > MAX_GREEN || cblue > MAX_BLUE)
            return false;
    }

    return true;
}

u_int32_t part_two(std::string &line) {
    auto rem_line = line.substr(line.find(':') + 2);

    u_int32_t cred = 0, cgreen = 0, cblue = 0;

    for (const auto set : std::views::split(rem_line, "; ")) {
        for (const auto item : std::views::split(set, ", ")) {
            auto item_string = std::string(std::string_view(item));

            auto count = (unsigned)std::stoi(
                item_string.substr(0, item_string.find(' ')));

            if (item_string.find("red") != std::string::npos && count >= cred)
                cred = count;
            if (item_string.find("green") != std::string::npos &&
                count >= cgreen)
                cgreen = count;
            if (item_string.find("blue") != std::string::npos && count >= cblue)
                cblue = count;
        }
    }

    return cred * cgreen * cblue;
}

int main() {
    std::ifstream input("input");

    u_int32_t game = 1;
    u_int32_t res = 0, res_two = 0;

    for (std::string line; std::getline(input, line);) {
        if (part_one(line))
            res += game;

        res_two += part_two(line);

        game++;
    }

    std::cout << "part 1: " << res << '\n';
    std::cout << "part 2: " << res_two << '\n';

    return 0;
}
