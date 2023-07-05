#include <stdio.h>

int main()
{
    int x;
    int y;
    
    printf("Escreva um número:\n");
    scanf("%d", &x);

    printf("Escreva outro número:\n");
    scanf("%d", &y);

    int result = x * y;
    printf("Resultado de %d * %d = %d\n", x, y, result);
}