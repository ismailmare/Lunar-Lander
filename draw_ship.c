/* Name: Ismail Mare
 *  ID: 1388973
 *  LECTURE SECTION: B1
 *  UNIX ID: imare
 *  INSTRUCTOR: Smith Jaqueline
 *  LAB SECTION: H02
 *  TA'S NAME: Nicholas Barriga
*/


# include "draw_ship.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <math.h>
# include "main.h"
# include <ctype.h>
# include <math.h>
# include <assert.h>

void tran(double* ship_coor_x, double* trans_x, int num_coor);


void draw_ship(FILE* executable, FILE* debug_log, double* ship_coor_x, double* ship_coor_y, int num_coor ) {
    
    int k=0;
    long x1_r = 0, y1_r = 0, x2_r = 0, y2_r = 0;
    
    //CHANGE TO LROUND
    for(k = 1 ;k < num_coor; k++) {
        
        x1_r = lround(ship_coor_x[k-1]);
        y1_r = lround(ship_coor_y[k-1]);
        x2_r = lround(ship_coor_x[k]);
        y2_r = lround(ship_coor_y[k]);
    }        
        // send
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n"
                , x1_r, y1_r, x2_r, y2_r);
    
    
}


void rotate_ship(FILE* debug_log, FILE* executable, double angle, double* ship_coor_x, double* ship_coor_y, int num_coor) {
    
    
    int k=0;
    long x1_r = 0, y1_r = 0, x2_r = 0, y2_r = 0;
    double trans_x[num_coor], trans_y[num_coor];
    double org_trans_x[num_coor], org_trans_y[num_coor];
 
    tran(ship_coor_x, trans_x, num_coor);
    tran(ship_coor_y, trans_y, num_coor);
    
    for(k = 0 ;k < num_coor; k++) {

	 org_trans_x[k] = (-1.0*trans_x[k]);
	 org_trans_y[k] = (-1.0*trans_y[k]);
    }
	
    // Erase old drawSegment
    eraseSegment(executable, ship_coor_x, ship_coor_y, num_coor);

    // Translate to midpoint(0,0)
    translate(org_trans_x, org_trans_y, ship_coor_x, ship_coor_y, num_coor);

    
    //Rotate from midpoint(0,0) 
    rotate(angle, ship_coor_x, ship_coor_y, num_coor, debug_log);

		
    // Translate back to original.
    translate(trans_x, trans_y, ship_coor_x, ship_coor_y, num_coor);
    
    // Draw new drawSegment 
    for(k = 1 ;k < num_coor; k++) {
        
        x1_r = lround(ship_coor_x[k-1]);
        y1_r = lround(ship_coor_y[k-1]);
        x2_r = lround(ship_coor_x[k]);
        y2_r = lround(ship_coor_y[k]);
        
            // send
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n"
                ,x1_r, y1_r, x2_r, y2_r);
    }
    
    fflush(executable);
}


// rotate the ship
void rotate(double angle,double* ship_coor_x, double* ship_coor_y, int num_coor, FILE* debug_log) {
    
    int k=0;
    double temp_org = 0; 
    
    for(k = 0 ;k < num_coor ; k++) {
    
	temp_org = ship_coor_x[k];

        ship_coor_x[k] = ship_coor_x[k] * cos(angle)
            - ship_coor_y[k] * sin(angle);
        
        ship_coor_y[k] = temp_org * sin(angle)
            + ship_coor_y[k] * cos(angle);

    	//trans_x[k] +=  ship_coor_x[k] - O_trans_x[k];
    	//trans_y[k] +=  ship_coor_y[k] - O_trans_y[k];
    } 

}

//Translate to midpoint and back after rotation
void translate(double* trans_x, double* trans_y, double* ship_coor_x, double* ship_coor_y, int num_coor) {
    
    int k=0;
    
    // shift the coordinates in the struct
    for(k = 0 ;k < num_coor; k++) {
        
        ship_coor_x[k] += trans_x[k];
        ship_coor_y[k] += trans_y[k];

    }
}

void eraseSegment(FILE* executable, double* ship_coor_x, double* ship_coor_y, int num_coor) {
    
    int k=0;    
    long x1_r = 0, y1_r = 0, x2_r = 0, y2_r = 0;
    for(k = 1 ;k < num_coor; k++) {
    
        x1_r = lround(ship_coor_x[k-1]);
        y1_r = lround(ship_coor_y[k-1]);
        x2_r = lround(ship_coor_x[k]);
        y2_r = lround(ship_coor_y[k]);
        
        // send
        fprintf(executable, "eraseSegment %ld %ld %ld %ld\n"
                , x1_r, y1_r, x2_r, y2_r );
    }

}
    	
void tran(double* ship_coor_x, double* trans_x, int num_coor) {

    double trans=0;
    double minimum = 0.0;
    double maximum = 0.0;
    int c =0;
    maximum = ship_coor_x[0];
 
    for (c = 1; c < num_coor; c++) {
  
        if (ship_coor_x[c] > maximum) {

           maximum  = ship_coor_x[c];
        }
    }

    minimum = ship_coor_x[0];
    for (c = 1; c < num_coor; c++) {
      
        if (ship_coor_x[c] < minimum) {
       
           minimum  = ship_coor_x[c];
        }  
    }
    trans = (minimum+maximum) / 2;
    
    for( c = 0; c < num_coor; c++) {
		
	trans_x[c] = trans;
    }
}
