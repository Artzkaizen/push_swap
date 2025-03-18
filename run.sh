#!/bin/bash

# Script to generate random numbers for push_swap and run it.

# Usage: ./generate_and_run.sh <number_count> [optional: range]

#  - <number_count>: The number of unique random integers to generate.
#  - [range]: The maximum value of the random numbers (defaults to 1000 if not provided).

# Error Handling for Arguments
if [ $# -lt 1 ]; then
  echo "Usage: $0 <number_count> [range]"
  echo "  <number_count>: The number of unique random integers to generate."
  echo "  [range]: The maximum value of the random numbers (defaults to 1000)."
  exit 1
fi

number_count=$1
range=${2:-1000}  # Default range to 1000 if not specified

# Input validation
if ! [[ "$number_count" =~ ^[0-9]+$ ]]; then
  echo "Error: number_count must be a positive integer."
  exit 1
fi

if ! [[ "$range" =~ ^[0-9]+$ ]]; then
  echo "Error: range must be a positive integer."
  exit 1
fi


# Function to generate unique random numbers
generate_unique_numbers() {
  local count=$1
  local max_value=$2
  local numbers=()
  local -i num i found

  i=0
  while [ $i -lt $count ]; do
    found=0
    num=$(( RANDOM % (max_value + 1) )) # Generate random number within range
    # Check if the number already exists in the array
    for existing_num in "${numbers[@]}"; do
      if [ "$num" -eq "$existing_num" ]; then
        found=1
        break
      fi
    done

    # Add the number to the array if it's unique
    if [ "$found" -eq 0 ]; then
      numbers+=("$num")
      ((i++))
    fi
  done

  # Print the numbers, space-separated
  echo "${numbers[@]}"
}

# Generate the random numbers
random_numbers=$(generate_unique_numbers $number_count $range)

# Run push_swap with the generated numbers
echo "Running push_swap with: $random_numbers"
./push_swap $random_numbers