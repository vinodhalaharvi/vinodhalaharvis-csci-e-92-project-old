#ifndef _MYMALLOC_H
#define _MYMALLOC_H 1
typedef enum { false, true } boolean; 

typedef struct _mymalloc_t {
    unsigned pid; 
    void * addr; 
    unsigned size; 
    boolean available; 
    boolean availableasheader; 
    struct _mymalloc_t * next; 
    struct _mymalloc_t * prev; 
} mymalloc_t;



void *mymalloc(unsigned int size);
void myfree(void *ptr);
void initmemory(); 
boolean hole(mymalloc_t * node); 
unsigned getcurrentprocessid(); 
char * booltostring(boolean bool); 
void printnode(mymalloc_t * node) ; 
void printmemory(mymalloc_t * node); 
void merge(mymalloc_t *node); 
mymalloc_t * split(mymalloc_t * node, unsigned size);
void printboolean(boolean bool); 
void printnode(mymalloc_t * node);
void nodefree(mymalloc_t *node); 
void * align(void * addr); 
void mygraphnode(); 
void printlist(); 
mymalloc_t * allocheadernode(); 
#endif
