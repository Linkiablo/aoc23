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
private:
  uint32_t source_start;
  uint32_t dest_start;
  uint32_t len;

public:
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
  std::vector<std::vector<range>> lookups;
  void add_range_to_map(uint32_t map_idx, const std::vector<uint32_t> &range);

public:
  almanach(const std::string &in);
  uint32_t find_lowest_location();
};

uint32_t almanach::find_lowest_location() {
  std::vector<uint32_t> locations;

  for (auto &s : this->seeds) {
    auto map_idx = 0;
    auto translation = s;

    for (const auto &m : this->lookups) {
      std::cout << "looking for " << translation << " at " << map_idx << '\n';

      for (const auto &r : m) {
        auto temp = r.translate(translation);
        if (temp.has_value()) {
          translation = temp.value();
	  break;
        }
      }

      map_idx++;
    }

    locations.push_back(translation);
    std::cout << translation << '\n';
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
  almanach a{"input"};

  auto res1 = a.find_lowest_location();
  std::cout << "part 1: " << res1 << '\n';
  // std::cout << "part 2: " << res2 << '\n';

  return 0;
}
