#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

class card {
  private:
    std::vector<uint32_t> winning;
    std::vector<uint32_t> numbers;

  public:
    card(const std::string &s);
    uint32_t get_score() const;
    uint32_t get_copies() const;
};

card::card(const std::string &s) {
    auto sv = s.substr(s.find(':') + 1);

    auto split_idx = sv.find('|');

    auto winning_string = sv.substr(0, split_idx);
    auto numbers_string = sv.substr(split_idx + 1);

    for (const auto part : std::views::split(winning_string, ' ')) {
        auto num = std::string_view{part};

        if (num.length() > 0 && num.at(0) != ' ')
            this->winning.push_back(
                std::stoi(std::string{std::string_view{part}}));
    }

    for (const auto part : std::views::split(numbers_string, ' ')) {
        auto num = std::string_view{part};

        if (num.length() > 0 && num.at(0) != ' ')
            this->numbers.push_back(
                std::stoi(std::string{std::string_view{part}}));
    }
}

uint32_t card::get_score() const {
    auto res = 0;

    for (const auto n : this->numbers) {
        for (const auto &o : this->winning) {
            if (o == n) {
                res == 0 ? res = 1 : res *= 2;
            }
        }
    }

    return res;
}

uint32_t card::get_copies() const {
    auto res = 0;

    for (const auto n : this->numbers) {
        for (const auto &o : this->winning) {
            if (o == n) {
                ++res;
            }
        }
    }

    return res;
}

class cardstack {
  private:
    std::vector<card> cards;
    uint32_t get_copies_total(uint32_t card_number);

  public:
    cardstack(const std::string &input);
    uint32_t part_one();
    uint32_t part_two();
};

cardstack::cardstack(const std::string &input) {
    std::ifstream in{input};

    for (std::string line; std::getline(in, line);) {
        this->cards.emplace_back(card{line});
    }
}

uint32_t cardstack::get_copies_total(uint32_t card_number) {
    auto copies = this->cards.at(card_number).get_copies();

    auto res = 1;
    if (copies > 0) {
        for (const auto v :
             std::views::iota(card_number + 1, card_number + 1 + copies)) {
            res += get_copies_total(v);
        }
    }
    return res;
}

uint32_t cardstack::part_one() {
    auto res = 0;

    for (const auto &c : this->cards)
        res += c.get_score();

    return res;
}

uint32_t cardstack::part_two() {
    auto res = 0;

    for (uint32_t i = 0; i < this->cards.size(); ++i)
        res += get_copies_total(i);

    return res;
}

int main() {
    cardstack cs{"input"};

    std::cout << "part 1: " << cs.part_one() << '\n';
    std::cout << "part 2: " << cs.part_two() << '\n';

    return 0;
}
