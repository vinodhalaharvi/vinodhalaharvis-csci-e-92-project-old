#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "limits.h"
#include "mymalloc.h"
#define MAX_STRING_LENGTH 1000
#define MAX_COMMAND_LINE_ARGUMENTS 100
#define MAX_LIST_SIZE 1000

#define HEADERSTARTADDR 0
#define DATASTARTADDR 1000

//Author: Vinod Halaharvi
//HUID: 80778287
//(904) 200 1070

static unsigned long totalmemorysize = (1 * 1024 * 1024 * 8);  
//static unsigned long totalheadersize = (1 * 1024 * 1024 * 8) / 32; 
static unsigned long totaldatasize = ((1 * 1024 * 1024 * 8) / 32) * 31;

void initmemory(){ 
    node = (mymalloc_t *) malloc(totalmemorysize);
    assert(node); 
    memset(node, '\0', totalmemorysize); 
    node->addr = 0; 
    node->available = true; 
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

void split(mymalloc_t * node, unsigned size){ 
    mymalloc_t * temp; 
    assert(node->size >= size); 
    temp = (mymalloc_t *) malloc(sizeof(mymalloc_t)); 
    assert(temp); 
    memset(temp, '\0', sizeof(mymalloc_t)); 
    temp->next = node->next; 
    node->next->prev = temp;
    temp->prev = node; 
    node->next = temp; 
    temp->size = size; 
    node->size = node->size - size; 
    return;
}

void *myMalloc(unsigned size){ 
    assert (size > 0); 
    while(node){ 
        if (hole(node) && node->size >= size){ 
            split(node, size); 
            return (void *) node->addr; 
        }
        node = node->next; 
    }
    return NULL; 
}

void myfree(void *ptr){ 
    assert((unsigned *) ptr > 0); 
    while(node){ 
        if (ptr == node->addr){ 
            assert(!hole(node)); 
            assert(getcurrentprocessid() == node->pid); 
            merge(node); 
        }
        node = node->next; 
    }
}

int main(int argc, char *argv[])
{
    initmemory(); 
    return 0;
}
