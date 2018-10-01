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

#define MAX_ARGUMENT_LENGTH 20
#define MAX_NUMBER_OF_ARGUMENTS 6
#define MAX_USER_INPUT_LENGTH MAX_ARGUMENT_LENGTH * MAX_NUMBER_OF_ARGUMENTS
#define MAX_NUMBER_OF_CHILDPIDS 10

struct stat var;

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum )
{
    char input[120];
    char** tokenStrArr;
    char* tStart; 
    int i;      

    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    printf("YWIMC > ");
    fgets(input, 120, stdin);

    tStart = strtok(input, " \n"); 

    i = 0;
    while (i < maxTokenNum && tStart){
        tokenStrArr[i] = (char*) malloc(sizeof(char) * maxTokenSize );
        strncpy(tokenStrArr[i], tStart, maxTokenSize);    
        tokenStrArr[i][maxTokenSize-1] = '\0';
        i++;
        tStart = strtok(NULL, " \n"); 
    }
    *readTokenNum = i;
    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }
    free(strArr);
}

int checkProcessBackground(char** argsArr, int length) {
    for (int i = 0; i < length; i++) {
        if (argsArr[i] != NULL && strcmp(argsArr[i], "&") == 0) {
            return 1;
        }
    }
    return 0;
}

void executeProcess(char** argsArr, int argsLength, int inputLength) {
    for (int i = 0; i < argsLength; i++) {
        printf("%s\n", argsArr[i]);
        if (strcmp(argsArr[i], "&") == 0) {
            argsArr[i] = NULL;
        }
    } 
    for (int i = 0; i < argsLength; i++) {
        printf("%s\n", argsArr[i]);
    }
    execv(argsArr[0], argsArr);
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
   int progStatus, result, status, bg, ctr = 0, childPids[MAX_NUMBER_OF_CHILDPIDS], nToken;
   char **argsArr;
   for (int i = 0; i < MAX_NUMBER_OF_CHILDPIDS; i++) {
       childPids[i] = -1;
   }
   argsArr = readTokens(MAX_NUMBER_OF_ARGUMENTS, MAX_ARGUMENT_LENGTH, &nToken);
   while (strcmp(argsArr[0], "quit") != 0 ){
        progStatus = stat(argsArr[0], &var);
        bg = checkProcessBackground(argsArr, MAX_NUMBER_OF_ARGUMENTS);
        if (progStatus == 0) {
            result = fork();
            if (bg == 1 && result != 0) {
               printf("Child %i is running in the background\n", result);
               childPids[(ctr++) % 10] = result;
            } else if (bg == 0 && result != 0) {
                waitpid(result, &status, 0);
            } else {
                executeProcess(argsArr, MAX_NUMBER_OF_ARGUMENTS, MAX_USER_INPUT_LENGTH);
            }
        } 
        else if (strcmp(argsArr[0], "wait") == 0) {
            int childPid = atoi(argsArr[1]);
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
        else if (strcmp(argsArr[0], "printchild") == 0) {
            printChildIds(childPids, MAX_NUMBER_OF_CHILDPIDS);
        }
        else if (strcmp(argsArr[0], "result") == 0) {
            printf("%i\n", status >> 8);
        }
        else {
            printf("%s not found\n", argsArr[0]);
        }
        argsArr = readTokens(MAX_NUMBER_OF_ARGUMENTS, MAX_ARGUMENT_LENGTH, &nToken);
    }
    printf("Goodbye!\n");
    return 0;
}