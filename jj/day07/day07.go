package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

const order = "23456789TJQKA"
const joker_rule_order = "J23456789TQKA"

type Value int

const (
	Fives Value = iota
	Fours
	FullHouse
	Threes
	TwoPair
	OnePair
	HighCard
)

type hand struct {
	cards string
	bid   int
}

func upgrade(v Value) Value {
	switch v {
	case HighCard:
		return OnePair
	case OnePair:
		return Threes
	case TwoPair:
		return FullHouse
	case Threes:
		return Fours
	case FullHouse:
		return Fours
	case Fours:
		return Fives
	case Fives:
		return Fives
	default:
		panic("huch?")
	}
}

func (h *hand) get_value(joker_rule bool) (v Value) {
	m := make(map[byte]byte)

	for _, c := range h.cards {
		_, ok := m[byte(c)]
		if ok {
			m[byte(c)]++
		} else {
			m[byte(c)] = 1
		}
	}

	v = HighCard
	for key, val := range m {
		// nur bearbeiten wenn die joker regel nicht aktiv,
		// oder wenn aktiv joker nicht hier bearbeiten
		if !joker_rule || key != 'J' {
			if val == 5 {
				v = Fives
			}
			if val == 4 {
				v = Fours
			}
			if val == 3 {
				if v == OnePair {
					v = FullHouse
				} else {
					v = Threes
				}
			}
			if val == 2 {
				if v == OnePair {
					v = TwoPair
				} else if v == Threes {
					v = FullHouse
				} else {
					v = OnePair
				}
			}
		}
	}

	if joker_rule {
		for i := 0; i < int(m['J']); i++ {
			v = upgrade(v)
		}
	}

	return v
}

func (h *hand) get_order(i int, joker_rule bool) int {
	if joker_rule {
		return strings.IndexByte(joker_rule_order, h.cards[i])
	} else {
		return strings.IndexByte(order, h.cards[i])
	}
}

func cmp_cards(lhs hand, rhs hand, joker_rule bool) int {
	lv := lhs.get_value(joker_rule)
	rv := rhs.get_value(joker_rule)

	// links hat kleineren wert als rechts
	if lv > rv {
		return 1
	}
	if lv < rv {
		return -1
	}
	if lv == rv {
		for i := 0; i < len(lhs.cards); i++ {
			if lhs.get_order(i, joker_rule) < rhs.get_order(i, joker_rule) {
				return 1
			}
			if lhs.get_order(i, joker_rule) > rhs.get_order(i, joker_rule) {
				return -1
			}
		}
	}

	return 0
}

func main() {
	f, err := os.OpenFile("input", os.O_RDONLY, os.ModePerm)
	if err != nil {
		panic("open file failed!")
	}
	defer f.Close()

	var hands []hand

	s := bufio.NewScanner(f)
	for s.Scan() {
		line := s.Text()
		parts := strings.Split(line, " ")

		bid, _ := strconv.Atoi(parts[1])
		hands = append(hands, hand{cards: parts[0], bid: bid})

	}

	sort.Slice(hands, func(i, j int) bool {
		return cmp_cards(hands[i], hands[j], false) > 0
	})

	part_one := 0
	for i, h := range hands {
		part_one += (i + 1) * h.bid
	}
	fmt.Printf("part one: %d\n", part_one)

	sort.Slice(hands, func(i, j int) bool {
		return cmp_cards(hands[i], hands[j], true) > 0
	})

	part_two := 0
	for i, h := range hands {
		part_two += (i + 1) * h.bid
	}
	fmt.Printf("part two: %d\n", part_two)
}
