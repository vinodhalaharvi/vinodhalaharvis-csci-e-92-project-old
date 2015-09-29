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

    initmemory(); 
    string1 = (char * ) mymalloc(100); 
    string2 = (char * ) mymalloc(100); 
    string3 = (char * ) mymalloc(100); 
    (void *) string1; 
    (void *) string2; 
    (void *) string3; 
    //mygraphnode(); 
    printlist(); 
    return 0;
}
