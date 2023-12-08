#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

std::vector<uint32_t> parse_list(const std::string_view &s) {
    std::vector<uint32_t> res;

    for (const auto part : std::views::split(s, ' '))
        res.push_back(std::stoi(std::string{std::string_view{part}}));

    return res;
}

class almanach {
  private:
    std::vector<uint32_t> seeds;
    std::vector<std::map<uint32_t, uint32_t>> lookups;
    void add_range_to_map(uint32_t map_idx, const std::vector<uint32_t> &range);

  public:
    almanach(const std::string &in);
};

void almanach::add_range_to_map(uint32_t map_idx,
                                const std::vector<uint32_t> &range) {
    auto source = range.at(0);
    auto dest = range.at(1);
    uint32_t len = range.at(2);

    for (const auto i : std::views::iota(0, (int)len)) {
        this->lookups.at(map_idx).emplace(std::make_pair(source + i, dest + i));
    }
}

almanach::almanach(const std::string &in) {
    std::ifstream input{in};

    std::string line;
    std::getline(input, line);

    auto seed_string = line.substr(line.find(':') + 2);
    auto seeds = parse_list(seed_string);
    this->seeds.insert(this->seeds.end(), seeds.begin(), seeds.end());

    auto map_idx = 0;
    for (; std::getline(input, line);) {
        auto c = line.at(0);

        if (c == '\n')
            continue;
        if (std::isalpha(c)) {
            map_idx++;
            continue;
        }
	if (std::isdigit(c))
		auto range = parse_list(line);
		this->add_range_to_map(map_idx, range);
    }
}

int main() {
    almanach a{"test"};

    // std::cout << "part 1: " << res1 << '\n';
    // std::cout << "part 2: " << res2 << '\n';

    return 0;
}
