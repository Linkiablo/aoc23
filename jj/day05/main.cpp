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
        res.push_back(std::stol(std::string{std::string_view{part}}));

    return res;
}

class range {
  public:
    uint32_t source_start;
    uint32_t dest_start;
    uint32_t len;

    range(uint32_t _source_start, uint32_t _dest_start, uint32_t _len)
        : source_start{_source_start}, dest_start{_dest_start}, len{_len} {};
    std::optional<uint32_t> translate(uint32_t) const;
};

std::optional<uint32_t> range::translate(uint32_t val) const {
    if (val >= this->source_start && val < (this->source_start + this->len))
        return (this->dest_start + val - this->source_start);

    return {};
}

class almanach {
  private:
    std::vector<uint32_t> seeds;
    std::vector<std::pair<uint32_t, uint32_t>> seed_ranges;
    std::vector<std::vector<range>> lookups;
    void add_range_to_map(uint32_t map_idx, const std::vector<uint32_t> &range);

  public:
    almanach(const std::string &in);
    uint32_t find_lowest_location();
    uint32_t find_lowest_ranges();
};

typedef struct {
    uint32_t start;
    uint32_t len;
    uint32_t map_idx;
} todo;

uint32_t almanach::find_lowest_ranges() {
    std::vector<uint32_t> locations;

    std::vector<todo> todos;

    for (const auto &sr : this->seed_ranges) {
        for (const auto &r : this->lookups.at(0)) {
            if (r.source_start >= sr.first &&
                r.source_start < sr.first + sr.second) {
                todos.emplace_back(todo{
                    r.dest_start + sr.first - r.source_start,
                    std::min(r.source_start + r.len - sr.first, sr.second), 1});
            }
        }
    }

    while (!todos.empty()) {
        auto cur = todos.back();
        todos.pop_back();

	std::cout << cur.map_idx << '\n';
        if (cur.map_idx == 6) {
            locations.push_back(cur.start);
        } else {
            for (const auto &r : this->lookups.at(cur.map_idx)) {
                if (r.source_start >= cur.start &&
                    r.source_start < cur.start + cur.len) {
                    todos.emplace_back(todo{
                        r.dest_start + cur.start - r.source_start,
                        std::min(r.source_start + r.len - cur.start, cur.len),
                        cur.map_idx});
                }
            }
        }
    }

    std::cout << locations.size() << '\n';
    auto min = locations.at(0);
    for (const auto l : locations) {
        if (l <= min)
            min = l;
    }

    return min;
}

uint32_t almanach::find_lowest_location() {
    std::vector<uint32_t> locations;

    for (auto &s : this->seeds) {
        auto translation = s;

        for (const auto &m : this->lookups) {
            for (const auto &r : m) {
                auto temp = r.translate(translation);
                if (temp.has_value()) {
                    translation = temp.value();
                    break;
                }
            }
        }

        locations.push_back(translation);
    }

    auto min = locations.at(0);
    for (const auto l : locations) {
        if (l <= min)
            min = l;
    }

    return min;
}

void almanach::add_range_to_map(uint32_t map_idx,
                                const std::vector<uint32_t> &range_vec) {
    auto dest = range_vec.at(0);
    auto source = range_vec.at(1);
    auto len = range_vec.at(2);

    std::cout << "adding " << source << ' ' << dest << ' ' << len << " to map "
              << map_idx << '\n';
    this->lookups.at(map_idx).emplace_back(range{source, dest, len});
}

almanach::almanach(const std::string &in) {
    std::ifstream input{in};

    std::string line;
    std::getline(input, line);

    auto seed_string = line.substr(line.find(':') + 2);
    auto seeds = parse_list(seed_string);
    this->seeds.insert(this->seeds.end(), seeds.begin(), seeds.end());

    for (auto it = seeds.begin(); it != seeds.end(); it += 2) {
        this->seed_ranges.emplace_back(std::make_pair(*it, *(it + 1)));
    }

    // initialize vector with maps
    this->lookups.emplace_back(std::vector<range>{});

    auto map_idx = 0;
    for (; std::getline(input, line);) {
        if (line.size() == 0)
            continue;

        auto c = line.at(0);
        if (std::isalpha(c)) {
            map_idx++;
            this->lookups.emplace_back(std::vector<range>{});
            continue;
        }
        if (std::isdigit(c)) {
            auto range = parse_list(line);
            this->add_range_to_map(map_idx - 1, range);
        }
    }
}

int main() {
    almanach a{"test"};

    auto res1 = a.find_lowest_location();
    auto res2 = a.find_lowest_ranges();
    std::cout << "part 1: " << res1 << '\n';
    std::cout << "part 2: " << res2 << '\n';

    return 0;
}
