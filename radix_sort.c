#include <stdio.h>
#include <stdlib.h>

int ft_strlen(char *str)
{
  int i;

  i = 0;
  while(str[i])
    i++;

  return (i);
}

int	ft_iterative_power(int nb, int power)
{
	int	i;

	i = nb;
	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	while (power > 1)
	{
		nb *= i;
		power--;
	}
	return (nb);
}
int compare_digits_at_position(int a, int b, int position) {

  if (position < 0) {
    printf("Error: Position must be non-negative.\n");
    return 0; // Or some other error indicator
  }

  int digit_a = (a /ft_iterative_power(10, position)) % 10;
  int digit_b = (b /ft_iterative_power(10, position)) % 10;


  printf("first: %d\ttwo: %d\n", digit_a, digit_b);

  if (digit_a > digit_b) {
    return 1; // a's digit is greater
  } else if (digit_a < digit_b) {
    return -1; // b's digit is greater
  } else {
    return 0; // digits are equal
  }
}

void	pa(int *a, int *b, int *size_a, int *size_b)
{
    if (*size_b == 0)
        return;
    
    // Shift all elements in stack a up by one
    for (int i = *size_a; i > 0; i--)
        a[i] = a[i - 1];
    
    // Move top element from b to a
    a[0] = b[0];
    
    // Shift all elements in stack b down by one
    for (int i = 0; i < *size_b - 1; i++)
        b[i] = b[i + 1];
    
    (*size_a)++;
    (*size_b)--;
}

void	pb(int *a, int *b, int *size_a, int *size_b)
{
    if (*size_a == 0)
        return;
    
    // Shift all elements in stack b up by one
    for (int i = *size_b; i > 0; i--)
        b[i] = b[i - 1];
    
    // Move top element from a to b
    b[0] = a[0];
    
    // Shift all elements in stack a down by one
    for (int i = 0; i < *size_a - 1; i++)
        a[i] = a[i + 1];
    
    (*size_b)++;
    (*size_a)--;
}

void	ra(int *a, int size_a)
{
    if (size_a <= 1)
        return;
    
    int temp = a[0];
    for (int i = 0; i < size_a - 1; i++)
        a[i] = a[i + 1];
    a[size_a - 1] = temp;
}

void	rb(int *b, int size_b)
{
    if (size_b <= 1)
        return;
    
    int temp = b[0];
    for (int i = 0; i < size_b - 1; i++)
        b[i] = b[i + 1];
    b[size_b - 1] = temp;
}

void	rr(int *a, int *b, int size_a, int size_b)
{
    ra(a, size_a);
    rb(b, size_b);
}

void	rra(int *a, int size_a)
{
    if (size_a <= 1)
        return;
    
    int temp = a[size_a - 1];
    for (int i = size_a - 1; i > 0; i--)
        a[i] = a[i - 1];
    a[0] = temp;
}

void	rrb(int *b, int size_b)
{
    if (size_b <= 1)
        return;
    
    int temp = b[size_b - 1];
    for (int i = size_b - 1; i > 0; i--)
        b[i] = b[i - 1];
    b[0] = temp;
}

void	rrr(int *a, int *b, int size_a, int size_b)
{
    rra(a, size_a);
    rrb(b, size_b);
}

void	sa(int *a)
{
    int temp;
    
    if (!a || !a[1])
        return;
    temp = a[0];
    a[0] = a[1];
    a[1] = temp;
}

void	sb(int *b)
{
    int temp;
    
    if (!b || !b[1])
        return;
    temp = b[0];
    b[0] = b[1];
    b[1] = temp;
}

void	ss(int *a, int *b)
{
    sa(a);
    sb(b);
}

// Helper function to find the minimum value in the array
int find_min(int *arr, int size)
{
    int min = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

// Helper function to normalize numbers (make them positive and start from 0)
void normalize_numbers(int *arr, int size)
{
    int min = find_min(arr, size);
    
    // If min is negative, shift all numbers to make them positive
    if (min < 0)
    {
        for (int i = 0; i < size; i++)
            arr[i] -= min;
    }
}

int *radix_sort(int *arr, int size)
{
    if (!arr || size <= 1)
        return arr;

    // Create stack b
    int *stack_b = (int *)malloc(sizeof(int) * size);
    if (!stack_b)
        return NULL;
    int size_a = size;
    int size_b = 0;

    // Normalize numbers to handle negative values
    normalize_numbers(arr, size);

    // Find maximum number to know number of bits
    int max_num = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max_num)
            max_num = arr[i];
    }

    // Calculate number of bits needed
    int max_bits = 0;
    while ((max_num >> max_bits) != 0)
        max_bits++;

    // Do radix sort for every bit
    for (int bit = 0; bit < max_bits; bit++)
    {
        // Process each number in stack a
        for (int i = 0; i < size; i++)
        {
            // If current bit is 0, push to stack b
            if (((arr[0] >> bit) & 1) == 0)
            {
                pb(arr, stack_b, &size_a, &size_b);
                printf("pb\n");
            }
            // If current bit is 1, rotate a
            else
            {
                ra(arr, size_a);
                printf("ra\n");
            }
        }
        
        // Push all numbers back to stack a
        while (size_b > 0)
        {
            pa(arr, stack_b, &size_a, &size_b);
            printf("pa\n");
        }
    }

    free(stack_b);
    return arr;
}

// Create 4 zones 
// sat, sab, sbt, sbb
// based on the max number have to decide the range of numbers for each zone 
// actions can only occur on bottom of bottom zone and top of top zones
// 





int main(void)
{
    // Test array with both positive and negative numbers
    int arr[] = {45, -12, 6, -3, 89, 0, -23, 15};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n\n");

    printf("Sorting operations:\n");
    radix_sort(arr, size);

    printf("\nSorted array: ");
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}





