package main

import (
	"fmt"
	"os"
	"strings"
)

func Gcd(a, b int) int {
	for b != 0 {
		t := b
		b = a % b
		a = t
	}
	return a
}

// find Least Common Multiple (LCM) via GCD
func Lcm(l []int) int {
	result := l[0] * l[1] / Gcd(l[0], l[1])

	for i := 2; i < len(l); i++ {
		result = Lcm([]int{result, l[i]})
	}

	return result
}

type directions struct {
	l string
	r string
}

type network map[string]directions

func (n network) LookupRange(inst string, nodes []*string) int {
	var cycles []int

	for _, node := range nodes {
		cycle := 0
		inst_step := 0
		for {
			if (*node)[2] == 'Z' {
				cycles = append(cycles, cycle)
				break
			}

			if inst[inst_step] == 'R' {
				temp := n[*node].r
				*node = temp
			} else {
				temp := n[*node].l
				*node = temp
			}

			cycle++
			inst_step = (inst_step + 1) % len(inst)
		}
	}

	return Lcm(cycles)
}

func (n network) CountInstructionSteps(inst string) int {
	res := 0
	inst_step := 0
	recent := "AAA"

	for recent != "ZZZ" {
		if inst[inst_step] == 'R' {
			recent = n[recent].r
		} else {
			recent = n[recent].l
		}

		inst_step = (inst_step + 1) % len(inst)
		res++
	}

	return res
}

func main() {
	in, err := os.ReadFile("input")
	if err != nil {
		panic("read file failed!")
	}

	parts := strings.Split(string(in), "\n\n")

	inst := parts[0]

	net := make(network)
	var starting_nodes []*string
	for _, def := range strings.Split(parts[1], "\n") {
		if len(def) == 0 {
			break
		}

		node := def[0:3]
		if node[2] == 'A' {
			starting_nodes = append(starting_nodes, &node)
		}

		net[def[0:3]] = directions{def[7:10], def[12:15]}
	}

	part_one := net.CountInstructionSteps(inst)
	fmt.Println("part one: ", part_one)

	part_two := net.LookupRange(inst, starting_nodes)
	fmt.Println("part two: ", part_two)
}
