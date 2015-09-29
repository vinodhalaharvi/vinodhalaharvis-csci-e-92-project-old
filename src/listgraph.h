#ifndef LISTGRAPH
#define LISTGRAPH value
#include "mymalloc.h"
char * tostring(mymalloc_t * node); 
char * allocstring(unsigned size); 
char * label(mymalloc_t * node); 
char * fields(mymalloc_t * node); 
void graphnode(mymalloc_t * node); 
char * graph(mymalloc_t * node); 
mymalloc_t *  allocnode(unsigned pid, void * addr, 
        unsigned size, boolean available); 

#endif

