/*************************************
* Lab 3 Exercise 1
* Name: Aadit Kamat
* Student No: A0164761B
* Lab Group: 02
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  // For Predefined constants
#include <sys/ipc.h>    // For POSIX IPC
#include <sys/shm.h>    // For POSIX Shared Memory

int main()
{
    int result, arraySize, initValue;
    char* shdMemRegion;
    int shdMemId, shdMemSize;  //Remember to initialize this before shmget()

    //Get input
    printf("Enter Array Size: ");
    scanf("%i",&arraySize);

    printf("Enter Start Value: ");
    scanf("%i",&initValue);

    //Calculate the correct shdMemSize
    shdMemSize = sizeof(int) * (arraySize + 2); 

    //Create a new shared memory region
    shdMemId = shmget( IPC_PRIVATE, shdMemSize, IPC_CREAT | 0666 );
    if (shdMemId < 0){
        printf("Cannot create shared memory region!\n");
        exit(1);
    }

    //Attach a new shared memory region
    shdMemRegion = (char*) shmat(shdMemId, NULL, 0);
    if ( shdMemRegion == (char*)-1){
        printf("Cannot attach shared memory region!\n");
        exit(1);
    }

    //Shared memory regions remained attached after fork()
    // Parent and child can now communicate with each other!
    result = fork();
    if (result){        //Parent
       int* A = (int*) shdMemRegion;
       for (int i = 0; i < arraySize; i++) {
            A[i] = initValue + i;
       }
       A[arraySize] = initValue;
       while(A[arraySize + 1] != initValue) {
           sleep(1);
       }
       A[arraySize + 1] = 0;
       for (int i = arraySize / 2; i < arraySize; i++) {
           A[arraySize + 1] += A[i];
       }
        //Calculation ends. Show results.
        printf("Parent Sum = %d\n", A[arraySize + 1]);
        printf("Child Sum = %d\n", A[arraySize]);
        printf("Total = %d\n", A[arraySize] + A[arraySize + 1]);

        /*Important: Remember to detach the shared memory region*/
        shmdt( shdMemRegion );
    } else {            //Child
        int* A = (int*) shdMemRegion;
        while(A[arraySize] != initValue) {
            sleep(1);
        }
        A[arraySize + 1] = initValue;
        A[arraySize] = 0;
        for (int i = 0; i < arraySize / 2; i++) {
            A[arraySize] += A[i];
        }
        /*Important: Remember to detach the shared memory region*/
        shmdt( shdMemRegion );

        return 0;   //Child Ends here

    }

    /*Important: Remember to remove the shared memory region after use!*/
    shmctl(shdMemId, IPC_RMID, NULL); 
    return 0;
}
