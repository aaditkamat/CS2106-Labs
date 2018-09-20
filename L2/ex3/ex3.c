/*************************************
* Lab 2 Exercise 3
* Name:
* Student No:
* Lab Group:
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

struct stat var;

typedef struct {
    char* path;
    char* argv[5];
} command_type;

char* readInput() {
   //hint: you probably want to find a better way to read the inputs
   //hint 2: the alternative can be found in one of the sample programs
   char userInput[120];
   char *result = malloc(120);
   printf("YWIMC > ");
   fgets(userInput, 120, stdin);
   strcpy(result, userInput);
   return result;
}

command_type* getCommand(char* result) {
   command_type* command = malloc(sizeof command);
   char* token = strtok(result, " ");
   command -> path = strtok(token, "\n");
   return command;
}

command_type* getArgs(command_type* command, char* userInput) {
    printf("User Input is: %s\n", userInput);
    char* token = strtok(userInput, " ");
    int argCtr = 0;
    while (argCtr < 5) {
        if (token == NULL)
            break;
        printf("Token %i: %s", argCtr + 1, token);
        command -> argv[argCtr] = malloc(120);
        strcpy(command -> argv[argCtr++], token);
        token = strtok(NULL, " ");
    }
    for (int i = argCtr; i < 5; i++) {
        command -> argv[i] = NULL;
    }
    return command;
}

command_type* parseInput() {
    command_type* command = malloc(sizeof command);
    char* result;
    char store[120];
    result = readInput();
    strcpy(store, result);
    command = getCommand(result);
    command = getArgs(command, store);
    return command;
}

int main()
{ 
   command_type* command;
   int progStatus, result;

   command = parseInput();
   while (strcmp(command -> path, "quit") != 0 ){
        progStatus = stat(command -> path, &var);
        if (progStatus == 0) {
            result = fork();
            if (result != 0) {
               printf("Child %d in background \n", result);
               waitpid(result, NULL, 0);
            } else {
                printf("Command path: %s", command -> path);
                char* const argv[4] = {command -> argv[0], command -> argv[1], command -> argv[2], command -> argv[3]};
                execv(command -> path, argv);
                exit(0);
            }
        } 
        else {
            printf("%s not found\n", command -> path);
        }
        command = parseInput();
    }
    printf("Goodbye!\n");
    return 0;

}