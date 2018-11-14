/*************************************
 * Lab 5 Exercise 3
 * Name: Aadit Kamat
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

//TODO: Add helper functions if needed
int check_file_name(uint8_t *name, char filename[], int length) {
    char *sfn = malloc(13);
    to_sfn(filename, sfn);
    for (int i = 0; i < length; i++) {
        if (name[i] != filename[i]) {
            return 0;
        }
    }  
    return 1;
}

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

/**
 * Import a file content into USFAT based filesystem
 */
int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector)
{
    int ctr = 0, sector_number = 0;
    for (int i = 0; i < S_SECTOR_SIZE; i++) {
        uint8_t* name = rt -> dir_buffer[i].name;
        //checks whether the directory has 8 or more files
        if (i >= 8 && rt -> dir_buffer[i].file_size == 0) {
                #ifdef DEBUG
                printf("\nThe directory has 8 or more files\n");
                #endif
                return -1;
            }
        char *sfn = malloc(13);
        to_sfn(filename, sfn);

        if (check_file_name(name, sfn, 11) == 1) {
            sector_number = i;
            ctr++;          
        }
     }
    
    //checks if the file is found and if there is a duplicate of the given file
    if (open(filename, O_RDONLY) == -1) {
        #ifdef DEBUG
        printf("\nThe file is not found\n");
        #endif
        return -1;
    }

    if (ctr > 1) {
       #ifdef DEBUG
       printf("\nThere is a duplicate of the given file\n");
       #endif
       return -1;   
    }
    
    ctr = 0;
    int start_sector = try_sector;
    do {
        if (rt -> fat[start_sector] == FE_FREE) {
            ctr++;
        }
        start_sector = (start_sector + 1) % FATFS_FAT_ENTRY_NUMBER;
    } while (start_sector != try_sector); 
    //checks if there is a free sector
    if (ctr == 0) {
        #ifdef DEBUG
        printf("\nThere is no free sector\n");
        #endif
        return -1;
    }

    int free_sectors[ctr];
    int written_bytes = 0;
    int total_size = rt -> dir_buffer[sector_number].file_size;
    #ifdef DEBUG
        printf("\nThere is no free sector\n");
        #endif
    get_read_sectors(rt, filename, read_sectors);
    int i;
    uint8_t* data;
    for (i = 0; i < number_of_read_sectors - 1; i++) {
        data = malloc(S_SECTOR_SIZE);
        //reading from the existing sectors and writing to the free sectors one by one
        read_data_sector(rt -> media_f, read_sectors[i], data);
        print_as_text(data, S_SECTOR_SIZE);
        write_data_sector(rt -> media_f, free_sectors[i], data);
        written_bytes += S_SECTOR_SIZE;
        rt -> fat[free_sectors[i]] = free_sectors[i + 1];
    }
    //reading and writing for the last data sector
    data = malloc(S_SECTOR_SIZE);
    read_data_sector(rt -> media_f, read_sectors[i], data);
    int size = total_size % S_SECTOR_SIZE;
    written_bytes += size;
    print_as_text(data, size);
    write_data_sector(rt -> media_f, free_sectors[i], data);
    rt -> fat[free_sectors[i]] = FE_END;

    FAT_DE new_de;
    construct_DE(&new_de, filename, 0, free_sectors[0], total_size);
    rt -> dir_buffer 
    return written_bytes;
}


int main(int argc, char** argv)
{
    FAT_RUNTIME fat_rt;
    char filename[13];
    int result;

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
    printf("Import File (\"DONE\" to quit) > ");
    while(scanf("%s", filename) != 0){

        if(!strcmp(filename, "DONE")) break;

        int start_sector;

        printf("Start sector (in Hex) > 0x");
        scanf("%x", &start_sector);

        printf("Import \"%s\" to [0x%04x] Data Sector...", filename, start_sector);

        if((result = import_file(&fat_rt, filename, start_sector)) == -1){
            printf("FAILED!\n");
        } else {
            printf("Written %d bytes.\n", result);
        } 

        print_directory(fat_rt.dir_buffer);
        printf("\nImport File (\"DONE\" to quit) > ");
    }

    return 0;
}
