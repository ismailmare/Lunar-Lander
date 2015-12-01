//
//  draw_ship.h
//  Lunar Lander
//
//  Created by Ismail Mare on 2015-03-24.
//  Copyright (c) 2015 Ismail Mare. All rights reserved.
//

#ifndef __Lunar_Lander__draw_ship__
#define __Lunar_Lander__draw_ship__

#include <stdio.h>


# define PROG_LEN 128 // Length of program length

# define LINE_LEN 256 // Max length of line

#define PAUSE_LEN 5 // Length of pause instruction

#define PI acos(-1.0)

long int lround(double x);
void eraseSegment(FILE* executable, double* ship_coor_x, double* ship_coor_y, int num_coor);
void translate(double* trans_x, double* trans_y, double* ship_coor_x, double* ship_coor_y, int num_coor);
void rotate(double angle,double* ship_coor_x, double* ship_coor_y, int num_coor, FILE* debug_log);

//void tran(double* ship_coor_x, double* trans_x, int num_coor)
#endif /* defined(__Lunar_Lander__draw_ship__) */

