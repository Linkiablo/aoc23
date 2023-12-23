package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type history struct {
	diffs [][]int
}

func (h *history) addElementLeft() int {
	prev := 0

	for i := len(h.diffs) - 1; i >= 0; i-- {
		entry := h.diffs[i]

		value := entry[0] - prev
		h.diffs[i] = append([]int{value}, entry...)
		prev = value
	}

	return prev
}

func (h *history) addElementRight() int {
	prev := 0

	for i := len(h.diffs) - 1; i >= 0; i-- {
		entry := h.diffs[i]

		value := prev + entry[len(entry)-1]
		h.diffs[i] = append(entry, value)
		prev = value
	}

	return prev
}

func (h *history) calculatePrev() {
	for i := 1; i < len(h.diffs[0]); i++ {
		finished := true
		var entry []int

		for j := 1; j < len(h.diffs[i-1]); j++ {
			entry = append(entry, h.diffs[i-1][j]-h.diffs[i-1][j-1])
		}

		for _, item := range entry {
			if item != 0 {
				finished = false
			}
		}

		h.diffs = append(h.diffs, entry)

		if finished {
			break
		}
	}
}

func parseInput(filename string) []history {
	input, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}

	var histories []history
	for _, line := range strings.Split(string(input), "\n") {
		if line != "" {
			var entry []int
			for _, item := range strings.Split(line, " ") {
				item, err := strconv.Atoi(item)
				if err != nil {
					log.Fatal(err)
				}
				entry = append(entry, item)
			}

			histories = append(histories, history{[][]int{entry}})
		}
	}
	return histories
}

func main() {
	histories := parseInput("input")

	res_one := 0
	res_two := 0
	for _, h := range histories {
		h.calculatePrev()
		res_one += h.addElementRight()
		res_two += h.addElementLeft()
	}
	fmt.Println("part one: ", res_one)
	fmt.Println("part two: ", res_two)
}
