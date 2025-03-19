#!/bin/bash

# Compile the program
gcc -fopenmp -o sieve test.c -lm

# Array of different n values
n_values=()
for ((i=29; i<=32; i++)); do
    n_values+=($((2**i)))
done

# Array of different compiler flags
compiler_flags=(
    # "-O0" 
    # "-O1" 
    # "-O2" 
    "-O3" 
    "-Ofast"
    )

# Run the program with different n values and compiler flags
for n in "${n_values[@]}"; do
  echo ""
  for flag in "${compiler_flags[@]}"; do
    echo "Running with n=$n and flag=$flag"
    gcc -fopenmp $flag -o sieve test.c -lm
    ./sieve $n
    echo ""
  done
done