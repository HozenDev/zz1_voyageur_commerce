#ifndef _utils_h_
#define _utils_h_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../sdl/sdl.h"


float distance(SDL_Point pt1, SDL_Point pt2);
void free_matrix_char(char ** matrix, unsigned int n);
void free_matrix_float(float ** matrix, unsigned int n);

#endif
