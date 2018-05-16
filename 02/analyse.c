#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int count = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    char character;

    while (character = getc(stdin)) {
        switch(character) {
            case 'A': 
                a++;
                break;
            case 'B':
                b++;
                break;
            case 'C':
                c++;
                break;
        }
        count++;
        if (count >= 100000) {
            fprintf(stdout, "Anzahl von: A: %i, B: %i, C: %i\n", a,b,c);
            fflush(stdout);
            a = 0;
            a = 0;
            a = 0;
            count = 0;
        }
    }

    return 0;
}
