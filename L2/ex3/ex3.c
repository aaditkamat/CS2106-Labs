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

void readInput(char* userInput) {
   printf("YWIMC > ");
   fgets(userInput, 120, stdin);
}

void getArgs(char* userInput, char* argv[]) {
   int strCtr = 0, argCtr = 0;
   char path[120];
   for (int i = 0; i < strlen(userInput); i++) {
       if (*(userInput + i) != ' ' && *(userInput + i) != '\n') {
           path[strCtr++] = *(userInput + i);
       }
       else {
           path[strCtr] = '\0';
           strcpy(argv[argCtr++], path);
           char path[120];
           strCtr = 0;
       }
   }
}

void parseInput(char* userInput, char* argv[]) {
    readInput(userInput);
    getArgs(userInput, argv);
}

int checkProcessBackground(char* argv[]) {
    for (int i = 0; i < 6; i++) {
        if (strlen(argv[i]) >= 1 && strcmp(argv[i], "&") == 0) {
            return 1;
        }
    }
    return 0;
}

void executeProcess(char* store[6]) {
    for (int i = 0; i < 5; i++) {
        printf("%i - %s\n", i, store[i]);
    }
    char* temp[5];
    for (int i = 0; i < 5; i++) {
        temp[i] = malloc(120);
        printf("LENGTH: %lu\n", strlen(store[i]));
        if (strlen(store[i]) >= 1) {
            strcpy(temp[i], store[i]);
            printf("%i - %s\n", i, temp[i]);
        }
        else {
            temp[i] = NULL;
        }
    }
    char* const argv[5] = {temp[0], temp[1], temp[2], temp[3], temp[4]};
    execv(argv[0], argv);
}

void createArgs(char* argv[]) {
    for (int i = 0; i < 6; i++) {
        argv[i] = malloc(120);
    }
}
int main()
{ 
   int progStatus, result, status;
   char* userInput = malloc(120);
   char* argv[6];
   createArgs(argv);
   parseInput(userInput, argv);
   while (strcmp(argv[0], "quit") != 0 ){
        progStatus = stat(argv[0], &var);
        printf("%i\n", progStatus);
        if (progStatus == 0) {
            result = fork();
            if (result != 0) {
                waitpid(result, NULL, 0);
            } else {
                printf("%i\n", result);
                executeProcess(argv);
            }
        } 
        else {
            printf("%s not found\n", argv[0]);
        }
        userInput = malloc(120);
        createArgs(argv);
        parseInput(userInput, argv);
    }
    printf("Goodbye!\n");
    return 0;
}