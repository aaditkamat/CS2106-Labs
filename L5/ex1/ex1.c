/*************************************
* Lab 5 Exercise 1
<<<<<<< HEAD
* Name: Aadit Rahul Kamat
* Student No: A0164761B
* Lab Group: 02
=======
* Name:
* Student No:
* Lab Group:
>>>>>>> b811378... Add source files and pdf for Lab 5
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fdIn,  fileSize;
    int *ip;
    char fileName[40], buffer[4];

    //Checking 32-bit vs 64-bit. Do not remove!
    printf("Integer size is %d bytes. Should be 4 bytes (32-bit).\n",
            sizeof(int));

    if (sizeof(int) != 4){
        printf("64-bit environment detected!\n");
        printf("Please recompile with \"-m32\" flag\n");
        exit(1);
    }

    //Program proper
    printf("File Name: ");
    scanf("%s", fileName);

    //Open file for read only
    fdIn = open(  fileName, O_RDONLY );

    //TODO:Check for valid file
<<<<<<< HEAD
	if (fdIn == -1) {
	   printf("Cannot Open");
	   exit(1);
	}
    //TODO:Calculate the file size
    fileSize = lseek(fdIn, 0, SEEK_END);
    printf("Size = %i bytes\n", fileSize);
	lseek(fdIn, 0, 0);
    //TODO: Change the code below so that it reads until the end of file
    // Right now, this only read the first item.
	for (int size = 0; size < fileSize; size += 4) {
	  read( fdIn, buffer, 4);
    //This is how you can "convert" bytes into a single integer
      ip = (int*) &buffer;
      printf("%d\n", *ip); 
	}
=======

    //TODO:Calculate the file size

    fileSize = 0;   //change this line to reflect actual file size
    printf("Size = %i bytes\n", fileSize);

    //TODO: Change the code below so that it reads until the end of file
    // Right now, this only read the first item.
    read( fdIn, buffer, 4);

    //This is how you can "convert" bytes into a single integer
    ip = (int*) &buffer;
    printf("%d\n", *ip); 
>>>>>>> b811378... Add source files and pdf for Lab 5
    
    close( fdIn );  //good practice

    return 0;
}
