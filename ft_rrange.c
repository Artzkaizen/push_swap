#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
int *ft_rrange(int start, int end)
{
  int size;

  size = (end - start) < 0 ? ((end - start) *  -1)  + 1: (end - start) + 1;

printf("size = %d\n", size);
 int i = 0;
  int *arr = (int *)malloc(sizeof(int) * size);


  if(!arr)
      return NULL;

  while (i < size) {
    
    if(end > 0)
      arr[i] = end - i;
    else {  
      arr[i] = end + i;
    }
    i++;
  }
  return arr;
}


int main(void)
{
    int *arr = ft_rrange(0, -7);

    int size =  sizeof(arr) / sizeof(arr[0]);
printf("sizeof: %d\n", size);

  for(int i = 0; i < 9; i++)
    printf("%d\n", arr[i]);

  return 0;
}
