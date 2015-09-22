#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "map.h"
#include <assert.h>

//Author: Vinod Halaharvi
//Very rudimentary doubly linked list 
//to keep track of environment variables

/*int main(int argc, char *argv[])
{
    node_type * environ = NULL; 
    //map.c testing..
    environ = put(environ, "key1", "value1");
    environ = put(environ, "key2", "value2");
    environ = put(environ, "key3", "value3");
    assert(contains(environ, "key1") == true); 
    assert(contains(environ, "key2") == true); 
    assert(contains(environ, "key3") == true); 
    delete(&environ, "key2"); 
    assert(contains(environ, "key2") == false); 
    print(environ); 
    return 0; 
}*/

//helper function to print the list
void print(node_type * environ){
    int i = 0;  
    while(environ){
        fprintf(stdout, "%s = %s\n", environ->key, environ->value);
        environ = environ->next;
    }
}

//get the value for a given key
value_type get(node_type * node, key_type key){
    int i = 0;  
    while(node){
        if (match(node->key, key)){ 
            return node->value; 
        }
        node = node->next;
    }
    return NULL; 
}

//nothing is freed yet. 
//will implement in future release
void freenode(node_type * node){ 
    free(node->key); 
    free(node->value); 
    free(node->next); 
    free(node->prev); 
    free(node); 
}


//delete from the list
//many edge cases to consider
void delete(node_type **head, key_type key){
    int i = 0;  
    node_type * node = *head; 
    node_type * temp; 
    while(node){
        if (match(node->key, key)){ 
            //delete the entire list if there is only one node
            if (node->prev == NULL && node->next == NULL){ 
                //freenode(node); 
                *head = NULL; 
                return;
            }
            //delete intermediate node
            if (node->prev && node->next){ 
                node->prev->next = node->next; 
                node->next->prev = node->prev; 
                //freenode(node); 
                return; 
            }
            //delete last node
            if(node->next == NULL) { 
                node->prev->next = NULL; 
                //freenode(node); 
                return; 
            }

            //delete first node
            if(node->prev == NULL) { 
                *head = node->next; 
                //freenode(node); 
                return; 
            }
        }
        node = node->next; 
    }
    return;
}


//put the key, value pair in this list
node_type * put(node_type * node, key_type key, value_type value){
    node_type * temp = (node_type *) malloc(sizeof(node_type)); 
    memset(temp, '\0', sizeof(node_type)); 
    temp->key = key; 
    temp->value = value; 
    temp->next = NULL; 
    temp->prev = NULL; 
    if (node){ 
        temp->next = node; 
        node->prev = temp; 
    } else { 
        temp->next = NULL; 
        temp->prev = NULL; 
    }
    return temp; 
}


//simple helper function for string equals
boolean match(key_type key, key_type otherkey) { 
    return (strcmp(key, otherkey) == 0)? 1 : 0; 
}
 
//check if list contains the key
boolean contains(node_type * node, key_type key){
    int i = 0;  
    while(node){
        if (match(node->key, key)){ 
            return true; 
        }
        node = node->next;
    }
    return false; 
}
