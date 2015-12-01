/*Name: Ismail Mare
 *  ID: 1388973
 *  LECTURE SECTION: B1
 *  UNIX ID: imare
 *  INSTRUCTOR: Smith Jaqueline
 *  LAB SECTION: H02
 *  TA'S NAME: Nicholas Barriga
 */

# include "draw_landscape.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <math.h>

# include <ctype.h>
# include <math.h>
# include <assert.h>

int draw_landscape(FILE* input, FILE* executable, FILE* debug_log, double* delta_x, double* delta_y,int land_coor ) {
    
    int mem_space_double = IMAGE_LEN;
    double tmp_x=0,tmp_y=0;
    int k=0;
    char line1[LINE_LEN+1]; // used to store linemem_space = IMAGE_LEN;
    
//    double angle_degrees = 0, angle_radians = 0, trans_x = 0, trans_y = 0;
    long x1_r = 0, y1_r = 0, x2_r = 0, y2_r = 0;

 
    while (fgets(line1, LINE_LEN, input) !=NULL) {
        
        
        if (sscanf(line1, "%lf%lf",&tmp_x,&tmp_y)){
        
       	    delta_x[land_coor] = tmp_x;
            delta_y[land_coor] = tmp_y;
            land_coor++;
            if(land_coor == mem_space_double){
            
                mem_space_double = mem_space_double*2;
            
                double *tmp2;
                double *tmp3;
            
            
                // Check and realloc mem_space_double to make
                // space for more coordinates
            
                // Error handling in case realloc returns null
                if ((tmp2 = realloc(delta_x,
                                sizeof(double) *
                                (mem_space_double))) == NULL) {
                
                    fprintf(stderr, "lander: error: realloc failed\n");
                        
                    exit(EXIT_FAILURE);
                }
            
                // Error handling in case realloc returns null
                if ((tmp3 = realloc(delta_y,
                                sizeof(double) *
                                (mem_space_double))) == NULL) {

                    fprintf(stderr, "lander: error: realloc failed\n");
                
                    exit(EXIT_FAILURE);
                }
            
            
                delta_x = tmp2;
                delta_y = tmp3;
            }
    
    	} 
	
	else {
		fprintf(stderr, "lander: error: incorrect value in inputfile\n");   
	}

    } // End of while

    //CHANGE TO LROUND
    for(k = 1 ;k < land_coor; k++) {
        
        x1_r = lround(delta_x[k-1]);
        y1_r = lround(delta_y[k-1]);
        x2_r = lround(delta_x[k]);
        y2_r = lround(delta_y[k]);
        
        // send
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n"
                , x1_r, y1_r, x2_r, y2_r );
    }
    return land_coor;    
    
} // End of draw
