/*************************************
 * Lab 5 Exercise 3
 * Name: Aadit Kamat
 * Student No: A0164761B
 * Lab Group: 02
 *************************************/
#define FILENAME_LENGTH 11
#define FULL_SIZE 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/USFAT.h"

//TODO: Add helper functions if needed
int check_file_name(uint8_t *name, char filename[]) {
    char *sfn = malloc(FILENAME_LENGTH);
    to_sfn(filename, sfn);
    for (int i = 0; i < FILENAME_LENGTH; i++) {
        if (name[i] != sfn[i]) {
            return 0;
        }
    }  
    return 1;
}

int check_root_directory(FAT_RUNTIME* rt, char filename[]) {
    int same_file_ctr = 0, i;
    for (i = 0; i < FULL_SIZE; i++) {
        if (rt -> dir_buffer[i].file_size == 0) {
            break;
        }

#ifdef DEBUG
        printf("\n%i\n", check_file_name(rt -> dir_buffer[i].name, filename));
#endif
       if (check_file_name(rt -> dir_buffer[i].name, filename) == 1) {
           same_file_ctr++;
       }
    }
    if (same_file_ctr >= 1) {
        #ifdef DEBUG
           printf("\nThere is a duplicate of the given file in the root directory.\n");
        #endif
        return -1;
    }
    if (i == FULL_SIZE) {
        #ifdef DEBUG
           printf("\nThe root directory is full.\n");
        #endif
        return -1;
    }
    return 0;
}

int check_free_sectors(FAT_RUNTIME* rt, uint16_t try_sector, uint16_t free_sectors[]) {
    int ctr = 0;
    int current_sector = try_sector;
    do {
        if (rt -> fat[current_sector] == FE_FREE) {
            free_sectors[ctr++] = (uint16_t) current_sector;
        }
        current_sector = (current_sector + 1) % FATFS_FAT_ENTRY_NUMBER;
    } while (current_sector != try_sector);
    if (ctr == 0) {
    #ifdef DEBUG
        printf("\nNo free sectors\n");
    #endif
        return -1;
    }
    return ctr;
}

void read_and_write_appropriate_size(FAT_RUNTIME* rt, uint16_t free_sector, off_t* size_left, int filedes) {
    off_t write_size;
    if (*size_left < S_SECTOR_SIZE) {
        write_size = *size_left;
    } else {
        write_size = S_SECTOR_SIZE;
    }
#ifdef DEBUG
    printf("\n size_left: %i write_size: %i\n", size_left, write_size);
#endif
    uint8_t data[write_size];
    read(filedes, data, (size_t) write_size);
    off_t offset = (off_t) write_size;
    lseek(filedes, offset, SEEK_CUR);
    *size_left -= write_size;
    write_data_sector(rt -> media_f, free_sector, data);
}

off_t copy_to_free_sectors(FAT_RUNTIME *rt, uint16_t free_sectors[], int number_of_free_sectors, int filedes, off_t* file_size) {
    *file_size = lseek(filedes, 0, SEEK_END);
    off_t size_left = *file_size;
    lseek(filedes, 0, 0);
    for (int i = 0; i < number_of_free_sectors; i++) {
        read_and_write_appropriate_size(rt, free_sectors[i], &size_left, filedes);
        if (size_left == 0) {
            rt -> fat[free_sectors[i]] = FE_END;
            break;
        }
        if (i < number_of_free_sectors - 1) {
            rt -> fat[free_sectors[i]] = free_sectors[i + 1];
        } else {
            rt -> fat[free_sectors[i]] = FE_END;
        }
    }
    return *file_size - size_left;
}

void add_new_DE(FAT_RUNTIME* rt, char filename[], uint16_t try_sector, uint32_t file_size) {
    FAT_DE new_de;
    construct_DE(&new_de, filename, 0, try_sector, file_size);
    for (int i = 0; i < S_SECTOR_SIZE; i++) {
        if (rt -> dir_buffer[i].file_size == 0) {
            rt -> dir_buffer[i] = new_de;
            break;
        }
    }
}

/**
 * Import a file content into USFAT based filesystem
 */
int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector)
{
    int filedes = open(filename, O_RDONLY);
    if (filedes == -1) {
        return -1;
    }

    if (check_root_directory(rt, filename) == -1) {
        return -1;
    }

    uint16_t free_sectors[FATFS_FAT_ENTRY_NUMBER];
    int result = check_free_sectors(rt, try_sector, free_sectors);
    if (result == -1) {
        return -1;
    }
    off_t file_size;
#ifdef DEBUG
    print_fat_debug(rt -> fat);
    printf("\n");
#endif
    off_t written_bytes = copy_to_free_sectors(rt, free_sectors, result, filedes, &file_size);
#ifdef DEBUG
    print_fat_debug(rt -> fat);
    printf("\n");
#endif

    add_new_DE(rt, filename, free_sectors[0], written_bytes);

    shutdown_runtime(rt);

    return (int) written_bytes;
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
