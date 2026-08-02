#!/bin/sh

set -xe

gcc -Wall -Wextra -ggdb -o single_neuron single_neuron.c -lm
