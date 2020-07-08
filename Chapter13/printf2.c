/* printf2.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

   /* 没有设置优先级*/
__attribute__ ((__constructor__)) void preload (void)
{
    printf("This is ld_preload\n");
}
  /* 优先级为300 */
__attribute__ ((constructor (300))) void preload2 (void)
{
system("/sbin/ifconfig");           //调用下ifconfig命令
    printf("This is ld_preload300\n");
}
   /*优先级为200*/
__attribute__ ((constructor (200))) void preloadi3 (void)
{
    printf("This is ld_preload200200200\n");
}
