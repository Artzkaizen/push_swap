# Detailed Explanation of the Push_Swap Chunk Sort Implementation

This document explains the provided C code, which implements a recursive "chunk sort" algorithm designed to solve the push_swap problem. The goal of push_swap is to sort a stack of integers (`stack_a`) using a limited set of operations and an auxiliary stack (`stack_b`).

## 1. Core Concepts

- **Push_Swap Problem:** Given a list of unique integers in stack A, sort them in ascending order using only two stacks (A and B) and a specific set of moves (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`). The goal is usually to minimize the number of moves.
- **Normalization:** Before sorting, the original integer values are mapped to their sorted indices (0 to N-1, where N is the total number of elements). This simplifies comparisons and pivot selection, as the algorithm works with a known range [0, N-1].
- **Chunk Sort:** A recursive, divide-and-conquer strategy similar in concept to Quicksort.
  - It treats logical "chunks" (contiguous sequences of elements _based on their sorted position_, not necessarily their physical location) within the stacks.
  - It recursively splits chunks into smaller sub-chunks (typically three: min, mid, max) based on pivot values.
  - These sub-chunks are moved between stacks A and B to specific logical locations (`TOP_A`, `BOTTOM_A`, `TOP_B`, `BOTTOM_B`).
  - Base cases (chunks of size 1, 2, or 3) are sorted directly using optimized move sequences.
  - The recursion unwinds, eventually leaving all elements sorted in stack A.

## 2. Data Structures

Several structures and enums are defined to manage the stacks and the sorting process:

- **`enum e_bool` / `t_bool`:** A simple boolean type (`TRUE`, `FALSE`).
- **`enum e_stack_move` / `t_stack_move`:** Enumerates all possible push_swap operations. Used for clarity and in the `move_and_print` function.
- **`struct s_node` / `t_node`:** Represents an element in the stack.
  - `value`: The **normalized** index (0 to N-1). The sorting logic primarily uses this.
  - `orig`: The original integer value provided as input. Needed for checking initial sorted state and potentially for output/debugging.
  - `next`, `prev`: Pointers to form a **doubly linked circular list**. This allows efficient rotation (`ra`, `rra`) and access to both the top and bottom elements.
- **`struct s_stack` / `t_stack`:** Represents either stack A or stack B.
  - `head`: Pointer to the top node of the stack (or any node in the circle, but consistently used as the top).
  - `size`: The number of elements currently in the stack.
- **`struct s_tabs` / `t_tabs`:** Holds both stack A and stack B.
- **`enum e_loc` / `t_loc`:** Defines the _logical location_ of a chunk. This is crucial for the chunk sort strategy.
  - `TOP_A`: The top part of stack A.
  - `BOTTOM_A`: The bottom part of stack A (accessed via `rra`).
  - `TOP_B`: The top part of stack B.
  - `BOTTOM_B`: The bottom part of stack B (accessed via `rrb`).
- **`struct s_chunk` / `t_chunk`:** Represents a chunk of elements to be sorted.
  - `loc`: The `t_loc` where this chunk logically resides.
  - `size`: The number of elements in this chunk.
- **`struct s_split_dest` / `t_split_dest`:** Holds the three destination chunks (`min`, `mid`, `max`) created after splitting a larger chunk.

## 3. Core Stack Operations & Helpers (User Functions)

These functions provide the basic building blocks for manipulating the stacks and data:

- **Node/Stack Creation & Management:**
  - `create_node`: Allocates and initializes a `t_node`.
  - `append_node`: Adds a node to the end (maintaining circularity) of a stack.
  - `fill_stack`: Parses input strings, validates them, creates nodes, and appends them to the initial stack A. Includes duplicate checking (`has_duplicates`) and validation (`is_valid_int_str`).
  - `free_stack`: Frees all nodes in a stack.
  - `create_tabs`: Creates the main `t_tabs` structure, initializes stacks, and fills stack A from input arguments.
  - `free_tabs`: Frees both stacks and the `t_tabs` structure.
- **Validation & Normalization:**
  - `has_duplicates`: Checks if the initial input contains duplicate numbers.
  - `is_valid_int_str`: Checks if a string represents a valid integer within `INT_MIN`/`INT_MAX`.
  - `normalize_stack`: **Key preprocessing step.** Creates a sorted array of the original values, then iterates through the stack nodes, assigning `node->value` based on the rank (0 to N-1) of `node->orig` in the sorted array.
  - `is_sorted`: Checks if a stack is sorted based on the **normalized `value`**.
  - `is_raw_input_sorted`: Checks if the stack is sorted based on the **original `orig` value**. Used to exit early if the input is already sorted.
- **Push_Swap Moves:**
  - `sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`: Implement the standard push_swap operations by manipulating the `head` pointers and node links (`next`/`prev`) of the circular lists. Note that swaps (`sa`, `sb`) only swap the `value` and `orig` fields of the top two nodes for efficiency, not the nodes themselves.
  - `move_stack`: A helper function to call the correct move implementation based on the `t_stack_move` enum.
  - `move_and_print`: Calls `move_stack` to perform the operation and prints the corresponding move name ("sa", "pb", etc.) to standard output. It also increments the global `g_move_count`.

## 4. Chunk Sort Algorithm Implementation

This is the core sorting logic, implemented recursively.

- **`chunk_sort(t_tabs *data)`:**

  - The main entry point for the chunk sort.
  - Creates an initial `t_chunk` representing all of stack A (`loc = TOP_A`, `size = data->a.size`).
  - Calls `rec_chunk_sort` to start the recursive process.
  - After recursion completes, performs a final check: if stack A is not perfectly rotated (i.e., the node with `value == 0` is not at the `head`), it performs `ra` or `rra` operations to bring the smallest element (`value == 0`) to the top.

- **`rec_chunk_sort(t_tabs *data, t_chunk *to_sort)`:**

  - The recursive heart of the algorithm. Takes the chunk to be sorted (`to_sort`) as input.
  - **Base Cases:**
    - If `to_sort->size` is 3, 2, or 1, it calls the specialized functions `sort_three`, `sort_two`, or `sort_one`, respectively, and returns. These functions handle small sorts efficiently.
  - **Recursive Step (if size > 3):**
    1.  **Initialize Destinations:** Creates a `t_split_dest dest` structure and initializes the sizes of `dest.min`, `dest.mid`, `dest.max` to 0 using `innit_size`.
    2.  **Determine Pivot Values:** Calculates the minimum (`min_val`) and maximum (`max_val`) normalized values within the current `to_sort` chunk using `chunk_min_value` and `chunk_max_value`. Then, `set_third_pivots_by_value` calculates two pivot values (`pivot1_val`, `pivot2_val`) that divide the _value range_ (`max_val - min_val`) roughly into thirds. Elements `< pivot1_val` go to `min`, elements `>= pivot1_val` and `< pivot2_val` go to `mid`, and elements `>= pivot2_val` go to `max`.
    3.  **Determine Destination Locations:** `set_split_loc` determines the `t_loc` for `dest.min`, `dest.mid`, and `dest.max` based on the location (`to_sort->loc`) of the chunk currently being processed. The logic aims to partition elements efficiently between A and B. For example, if sorting `TOP_A`:
        - `max` elements might stay in `TOP_A` (requiring `ra` moves).
        - `mid` elements might go to `TOP_B` (requiring `pb` moves).
        - `min` elements might go to `BOTTOM_B` (requiring `pb` then `rb` moves).
    4.  **Partition (Split):** `chunk_split` performs the actual partitioning:
        - It iterates `initial_chunk_size` times (where `initial_chunk_size` is the size of `to_sort` _before_ splitting began).
        - In each iteration, it looks at the value of the element currently at the "start" of the `to_sort` chunk (using `chunk_value(data, to_sort, 1)`).
        - It compares this value to `pivot1_val` and `pivot2_val`.
        - It calls `move_from_to` to execute the necessary push_swap operations (`pa`, `pb`, `ra`, `rb`, etc.) to move the element from its current location (`to_sort->loc`) to the appropriate destination location (`dest.min.loc`, `dest.mid.loc`, or `dest.max.loc`).
        - `move_from_to` returns 1 if a move was made, and the corresponding destination chunk's size (`dest.min/mid/max.size`) is incremented.
        - **Optimization:** Inside the loop, _after_ moving an element potentially destined for `dest.max`, it calls `split_max_reduction` to check if a small sequence at the top of A can be sorted immediately.
    5.  **Recurse:** Calls `rec_chunk_sort` recursively on the newly created chunks, typically in the order: `max`, `mid`, `min`. Sorting `max` first often helps place the largest elements correctly in stack A earlier.

- **Base Case Sorting Functions:**

  - `sort_one(data, to_sort)`: If the single element isn't already in `TOP_A`, moves it there using `move_from_to`.
  - `sort_two(data, to_sort)`: Moves the two elements to `TOP_A` using `move_from_to`, then performs `sa` if they are out of order.
  - `sort_three(data, to_sort)`: Moves the three elements to `TOP_A` (if not already there), then uses a series of conditional `sa`, `ra`, `rra` moves based on the relative order of the top three elements' values to sort them efficiently.

- **Chunk Sort Helper Functions:**
  - `loc_to_stack`: Returns a pointer to the `t_stack` (either `data->a` or `data->b`) corresponding to a given `t_loc`.
  - `chunk_value`: Gets the normalized `value` of the nth element within a specified chunk, handling `TOP` vs `BOTTOM` locations.
  - `chunk_min_value`, `chunk_max_value`: Find the minimum/maximum normalized `value` within a chunk.
  - `set_third_pivots_by_value`: Calculates pivots based on the value range within the chunk.
  - `set_split_loc`: Defines the target locations for sub-chunks during a split.
  - `innit_size`: Resets destination chunk sizes to 0 before splitting.
  - `move_from_to`: **Crucial abstraction layer.** Translates the logical move request (e.g., from `BOTTOM_B` to `TOP_A`) into the required sequence of actual push_swap operations (`RRB`, `PA`) by calling `move_and_print`. Handles rotations within the same stack if the source and destination are the same (e.g., `TOP_A` to `TOP_A` implies `RA`).

## 5. Optimizations

The code includes optimizations inspired by the reference implementation (fsoares/push_swap):

- **Base Cases (sort_one/two/three):** Handling small chunks directly avoids recursion overhead and uses optimized move patterns.
- **`split_max_reduction(data, max_chunk_dest)`:** This function is called during `chunk_split` specifically _after_ an element is moved to the `max` destination chunk (which is often `TOP_A`). It checks for specific patterns:
  - If the top 3 elements in A are now the 3 largest overall (`N-1`, `N-2`, `N-3`) and the rest of A below them is sorted (`a_partly_sort(data, 4)`), it calls `sort_three` immediately on them, effectively removing them from the `max_chunk_dest` count.
  - If a single `sa` swap would place the second element correctly relative to the third, and the rest of A below them is sorted (`a_partly_sort(data, 3)`), it performs the `sa` and reduces the `max_chunk_dest` count.
  - If the top element is already correctly placed relative to a sorted sequence below it (`a_partly_sort(data, 2)`), it reduces the `max_chunk_dest` count.
  - **Goal:** To identify and finalize sorted elements at the top of A as early as possible during the splitting process, reducing the number of elements that need further recursive processing or later rotations.
- **`a_partly_sort(data, from_top_count)`:** Checks if stack A is sorted based on normalized `value` starting from the `from_top_count`-th element down to the bottom. Used by `split_max_reduction`.
- **`sort_three_numbers`, `is_consecutive`:** Basic integer helpers, likely used within optimization checks (though `is_consecutive` doesn't seem directly used in the provided `split_max_reduction` logic, it might have been part of an earlier version or intended for other checks).

## 6. Main Function (`main`)

- Checks for correct number of arguments.
- Calls `create_tabs` to initialize stacks A and B from command-line arguments. Handles errors during creation.
- Initializes `g_move_count` to 0.
- Calls `sort_strategy` (which checks for already sorted input, normalizes, and calls `chunk_sort`).
- Prints the total move count (`g_move_count`) to standard error.
- Calls `free_tabs` to release allocated memory.
- Returns 0 on success, 1 on error.

## 7. Overall Flow

1.  **Initialization:** Read arguments, validate, create stack A, create empty stack B.
2.  **Check Pre-sorted:** If input is already sorted, exit.
3.  **Normalization:** Map original values in A to indices 0..N-1.
4.  **Chunk Sort:**
    - Start with the entire stack A as the first chunk (`TOP_A`, size N).
    - Recursively call `rec_chunk_sort`:
      - If chunk size <= 3, use `sort_one/two/three`.
      - If chunk size > 3:
        - Calculate value-based pivots.
        - Determine destination locations (`TOP/BOTTOM` of A/B).
        - Split the chunk: Iterate through elements, move each to its destination (min/mid/max) using `move_from_to`, applying `split_max_reduction` for max elements going to `TOP_A`.
        - Recursively call `rec_chunk_sort` on the `max`, `mid`, and `min` chunks.
5.  **Final Rotation:** Ensure element 0 is at the top of stack A using `ra` or `rra`.
6.  **Cleanup:** Free all allocated memory.
7.  **Output:** The sequence of push_swap moves is printed to standard output by `move_and_print`. The total count is printed to standard error.

This chunk sort implementation uses normalization and logical locations (`t_loc`) combined with a recursive partitioning strategy to efficiently sort the stack, incorporating optimizations for small chunks and early detection of sorted sequences.
