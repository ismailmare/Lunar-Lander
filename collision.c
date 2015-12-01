/* Name: Ismail Mare
 *  ID: 1388973
 *  LECTURE SECTION: B1
 *  UNIX ID: imare
 *  INSTRUCTOR: Smith Jaqueline
 *  LAB SECTION: H02
 *  TA'S NAME: Nicholas Barriga
*/

#include "collision.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <math.h>
# include "main.h"
# include <ctype.h>
# include <math.h>
# include <assert.h>

// will feed the other below function all the lines in the ship and 
// and the lines in the landscape
int line_intersection(double* ship_coor_x, double* ship_coor_y, double* delta_x, double *delta_y, int num_coor, int land_coor, FILE* debug_log) {
    
    int k=1;
    int i=1;
    int result=0;       
    for (k=1; k<num_coor; k++) {
        
        for (i=1; i<land_coor; i++) {
            
            result = lineSegmentIntersection(ship_coor_x[k-1], ship_coor_y[k-1], 
		ship_coor_x[k], ship_coor_y[k], delta_x[i-1], delta_y[i-1], 
		delta_x[i], delta_y[i]);
	    if( result == 1){
		return result;
	    }
	}
    }
    return 0;
    
}


int lineSegmentIntersection(
    double Ax, double Ay,
    double Bx, double By,
    double Cx, double Cy,
    double Dx, double Dy) {

  double  distAB, theCos, theSin, newX, ABpos ;
  double X[1];
  double Y[1];
  //  Fail if either line segment is zero-length.

  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) {
	return 0;
  }

  //  Fail if the segments share an end-point.
  if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)
  || ( Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) {
    return 0; 
  }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)) {
	return 0;
  }

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if ((ABpos<0.) || (ABpos>distAB)) {
	return 0;
  }

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;

  //  Success.
  return 1; 
} 
        
        
