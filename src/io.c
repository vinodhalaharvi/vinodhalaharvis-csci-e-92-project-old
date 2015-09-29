#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "mymalloc.h"

//Author: Vinod Halaharvi
//HUID: 80778287
//vinod.halaharvi@gmail.com, (904) 200 1070
typedef enum {
    LED, PUSHBUTTON, FILESYS
} device_type_t;


typedef struct _device_t {
    device_type_t device_type; 
    (int)(*fputc)(int c, unsigned deviceno); 
    (int)(*fgetc)(unsigned deviceno); 
    (int)(*fclose)(unsigned deviceno); 
    (int)(*fopen)(unsigned deviceno); 
    union { 
        struct _led {
            /* data */
        } led;
        struct _file {
            /* data */
        } file;
        struct _pushbutton {
            /* data */
        } pushbutton;
        struct _file{ 
            char * name;
            (void)(*create)(char *name); 
            (void)(*delete)(char *name); 
            enum {
                READONLY, READWRITE, WRITEONLY
            } fileops;
        } file_t; 
    } data; 
} device_t;


typedef struct _stream_t {
    /* data */
    device_t * device;
    unsigned deviceno; 
} stream_t;


int main(int argc, char *argv[])
{
    
    return 0;
}

int fputc(int c, unsigned deviceno){
}

int fgetc(unsigned deviceno){
} 

int fclose(unsigned deviceno){
} 

int fopen(unsigned deviceno){
} 
