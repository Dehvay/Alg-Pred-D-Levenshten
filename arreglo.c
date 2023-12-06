#include <stdio.h>
#define NUMELEMENTOS 5
#define LongCadena 20

int main()
{
    char grupo[NUMELEMENTOS][LongCadena];
    int i;

    for (i = 0; i < NUMELEMENTOS; i++)
    {
        printf("Nombre: ");
        scanf_s("%s", grupo[i], LongCadena);
    }

    for (i = 0; i < NUMELEMENTOS; i++)
    {
        printf("\t %s", grupo[i]);
    }

    return 0;
}