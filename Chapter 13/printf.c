/* printf.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
__attribute__ ((__constructor__)) void preload (void)
{
    printf("This is ld_preload\n");
}
