#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "listgraph.h"
#include "mymalloc.h"

int main(int argc, char *argv[])
{
    char * string1; 
    char * string2; 
    char * string3; 
    char * string4; 
    initmemory(); 
    string1 = (char * ) mymalloc(100); 
    string2 = (char * ) mymalloc(100); 
    string3 = (char * ) mymalloc(100); 
    string4 = (char * ) mymalloc(100); 

    strcpy(string1, "hello"); 
    strcpy(string2, "world"); 
    strcpy(string3, "and"); 
    strcpy(string4, "somethingelse"); 
    fprintf(stdout, "%s\n", string1);
    fprintf(stdout, "%s\n", string2);
    fprintf(stdout, "%s\n", string3);
    fprintf(stdout, "%s\n", string4);
    //mygraphnode(); 
    printlist(); 
    return 0;
}
