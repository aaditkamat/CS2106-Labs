/*************************************
* Lab 2 Exercise 3
* Name: Aadit Rahul Kamat
* Student No: A0164761B
* Lab Group: 02
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

struct stat var;

void readInput(char* userInput, int length) {
   printf("YWIMC > ");
   fgets(userInput, length, stdin);
}

void getArgs(char* userInput, char* argv[], int length) {
   int strCtr = 0, argCtr = 0;
   char path[length];
   for (int i = 0; i < strlen(userInput); i++) {
       if (*(userInput + i) != ' ' && *(userInput + i) != '\n') {
           path[strCtr++] = *(userInput + i);
       }
       else {
           path[strCtr] = '\0';
           strcpy(argv[argCtr++], path);
           char path[length];
           strCtr = 0;
       }
   }
}

void parseInput(char* userInput, char* argv[], int length) {
    readInput(userInput, length);
    getArgs(userInput, argv, length);
}

int checkProcessBackground(char* argv[], int length) {
    for (int i = 0; i < length; i++) {
        if (strlen(argv[i]) >= 1 && strcmp(argv[i], "&") == 0) {
            return 1;
        }
    }
    return 0;
}

void executeProcess(char* store[], int argsLength, int inputLength) {
    for (int i = 0; i < argsLength; i++) {
        //printf("%i - %s\n", i, store[i]);
    }
    char* temp[argsLength];
    for (int i = 0; i < 5; i++) {
        temp[i] = malloc(inputLength);
        //printf("LENGTH: %lu\n", strlen(store[i]));
        if (strlen(store[i]) >= 1) {
            strcpy(temp[i], store[i]);
            //printf("%i - %s\n", i, temp[i]);
        }
        else {
            temp[i] = NULL;
        }
    }
    char* const argv[] = {temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]};
    execv(argv[0], argv);
}

void createArgs(char* argv[], int length) {
    for (int i = 0; i < 6; i++) {
        argv[i] = malloc(length);
    }
}

int isValidChildPid(int pid, int pids[], int length) {
    for (int i = 0; i < length; i++) {
        if (pid == pids[i]) {
            return 1;
        }
    }
    return 0;
}

void removeChildPid(int pid, int pids[], int length) {
    for (int i = 0; i < length; i++) {
        if (pid == pids[i]) {
            pids[i] = -1;
        }
    }
}

void printChildIds(int childPids[], int length) {
    printf("Unwaited Child Processes:\n");
    for (int i = 0; i < length; i++) {
        if (childPids[i] >= 0) {
            printf("%i\n", childPids[i]);
        }
    }
}

int main()
{ 
   int progStatus, result, status, bg, MAX_USER_INPUT_LENGTH = 120, 
       MAX_ARGUMENTS_LENGTH = 6, MAX_NUMBER_OF_CHILDPIDS = 10, childPids[MAX_NUMBER_OF_CHILDPIDS], 
       ctr = 0;
   char* userInput = malloc(MAX_USER_INPUT_LENGTH), *argv[MAX_ARGUMENTS_LENGTH];
   for (int i = 0; i < MAX_NUMBER_OF_CHILDPIDS; i++) {
       childPids[i] = -1;
   }
   createArgs(argv, MAX_ARGUMENTS_LENGTH);
   parseInput(userInput, argv, MAX_USER_INPUT_LENGTH);
   while (strcmp(argv[0], "quit") != 0 ){
        progStatus = stat(argv[0], &var);
        //printf("%i\n", progStatus);
        bg = checkProcessBackground(argv, MAX_ARGUMENTS_LENGTH);
        if (progStatus == 0) {
            result = fork();
            if (bg == 1 && result != 0) {
               printf("Child %i is running in the background\n", result);
               childPids[ctr++] = result;
            } else if (bg == 0 && result != 0) {
                waitpid(result, NULL, 0);
            } else {
                //printf("%i\n", result);
                executeProcess(argv, MAX_ARGUMENTS_LENGTH, MAX_USER_INPUT_LENGTH);
            }
        } 
        else if (strcmp(argv[0], "wait") == 0) {
            int childPid = atoi(argv[1]);
            printf("%i\n", childPid);
            if (isValidChildPid(childPid, childPids, MAX_NUMBER_OF_CHILDPIDS) == 1) {
                result = fork();
                if (result != 0) {
                    waitpid(childPid, &status, 0);
                    removeChildPid(childPid, childPids, MAX_NUMBER_OF_CHILDPIDS);
                } 
            } else {
                printf("%i not a valid child\n", childPid);
            }
        }
        else if (strcmp(argv[0], "printchild") == 0) {
            printChildIds(childPids, MAX_NUMBER_OF_CHILDPIDS);
        }
        else if (strcmp(argv[0], "result") == 0) {
            printf("%i\n", status >> 8);
        }
        else {
            printf("%s not found\n", argv[0]);
        }
        userInput = malloc(MAX_USER_INPUT_LENGTH);
        createArgs(argv, MAX_ARGUMENTS_LENGTH);
        parseInput(userInput, argv, MAX_USER_INPUT_LENGTH);
    }
    printf("Goodbye!\n");
    return 0;
}