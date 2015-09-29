#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "listgraph.h"

/*int main(int argc, char *argv[])
{
    char * header = " digraph G { \n nodesep=.05; \n rankdir=LR; \n node [shape=record,width=.1,height=.1]; \n node [width = 1.5];";
    //pid, addr, size, available
    //mymalloc_t * node = allocnode(0, (void * ) 8, 10, true); 
    //mymalloc_t * node1 = allocnode(1, (void *) 8, 10, true); 
    mymalloc_t * node[10]; 
    unsigned count = 10;
    unsigned i = 0 ; 
    for (i = 0; i < count; ++i) {
        node[i] = allocnode(i, (void *) 8, 10, true); 
    } 
    for (i = 0; i < count-1; ++i) {
        node[i]->next = node[i+1]; 
        node[i+1]->prev = node[i]; 
    }
    node[0]->prev = NULL; 
    node[9]->next = NULL; 
    fprintf(stdout, "%s\n", header);
    fprintf(stdout, "%s\n", listgraph(node[0])); 
    fprintf(stdout, "%s\n", "}");
    return 0;
}
*/


char * tostring(mymalloc_t * node){ 
    char * string = allocstring(100); 
    sprintf(string, "%s  [label = \" { %s }\"];", label(node), fields(node));
    return string; 
}

char * allocstring(unsigned size){ 
    char * string =  (char * ) malloc(size); 
    assert(string); 
    memset(string, '\0', size); 
    return string; 
}

char * label(mymalloc_t * node){ 
    char * string; 
    char * format = "%s%d"; 
    if (!node) 
        return ""; 
    string = allocstring(100); 
    assert(node);
    sprintf(string, format, "node", node->pid); 
    return string; 
}

char * fields(mymalloc_t * node){ 
    char * string = allocstring(100); 
    char * format = "<addr> %p | <size> %u | <av> %s" ; 
    sprintf(string, format, 
            node->addr, 
            node->size, 
            booltostring(node->available));
    return string; 
}

char *  graph(mymalloc_t * node){ 
    char * string = allocstring(10000); 
    mymalloc_t * trav = node; 
    while(trav){ 
        sprintf(string, "%s%s\n", string, tostring(trav));
        if (trav->next)
            sprintf(string, "%s%s -> %s;\n", string, label(trav), label(trav->next)); 
        if (trav->prev)
            sprintf(string, "%s%s -> %s;\n", string, label(trav), label(trav->prev)); 
        trav = trav->next; 
    }
    return string; 
}


mymalloc_t *  allocnode(unsigned pid, void * addr, 
        unsigned size, boolean available){ 
    mymalloc_t * node =  (mymalloc_t *) malloc(sizeof(mymalloc_t));  
    assert(node);
    memset(node, '\0', sizeof(mymalloc_t)); 
    node->pid = pid; 
    node->addr = addr; 
    node->size = size; 
    node->available = available; 
    return node; 
}

void graphnode(mymalloc_t * node){ 
    char * header = " digraph G { \n nodesep=.05; \n rankdir=LR; \n node [shape=record,width=.1,height=.1]; \n node [width = 1.5];";
    fprintf(stdout, "%s\n", header);
    fprintf(stdout, "%s\n", graph(node)); 
    fprintf(stdout, "%s\n", "}");
}
