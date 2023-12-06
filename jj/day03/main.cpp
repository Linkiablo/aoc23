#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

typedef struct {
    size_t line_idx;
    size_t start_idx;
    size_t len;
    size_t value;
} num;

class schematic {
  private:
    std::vector<std::string> inner_;
    std::vector<num> nums_;
    std::map<std::pair<int32_t, int32_t>, std::pair<bool, int32_t>>
        gear_ratios_;
    bool is_part_number(const num &n);

  public:
    schematic(const std::string &input_file);
    uint32_t sum_nums();
    uint32_t sum_ratios();
};

schematic::schematic(const std::string &input_file) {
    std::ifstream input(input_file);

    for (std::string line; std::getline(input, line);) {
        this->inner_.push_back(line);
    }

    for (auto const [i, line] : std::views::enumerate(this->inner_)) {
        size_t end_idx = 0;

        for (;;) {
            auto start_idx = line.find_first_of("0123456789", end_idx);
            if (start_idx == std::string::npos)
                break;

            end_idx = start_idx;
            while (end_idx < line.length() && std::isdigit(line.at(end_idx)))
                end_idx++;

            auto len = end_idx - start_idx;
            size_t value = std::stoi(line.substr(start_idx, len));

            this->nums_.push_back({.line_idx = (size_t)i,
                                   .start_idx = start_idx,
                                   .len = len,
                                   .value = value});
        }
    }
}

bool schematic::is_part_number(const num &n) {
    for (int i = std::max((signed)n.line_idx - 1, 0);
         i < std::min((signed)n.line_idx + 2, (signed)this->inner_.size());
         ++i) {
        for (auto j = std::max((signed)n.start_idx - 1, 0);
             j < std::min((signed)(n.start_idx + n.len + 1),
                          (signed)this->inner_.at(i).length());
             ++j) {

            auto const el = this->inner_.at(i).at(j);

            if (el == '*') {
                auto coord = std::make_pair(i, j);
                if (this->gear_ratios_.contains(coord)) {
                    this->gear_ratios_.at(coord).first = true;
                    this->gear_ratios_.at(coord).second *= n.value;
                } else {
                    this->gear_ratios_.emplace(
                        std::make_pair(coord, std::make_pair(false, n.value)));
                }
            }

            if (!std::isdigit(el) && el != '.') {
                std::cout << "found symbol: " << el << '\n';

                return true;
            }
        }
    }

    return false;
}

uint32_t schematic::sum_nums() {
    auto res = 0;
    for (auto const n : this->nums_) {
        std::cout << "checking number: " << n.value << '\n';
        if (is_part_number(n))
            res += n.value;
    }
    return res;
}

uint32_t schematic::sum_ratios() {
    auto res = 0;
    for (auto const &[_, v] : this->gear_ratios_) {
        if (v.first)
            res += v.second;
    }
    return res;
}

int main() {
    schematic s{"input"};

    auto res1 = s.sum_nums();
    auto res2 = s.sum_ratios();
    std::cout << "part 1: " << res1 << '\n';
    std::cout << "part 2: " << res2 << '\n';

    return 0;
}
