#ifndef _MAP_H
#define _MAP_H value
//Author: Vinod Halaharvi
typedef char * key_type; 
typedef char * value_type; 
typedef enum { false, true } boolean; 
#define MAX_LIST_SIZE 100

typedef struct node_type {
    key_type key; 
    value_type value; 
    struct node_type * next; 
    struct node_type * prev; 
    int size; 
} node_type;

value_type get(node_type * node, key_type key); 
node_type *put(node_type * node, key_type key, value_type value); 
boolean contains(node_type * node, key_type key); 
void delete(node_type **head, key_type key); 
boolean match(key_type key, key_type otherkey); 
void freenode(node_type * node); 

/* misc functions */
void print(node_type * environ);
#endif
