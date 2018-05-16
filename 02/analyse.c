#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int count = 0;
    int characters[3];
    characters[0] = 0;
    characters[1] = 0;
    characters[2] = 0;

    while (1) {
        characters[getc(stdin)-65]++;
        count++;
        if (count >= 100) {
            fprintf(stdout, "Anzahl von: A: %i, B: %i, C: %i\n", characters);
            fflush(stdout);
            characters[0] = 0;
            characters[1] = 0;
            characters[2] = 0;
            count = 0;
        }
    }

    return 0;
}
