import math
from dataclasses import dataclass
from functools import reduce

def get_number_of_ways(time: int, distance: int) -> int:
    # res = sum(map(lambda x: 1 if x * (time - x) > distance else 0, range(time)))

    # nullstellenberechnung
    rhs = math.sqrt((time / 2)**2 - distance)

    x1 = math.floor(-rhs + time / 2)
    x2 = math.ceil(rhs + time / 2)

    # anzahl der integer zw. x1 und x2
    res = x2 - x1 - 1

    # print(f"res: {res}")
    return res

@dataclass
class Race:
    times = []
    distances = []
    
    def __init__(self, filename: str):
        with open(filename, 'r') as input:
            lines = input.readlines()
            self.times = [int(part) for part in lines[0].split()[1:]]
            self.distances = [int(part) for part in lines[1].split()[1:]]


    def part_one(self) -> int:
        res = []
        for time, distance in zip(self.times, self.distances):
            res.append(get_number_of_ways(time, distance))

        return reduce(lambda a, b: a * b, res)

    def part_two(self) -> int:
        large_time = 0
        large_distance = 0
        for time, distance in zip(self.times, self.distances):
            large_time *= 10**(math.floor(math.log10(time)) + 1)
            large_distance *= 10**(math.floor(math.log10(distance)) + 1)
            large_time += time
            large_distance += distance

        return get_number_of_ways(large_time, large_distance)

if __name__ == "__main__":
    race = Race("input")
    print(f"part one: {race.part_one()}")
    print(f"part two: {race.part_two()}")
