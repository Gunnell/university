#include <stdio.h>
#include <math.h>

int main() {
    int number = 97;
    int bound = sqrt(number);
    int i;
    if (number <= 1) {
        printf("Not prime\n");
        return -1;
    }
    if (number == 2) {
        printf("Prime\n");
        return 0;
    }
    if (number % 2 == 0) {
        printf("Not prime\n");
        return -1;
    }

    for (i = 3; i <= bound; i += 2) {
        if (number % i == 0) {
            printf("Not prime\n");
            return -1;
        }
    }
    printf("Prime\n");
    return 0;
}
