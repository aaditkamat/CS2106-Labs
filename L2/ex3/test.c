#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

typedef struct {
    char* path;
    char *argv[5];
} command_type;

int main() {
    char userInput[120];
    strcpy(userInput, "/bin/ls -l -a -f");
    printf("%s\n", userInput);
    char* token = malloc(120);
    token = strtok(userInput, " ");
    printf("%s\n", token);
    command_type* command = malloc(sizeof command);
    int argCtr = 0;

    while (argCtr <= 5) {
        if (token == NULL)
            break;
        command -> argv[argCtr] = malloc(120);
        printf("Token %i: %s\n", argCtr + 1, token);
        strcpy(command -> argv[argCtr++], token);
        token = strtok(NULL, " ");
    }
    return 0;
}