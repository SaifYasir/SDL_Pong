#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "shapes.h"

node* get_hollow_circle(int x_pos_midpoint, int y_pos_midpoint, int radius){
    //first x pos to use
    node *all_coordinates = NULL;
    int fill_count = 0;
    int x_coordinate = radius;
    int y_coordinate = 0;

    insert_node(&all_coordinates, x_coordinate + x_pos_midpoint,y_coordinate + y_pos_midpoint);
    

    //Check this lad
    while(atan((double)(y_coordinate)/(double)(x_coordinate)) < (M_PI / 4)){
        //always increment ycoord by 1 first before doing x calculation
        y_coordinate++;
        x_coordinate = sqrt(pow(radius,2) - pow(y_coordinate,2));

        
        insert_node(&all_coordinates, x_coordinate + x_pos_midpoint, y_coordinate + y_pos_midpoint);
        insert_node(&all_coordinates, y_coordinate + x_pos_midpoint, x_coordinate + y_pos_midpoint);

        insert_node(&all_coordinates, - x_coordinate + x_pos_midpoint, - y_coordinate + y_pos_midpoint);
        insert_node(&all_coordinates, - y_coordinate + x_pos_midpoint, - x_coordinate + y_pos_midpoint);

        insert_node(&all_coordinates, - x_coordinate + x_pos_midpoint, y_coordinate + y_pos_midpoint);
        insert_node(&all_coordinates, x_coordinate + x_pos_midpoint, - y_coordinate + y_pos_midpoint);

        insert_node(&all_coordinates, - y_coordinate + x_pos_midpoint, x_coordinate + y_pos_midpoint);
        insert_node(&all_coordinates, y_coordinate + x_pos_midpoint, - x_coordinate + y_pos_midpoint);     
    }
    return all_coordinates;
}













