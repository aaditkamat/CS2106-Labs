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
            children[i] = getpid();
             printf("Child %i[%i]: Hello!\n", i + 1, getpid());
            printf("Parent: Child %i[%i] done.\n", i + 1, getpid());
            exit(i + 1);
        }
    }

    sleep(2);

    for (int i = 0; i < nChild; i++) {
        pid = waitpid(result, &status, WEXITSTATUS(result));
    }
    
    printf("Parent: Exiting\n");

    return 0;
}
