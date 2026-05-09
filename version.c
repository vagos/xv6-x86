#include <stdio.h>
int main()
{
  #if defined __STDC_VERSION__
    if(__STDC_VERSION__ == 202000)
        printf("We are using C23!\n");
    else if (__STDC_VERSION__ == 201710L)
        printf("We are using C17!\n");
    else if (__STDC_VERSION__ == 201112L)
        printf("We are using C11!\n");
    else if (__STDC_VERSION__ == 199901L)
        printf("We are using C99!\n");
  #else
    printf("We are using C90!\n");
  #endif

    return 0;
}
