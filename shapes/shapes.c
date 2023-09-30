#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shapes.h"
#include "node.h"

int* get_filled_circle(int x_pos_midpoint, int y_pos_midpoint, int radius){

//equation of a circle is (x - a)2 + (y - b)2 = r2

//equation of next x point is (x + x_pos_midpoint)^2 - 2y(y_pos_midpoint + 1) - (b + 1)^2

//first x pos to use
node *first_coordinate = NULL;


}