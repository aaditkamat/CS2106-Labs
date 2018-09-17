/*************************************
* Lab 2 Exercise 1
* Name: Aadit Rahul Kamat   
* Student No: A0164761B 
* Lab Group: 02
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

//int num = 1;

int main()
{
    int nChild, result, status, pid;

    //Read the number of child
    scanf("%d", &nChild);
    int children[nChild];
    //Spawn child processes
    for (int i = 0; i < nChild; i++) {
        result = fork();
        if (result == 0) {
            printf("Child %i[%i]: Hello!\n", i + 1, getpid());
            exit(i + 1);
        } else {
            children[i] = result;
        }
    }
    for (int i = 0; i < nChild; i++) {
        pid = waitpid(children[i], NULL, 0);
        printf("Parent: Child %i[%i] done.\n", i + 1, pid);
    }
    printf("Parent: Exiting\n");

    return 0;
}
