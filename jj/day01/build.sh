#!/bin/bash

set -x

g++ -Wall -Werror -Wall -Wpedantic -fsanitize=address,leak main.cpp -o out
