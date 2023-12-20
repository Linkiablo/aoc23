package main

import (
	"log"
	"testing"
)

func TestConc(t *testing.T) {
	net, inst, starting_nodes, err := parseInput("input")
	if err != nil {
		log.Fatal(err)
	}

	part_two := net.LookupRange(inst, starting_nodes)

	if part_two != 22289513667691 {
		t.Fatal("Wrong result for part two!")
	}
}

func TestSeq(t *testing.T) {
	net, inst, starting_nodes, err := parseInput("input")
	if err != nil {
		log.Fatal(err)
	}

	part_two := net.LookupRangeSeq(inst, starting_nodes)

	if part_two != 22289513667691 {
		t.Fatal("Wrong result for part two!")
	}
}

func BenchmarkConc(b *testing.B) {
	net, inst, starting_nodes, err := parseInput("input")
	if err != nil {
		b.Fatal(err)
	}

	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		net.LookupRange(inst, starting_nodes)
	}

}

func BenchmarkSeq(b *testing.B) {
	net, inst, starting_nodes, err := parseInput("input")
	if err != nil {
		b.Fatal(err)
	}

	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		net.LookupRangeSeq(inst, starting_nodes)
	}

}
