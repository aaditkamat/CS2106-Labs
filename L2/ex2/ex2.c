/*************************************
* Lab 2 Exercise 2
* Name: Aadit Rahul Kamat   
* Student No: A0164761B
* Lab Group: 2
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include<stdlib.h>
#include <string.h>     //For string comparison, string copy
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

struct stat var;

int main()
{
    char path[20];
    int progStatus, pid, status, result;

    //read user input
    printf("YWIMC > ");
    scanf("%s", path );

    while (strcmp( path, "quit") != 0){ 
        // Check whether file exist
        // In real interpreter, a lot more checks are needed
        // E.g. check for file type, execution right etc
        progStatus = stat(path, &var);
        if (progStatus == 0) {
            result = fork();
            if (result != 0) {
                waitpid(result, NULL, 0);
            }
            else {
                execl(path, NULL);
                exit(0);
            }
        }
        else {
            printf("%s not found\n", path);
        }
        printf("YWIMC >");
        scanf("%s", path);
    }
    
    printf("Goodbye!\n");
    return 0;
}
