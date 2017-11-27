/* Compile as C11 */
#define _POSIX_C_SOURCE 200809L

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    struct timespec buf_start; 

    int r= clock_gettime(CLOCK_REALTIME_COARSE, &buf_start);
    if (r != 0) {
        perror("clock_gettime"); 
        exit(1); 
    }
    

    char filename[10] ;
    fgets(filename, 10, stdin);
    filename[strlen(filename)-1]='\0';

    FILE *file= fopen(filename, "w");
    if (file == NULL) {
        perror("A");
        exit(1); 
    }
    r= fputs("aaa\n", file); 
    if (r == EOF) {
        perror("A"); 
        exit(1);
    }
    r= fclose(file); 
    if (r != 0) {
        perror("A"); 
        exit(1); 
    }


    struct stat stat_file;

    r= stat(filename, &stat_file);
    if (r != 0) {
        perror("A"); 
        exit(1); 
    }

    r= printf("%ld.%9ld\n%ld.%9ld\n",
          buf_start.tv_sec, buf_start.tv_nsec,
          stat_file.st_mtim.tv_sec, stat_file.st_mtim.tv_nsec);

    //r= printf("clock: %s\n mtime: %s",
     //     ctime(&buf_start), ctime(&stat_file.st_mtim));
    if (r < 0) {
        perror("printf"); 
        exit(1); 
    }

    exit(0);
}
