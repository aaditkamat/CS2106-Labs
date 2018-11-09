/*************************************
* Lab 5 Exercise 2
* Name: Aadit Rahul Kamat
* Student No: A0164761B
* Lab Group: 02
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/USFAT.h"

void print_as_text(uint8_t *data, int size)
{
    for (int i=0; i < size; i++)
    {
        printf("%c", data[i]);

		//Use a standard 80 char width
        if ( i+1 % 80 == 0) {
        	printf("\n");
        }
    }
}

int compare(uint8_t* str1, char* str2, int size) {
  printf("%s %s\n", str1, str2);
  int j;
  for (j = 0; j < size; j++) {
    if (str1[j] != ' ') {
      break;
    }
  }

  for (int i = 0; i < strlen(str2); i++) {
    printf("%c %c %d\n", str1[j], str2[i], str1[j] == str2[i]);
    if (str1[j] != str2[i]) {
      return -1;
    }
    j++;
  }
  return 0;
}

//TODO: Add helper functions if needed
int check_file_name(uint8_t *name, char filename[]) {
  char **nameParts = malloc(11);
  const char* delim = ".";
  int i = 0;
  char *token = strtok(filename, delim);
  nameParts[0] = token;
  while (token != NULL) {
    token = strtok(NULL, delim);
    nameParts[++i] = token;
  }
  strcat(nameParts[0], nameParts[1]);
  printf("%d %d\n", name, compare(name, nameParts[0], 11));
  return 0; 
}

int read_file( FAT_RUNTIME* rt, char filename[])
{
    //TODO: Your code here
	for (int i = 0; i < S_SECTOR_SIZE; i++) {
          char fileNameCopy[13];
          strcpy(fileNameCopy, filename);
	  int result = check_file_name(rt -> dir_buffer[i].name, fileNameCopy);
          printf("Result at read_file: %i %s\n", result, filename);
	  if (result == 0) {
	    int fdIn = open(filename, O_RDONLY), size = rt -> dir_buffer[i].file_size;
	    char* buffer;
	    read(fdIn, buffer, size);
	    printf("%s\n", buffer);
	    return 1;
	  }
	} 
	return 0;	
}

int main(int argc, char** argv)
{
	FAT_RUNTIME fat_rt;
	char filename[13];

	if (argc < 2 ){
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

    //Load USFAT-based file system from a "disk media"
    if (init_runtime_from_media(&fat_rt, argv[1]) == -1){
        printf("Unable to initialize USFAT from \"%s\".\n",argv[1]);
        return 1;
    }

	//Set current directory to root
    set_directory_to(&fat_rt, FATFS_ROOT_SECTOR);
	
	print_directory(fat_rt.dir_buffer);
	printf("Read File (\"DONE\" to quit) > ");
	while(scanf("%s", filename) != 0){

		if(!strcmp(filename, "DONE")) break;
		
		if(!read_file( &fat_rt, filename)){
			printf("\"%s\" not found!\n", filename);
		} else {
            printf("\n");   //additional newline for readability
        }

		print_directory(fat_rt.dir_buffer);
		printf("Read File (\"DONE\" to quit) > ");
	}
 
	return 0;
}
