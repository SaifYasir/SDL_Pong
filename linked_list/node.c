#include "node.h"

#include <stdio.h>
#include <stdlib.h>

void insert_node(node **start_pointer, int value_x, int value_y){
    node *new_node;
    new_node = malloc(sizeof(node));

    new_node -> coordinate[0] = value_x;
    new_node -> coordinate[1] = value_y;

    new_node -> next_ptr = NULL;

    node *current_ptr = *start_pointer;

    if(new_node != NULL){
        if(current_ptr == NULL){
            *start_pointer = new_node;
        }
        else{
            while(current_ptr -> next_ptr != NULL){
                current_ptr = current_ptr -> next_ptr;
            }
            current_ptr -> next_ptr = new_node;
        }
    }
    else{
        printf("Unable to add node");
    }
}

void delete_node(node **start_pointer){
    node *current_pointer = *start_pointer;
    node *previous_pointer;

    if(current_pointer == NULL){
        printf("List is already empty");
    }else if (current_pointer -> next_ptr){
        *start_pointer = NULL;
        free(current_pointer);
    }
    else{
        previous_pointer = current_pointer;
        while (current_pointer -> next_ptr != NULL)
        {
            previous_pointer = current_pointer;
            current_pointer = current_pointer -> next_ptr;
        }

        previous_pointer->next_ptr = NULL;
        free(current_pointer);
    }
}

/**
 * returns node value, when traverse depth = 0, the first value in linked list is given
*/
int* get_node_value(node **start_pointer, int traverse_depth){
    node *current_pointer = *start_pointer;

    while(traverse_depth > 0){
        if(current_pointer == NULL){
            printf("No value to display, list does not go that deep in depth");
            return -1;
        }
        current_pointer = current_pointer -> next_ptr;
        traverse_depth--;
    }
    int* coordinates = &(current_pointer -> coordinate);
    return coordinates;
}