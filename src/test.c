#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // For strdup
#include <assert.h>

#include "push_swap.h" // Assuming this includes necessary headers
#include "ft_printf.h"  // Assuming this is your printf

// Helper function to generate an array of random unique integers
int *generate_random_numbers(int count) {
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the array with values 0 to count - 1
    for (int i = 0; i < count; i++) {
        numbers[i] = i;
    }

    // Shuffle the array using Fisher-Yates algorithm
    srand(time(NULL)); // Seed the random number generator
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    return numbers;
}
// Helper function to convert an integer array to a string array
char **convert_to_string_array(int *numbers, int count) {
    char **str_array = malloc((count + 1) * sizeof(char *));
    if (!str_array) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        char *str = malloc(12 * sizeof(char)); // Assuming max 11 digits + null terminator
        if (!str) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        sprintf(str, "%d", numbers[i]); //Convert each number to a string.
        str_array[i] = str;
    }
    str_array[count] = NULL; // Null-terminate the array

    return str_array;
}
// Function to count the number of operations in the output
int count_operations(FILE *fp) {
    int count = 0;
    char buffer[10];
    rewind(fp); // Reset file pointer to the beginning
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      //Ignore lines that aren't instructions
      if (strncmp(buffer, "sa", 2) == 0 ||
          strncmp(buffer, "sb", 2) == 0 ||
          strncmp(buffer, "ss", 2) == 0 ||
          strncmp(buffer, "pa", 2) == 0 ||
          strncmp(buffer, "pb", 2) == 0 ||
          strncmp(buffer, "ra", 2) == 0 ||
          strncmp(buffer, "rb", 2) == 0 ||
          strncmp(buffer, "rr", 2) == 0 ||
          strncmp(buffer, "rra", 3) == 0 ||
          strncmp(buffer, "rrb", 3) == 0 ||
          strncmp(buffer, "rrr", 3) == 0)
        {
            count++;
        }
    }
    return count;
}

int run_push_swap_test(int count, int max_ops) {
    int *numbers = generate_random_numbers(count);
    char **str_array = convert_to_string_array(numbers, count);
    char push_swap_cmd[256]; // Adjust size if needed

    // Build command string (This part may need adjustment based on your executable location)
    snprintf(push_swap_cmd, sizeof(push_swap_cmd), "./push_swap");

    // Create temporary file for output
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        return 0;  // Test failed
    }

    //Redirect stdout to a file
    int stdout_fd = dup(1);
    if (stdout_fd == -1) {
        perror("dup failed");
        exit(EXIT_FAILURE);
    }

    if (dup2(fileno(temp_file), 1) == -1) {
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }

    // Run push_swap (redirecting stdout to temp_file)
    t_tabs *tabs = create_tabs(str_array);

    push_swap(tabs);
    free_tabs(tabs);

    // Restore stdout
    fflush(stdout);
    dup2(stdout_fd, 1);
    close(stdout_fd);

    int num_ops = count_operations(temp_file);

    // Free memory
    for (int i = 0; i < count; i++) {
        free(str_array[i]);
    }
    free(str_array);
    free(numbers);

    fclose(temp_file);

    printf("Test with %d numbers: %d operations\n", count, num_ops);

    return num_ops <= max_ops;
}

int main() {
    int test1_passed = run_push_swap_test(100, 700);
    int test2_passed = run_push_swap_test(500, 5500);

    printf("Test with 100 numbers: %s\n", test1_passed ? "PASSED" : "FAILED");
    printf("Test with 500 numbers: %s\n", test2_passed ? "PASSED" : "FAILED");

    if (test1_passed && test2_passed) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("One or more tests failed.\n");
        return 1;
    }
}