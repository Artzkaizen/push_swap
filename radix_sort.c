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

int *radix_sort(int *arr, int size)
{

  return NULL;
}
int main(void)
{
 int num1 = 12345;
  int num2 = 67890;

  int position_to_compare = 0; // Compare the digits at the "hundreds" place (position 2)

  int result = compare_digits_at_position(num1, num2, position_to_compare);

  if (result > 0) {
    printf("The digit at position %d in %d is greater than the digit at "
           "position %d in %d\n",
           position_to_compare, num1, position_to_compare, num2);
  } else if (result < 0) {
    printf("The digit at position %d in %d is less than the digit at position "
           "%d in %d\n",
           position_to_compare, num1, position_to_compare, num2);
  } else {
    printf("The digits at position %d in %d and %d are equal\n",
           position_to_compare, num1, num2);
  }
  return 0;
}





