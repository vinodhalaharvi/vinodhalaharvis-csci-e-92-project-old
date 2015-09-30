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
static mymalloc_t * start; 

void * align(void * addr){ 
    return (void *)(((unsigned)addr + 7) & (~7)); 
}

void initmemory(){ 
    mymalloc_t * trav; 
    start = (mymalloc_t *) malloc(totalmemorysize);
    assert(start); 
    memset(start, '\0', totalmemorysize); 
    start->addr = align((void *)start + totalheadersize); 
    start->available = true; 
    start->availableasheader = false; 

    trav = start + 1; 
    while(trav <= start + totalheadersize){ 
        trav->available = true; 
        trav->availableasheader = true; 
        trav++; 
    }
    start->pid = 0;
    start->next = NULL; 
    start->prev = NULL; 
    start->size = totaldatasize; 
    return;
}

boolean hole(mymalloc_t * node){ 
    return node->available == true; 
}


boolean headerhole(mymalloc_t * node){ 
    return node->availableasheader == true; 
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
    fprintf(stdout, "address=%p pid=%d size=%d available=%s availableasheader=%s\n",  node->addr, 
            node->pid, node->size, 
            booltostring(node->available), 
            booltostring(node->availableasheader)); 
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
    temp = allocheadernode(); 
    assert(temp); 
    memset(temp, '\0', sizeof(mymalloc_t)); 
    //case a
    node->size = node->size - size; 
    temp->addr = node->addr + node->size; 
    temp->size = size; 
    /*node->available = false; 
    temp->available = true; */
    if (node->size > 0) { 
        node->available = true; 
    }
    temp->available = false;
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
    mymalloc_t * node; 
    node = start; 
    while(node){ 
        //fprintf(stdout, "0x%p\n", (void *)node->addr);
        printnode(node); 
        node = node->next; 
    }
    return; 
}


mymalloc_t * allocheadernode(){ 
    mymalloc_t * trav = start;
    while(trav){ 
        if (headerhole(trav)){ 
            trav->availableasheader = false; 
            return (void * ) trav; 
        }
        trav++;
    }
    assert(0); 
    return NULL; 
}


void *mymalloc(unsigned size){ 
    mymalloc_t * node; 
    node = start; 
    assert (size > 0); 
    assert (size <= totalmemorysize); 
    while(node){ 
        assert(node <= start + totalheadersize); 
        if (hole(node) && node->size >= size){ 
            //fprintf(stdout, "%p\n", node);
            return (void *) split(node, size); 
        }
        node = node->next; 
    }
    assert(0); 
    return NULL; 
}

void myfree(void *ptr){ 
    mymalloc_t * node ; 
    node = start; 
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
    mymalloc_t * node = start; 
    graphnode(node); 
}

