#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

int parseInt(char* result) {
    int pos = 1, sum = 0;
    for (int i = 0; i < strlen(result) - 1; i++) {
        pos *= 10;
    }
    char each;
    for (int i = 0; i < strlen(result); i++) {
        each = *(result + i);
        printf("Is digit %c: %i\n", each, isdigit(each));
        if (isdigit(each) == 1) {
            sum += (each - 48) * pos;
            pos /= 10;
        }
    }
    return sum;
}

int main() {
    char* number = "102";
    printf("%i\n", parseInt(number));
}