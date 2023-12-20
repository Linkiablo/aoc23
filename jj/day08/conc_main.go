package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"sync"
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

func parseInput(filename string) (network, string, []string, error) {
	in, err := os.ReadFile(filename)
	if err != nil {
		return nil, "", nil, err
	}

	parts := strings.Split(string(in), "\n\n")

	inst := parts[0]

	net := make(network)
	var starting_nodes []string
	for _, def := range strings.Split(parts[1], "\n") {
		if len(def) == 0 {
			break
		}

		node := def[0:3]
		if node[2] == 'A' {
			starting_nodes = append(starting_nodes, node)
		}

		net[def[0:3]] = directions{def[7:10], def[12:15]}
	}

	return net, inst, starting_nodes, nil
}

func (n network) LookupRangeInternal(inst string, node string) int {
	cycle := 0
	inst_step := 0
	for {
		if node[2] == 'Z' {
			return cycle
		}

		if inst[inst_step] == 'R' {
			node = n[node].r
		} else {
			node = n[node].l
		}

		cycle++
		inst_step = (inst_step + 1) % len(inst)
	}
}

func (n network) LookupRange(inst string, nodes []string) int {
	var cycles []int
	c := make(chan int, len(nodes))

	var wg sync.WaitGroup
	for _, node := range nodes {
		wg.Add(1)
		go func(node string) {
			defer wg.Done()
			c <- n.LookupRangeInternal(inst, node)
		}(node)
	}
	wg.Wait()
	close(c)

	for cycle := range c {
		cycles = append(cycles, cycle)
	}

	return Lcm(cycles)
}

func (n network) LookupRangeSeq(inst string, nodes []string) int {
	var cycles []int

	for _, node := range nodes {
		cycle := 0
		inst_step := 0
		for {
			if node[2] == 'Z' {
				cycles = append(cycles, cycle)
				break
			}

			if inst[inst_step] == 'R' {
				node = n[node].r
			} else {
				node = n[node].l
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
	net, inst, starting_nodes, err := parseInput("input")
	if err != nil {
		log.Fatal(err)
	}

	part_one := net.CountInstructionSteps(inst)
	fmt.Println("part one: ", part_one)

	part_two := net.LookupRange(inst, starting_nodes)
	fmt.Println("part two: ", part_two)
}
