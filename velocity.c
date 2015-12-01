/* Name: Ismail Mare
 *  ID: 1388973
 *  LECTURE SECTION: B1
 *  UNIX ID: imare
 *  INSTRUCTOR: Smith Jaqueline
 *  LAB SECTION: H02
 *  TA'S NAME: Nicholas Barriga
*/



# include "velocity.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <math.h>
# include "main.h"
# include <ctype.h>
# include <math.h>
# include <assert.h>


void velocity_up(FILE* executable, double thrust, double gravity, double angle, double* ship_coor_x, double* ship_coor_y, int num_coor) {

    double xA = 0;
    double yA = 0;
    int k=0;
    static double vert_vel = 0;
    static double horz_vel = 0;
    double angle_radian = 0.0;
    
    angle_radian = angle * PI/180.0;   
 
    xA = thrust * cos(angle_radian);
    yA = gravity + thrust * sin(angle_radian);
    
    vert_vel += yA + thrust *sin(angle_radian);
    horz_vel += xA + thrust *cos(angle_radian);
   
    eraseSegment(executable, ship_coor_x, ship_coor_y, num_coor);    
    for(k = 0 ;k < num_coor; k++) {
    
        ship_coor_x[k] += horz_vel;
        ship_coor_y[k] += vert_vel;
    
    }     
}


