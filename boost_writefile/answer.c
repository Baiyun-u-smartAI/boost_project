#include <stdio.h>
#include "correct.hpp"
double factorial(unsigned int i)
{
   if(i <= 1)
   {
      return 1;
   }
   return i * factorial(i - 1);
}
int correct()
{
    int i = 15;
    printf("the factorial of%d is %lf \n", i, factorial(i));
    return 0;
}
