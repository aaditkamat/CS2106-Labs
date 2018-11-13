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
int check_file_name(uint8_t *name, char filename[], int length) {
  for (int i = 0; i < length; i++) {
    if (name[i] != filename[i]) {
       return 0;
    }
  }  
  return 1;
}

int read_file( FAT_RUNTIME* rt, char filename[])
{
    //TODO: Your code here
	for (int i = 0; i < S_SECTOR_SIZE; i++) {
          uint8_t* name = rt -> dir_buffer[i].name;
	  char *sfn = malloc(13);
          to_sfn(filename, sfn);
          
          if (check_file_name(name, sfn, 11) == 1) {
            int total_size = rt -> dir_buffer[i].file_size;
            uint16_t sector = rt -> dir_buffer[i].start_sector; 
              while(sector != FE_END){
               uint8_t* data;
               int size = 0; 
               if (total_size < S_SECTOR_SIZE) {
                  data = malloc(total_size);
                  size = total_size;
               }
	       else {
                 data = malloc(S_SECTOR_SIZE);
                 size = S_SECTOR_SIZE;
               }
               total_size -= S_SECTOR_SIZE;
               if (sector != FE_BAD && sector != FE_FREE) {
                  read_data_sector(rt -> media_f, sector, data);
               }
               print_as_text(data, size);
               sector = rt -> fat[sector];
             } 
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
