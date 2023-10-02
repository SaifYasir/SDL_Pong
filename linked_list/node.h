#pragma once

typedef struct node{
    int coordinate[2];
    struct node *next_ptr;
} node;

void insert_node(node **start_pointer, int value_x, int value_y);
void delete_node(node **start_pointer);
int* get_node_value(node **start_pointer, int traverse_depth);