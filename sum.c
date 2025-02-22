#include<stdio.h>
#include<conio.h>
int sum(int a, int b)
{
    return a+b;
}
int main()
{
    int x=9 , y= 5;
    printf("Sum = %d",sum(x,y));
    getch();
    return 0;
}