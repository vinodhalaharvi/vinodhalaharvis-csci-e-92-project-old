#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "limits.h"
#include "mymalloc.h"
#include "listgraph.h"

#define MAX_STRING_LENGTH 1000
#define MAX_COMMAND_LINE_ARGUMENTS 100
#define MAX_LIST_SIZE 1000

#define HEADERSTARTADDR 0
#define DATASTARTADDR 1000

//Author: Vinod Halaharvi
//HUID: 80778287
//(904) 200 1070

//static unsigned long totalmemorysize = (1 * 1024 * 1024 * 8);  
//static unsigned long totalheadersize = (1 * 1024 * 1024 * 8) / 32; 
//static unsigned long totaldatasize = ((1 * 1024 * 1024 * 8) / 32) * 31;

static unsigned long totalmemorysize = 1024 * 8; 
static unsigned long totalheadersize = 512 * 1; 
static unsigned long totaldatasize = 1024 * 8  - 512; 
static mymalloc_t * node; 

void * align(void * addr){ 
    return (void *)(((unsigned)addr + 7) & (~7)); 
}

void initmemory(){ 
    node = (mymalloc_t *) malloc(totalmemorysize);
    assert(node); 
    memset(node, '\0', totalmemorysize); 
    node->addr = align((void *)node + totalheadersize); 
    /*fprintf(stdout, "node, align(node), node->addr: %p, %p, %p\n", 
            node, align((void*)node), node->addr);*/
    node->available = true; 
    node->pid = 0;
    node->next = NULL; 
    node->prev = NULL; 
    node->size = totaldatasize; 
    return;
}

boolean hole(mymalloc_t * node){ 
    return node->available == true; 
}

unsigned getcurrentprocessid(){ 
    return 0; 
}

char * booltostring(boolean bool){ 
    switch (bool) {
        case false:
            return "false"; 
            break;
        case true:
            return "true"; 
        default:
            assert(0); 
    }
}

void printnode(mymalloc_t * node) { 
    fprintf(stdout, "address=%p pid=%d size=%d available=%s\n",  node->addr, 
            node->pid, node->size, booltostring(node->available)); 
}

void printmemory(mymalloc_t * node){ 
    while(node){ 
        printnode(node); 
        node = node->next; 
    }
}

void nodefree(mymalloc_t *node){ 
    free(node);
}

void merge(mymalloc_t *node){ 
    //case a from the book
    if (node->prev->available && node->next->available){ 
        node->available = true; 
    //case b from the book
    } else if (node->prev->available && !node->next->available){ 
        mymalloc_t * temp; 
        temp = node->next; 
        node->available = true; 
        node->size = node->size + node->next->size;
        node->next = node->next->next; 
        node->next->next->prev = node; 
        nodefree(temp); 
    //case c from the book
    } else if (!node->prev->available && node->next->available){ 
        mymalloc_t * temp; 
        temp = node->prev; 
        node->available = true; 
        node->size = node->size + node->prev->size;
        node->prev = node->prev->prev; 
        node->prev->prev->next = node; 
        nodefree(temp); 
    //case d from the book
    } else { 
        mymalloc_t * temp; 
        temp = node->prev; 
        node->prev->prev->next = node->next->next; 
        node->next->next->prev = node->prev->prev; 
        nodefree(temp); 
    }
    return; 
}

mymalloc_t *  split(mymalloc_t * node, unsigned size){ 
    mymalloc_t * temp; 
    assert(node); 
    assert(node->size >= size); 
    //temp = (mymalloc_t *) malloc(sizeof(mymalloc_t)); 
    temp = node + 1;  //take the next slot
    //assert(temp); 
    memset(temp, '\0', sizeof(mymalloc_t)); 
    //case a
    temp->addr = node->addr + size; 
    temp->size = size; 
    node->size = node->size - size; 
    node->available = false; 
    temp->available = true; 
    if (node->next){ 
        temp->prev = node; 
        temp->next = node->next; 
        node->next->prev = temp;
        node->next = temp; 
    } else { 
        temp->prev = node; 
        temp->next = NULL;
        node->next = temp; 
    }
    return temp->addr;
}

void printlist(){
    while(node){ 
        fprintf(stdout, "0x%p\n", (void *)node->addr);
        node = node->next; 
    }
    return; 
}

void *mymalloc(unsigned size){ 
    //return (void *) node->addr; 
    assert (size > 0); 
    while(node){ 
        if (hole(node) && node->size >= size){ 
            return (void *) split(node, size); 
            //return (void *) node->addr; 
        }
        node = node->next; 
    }
    assert(0); 
    return NULL; 
}

void myfree(void *ptr){ 
    assert((unsigned *) ptr > 0); 
    while(node){ 
        if (ptr == node->addr){ 
            assert(!hole(node->addr)); 
            assert(getcurrentprocessid() == node->pid); 
            merge(node); 
        }
        node = node->next; 
    }
}

void mygraphnode(){ 
    graphnode(node); 
}

