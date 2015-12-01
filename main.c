/* Name: Ismail Mare
 *  ID: 1388973
 *  LECTURE SECTION: B1
 *  UNIX ID: imare
 *  INSTRUCTOR: Smith Jaqueline
 *  LAB SECTION: H02
 *  TA'S NAME: Nicholas Barriga
*/

# define _POSIX_C_SOURCE 200112L
# define KEY_SPACE ' '


# include <unistd.h>
# include <sys/time.h>
# include <curses.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <math.h>
# include "main.h"
# include <ctype.h>
# include <assert.h>
# include <stdio.h>

sigset_t block_mask_g;



// Global Variables
FILE* executable; // to sketchpad
double gravity; // read from command line
//double ship_coor_x[5] = {310, 320, 280, 290, 310}; // holds the x coordinates of ship
//double ship_coor_y[5] = {100, 130, 130, 100, 100}; // holds the y coordinates of ship
double *ship_coor_x; //[8] = {310, 315, 295, 300, 310, 315, (315+295)/2, 295}; 
double *ship_coor_y; //[8] = {100, 120, 120, 100, 100, 120, 125, 120};
double thrust_x[3] = {315, (315+295)/2, 295};
double thrust_y[3] = {120, 125, 120};
extern int enviro_num;
int num_coor; // the num of coordinates in ship_coor
int thrust_coor;
int bounds_check = 0; // set to hold if line_segment is true or not
int enviro_coor = 0;
int land_coor=0; // used to hold the num of coordinates in delta_x, delta_y
FILE* input; // Holds inputfile
FILE* debug_output; // Holds debugging file that i write to
char* input_name; // inputfile name that i print if there is an error
int k = 0; // used in for loops
double thrust; // read from command line
double* delta_x; // holds landscape coordinates
double* delta_y; // holds landscape coordinates
double angle_radian = 0; // is used for rotate_ship function
double angle = 90; // remembers the orrientation of the ship
int improvements = 0; // set to 1 if there a -i in the command line




// our function which we will have called whenever the timer expires
int main(int argc, char * argv[]) {  
    //char line[LINE_LEN+1];   
    int exit_code=0; // if one the program will exit. Set to 1 if 'q' is read 
    const char Exec_name[] = "java -jar Sketchpad.jar";
    // print the date, program name and the inputfile name
    printf("%s started on ", argv[0]);
    fflush(stdout);
    system("date\n");

    // Reading from commmand line
 
/*   for(k=1; k < argc; k++) {

        if (strncmp(argv[k], "-g", 2)==0) {
            gravity = atoi(argv[k+1]);
        }
    
        else if (strncmp(argv[k], "-t", 2)==0) {
            thrust = atoi(argv[k+1]);
        }
     
        else if (strncmp(argv[k], "-f", 2)==0){
            input = fopen(argv[k+1], "r");
  	    input_name = argv[k+1];
        }
	else if (strncmp(argv[k], "-i", 2)==0){
	    improvements=1;
	}
*/
    
    //Perfect command line argument checking  
    int r;
    while ((r = getopt (argc, argv, "f:ig:t:")) != -1) {
        
        switch (r) {

            case 'f':
                input = fopen(optarg, "r");
                break;
            case 'i':
                improvements = 1;
                break;
            case 'g':
                gravity = atoi(optarg);
                break;
            case 't':
                thrust = atoi(optarg);
		break;
            case'?':
                printf("Command line argument not recognized\n");
                exit(EXIT_FAILURE);
        }
    }

    // More error handle
    if ((gravity > 20) || (gravity < 0)) { 
	
	printf("gravity < 0, > 20 not allowed\n");
	exit_code = 1;
    }
    if ((thrust > 0)|| (thrust < -20)) {
	
	printf("thrust > 0, < -20 not allowed\n");
	exit_code = 1;
    }
    if (input == NULL) {
	printf("could not open %s\n", input_name);
	exit_code = 1;
    }

    // Error handle
    if(exit_code==1) {
	fprintf(executable, " end\n");
        exit(EXIT_SUCCESS);
    }
	
    if(improvements == 1){
	double y[13] = {100, 107.5, 115, 120, 120, 125, 125, 120,
                                120, 115, 107.5, 100, 100};
	double x[13] = {310, 315, 325, 325, 315, 315, 295, 295, 285,
                                285, 295, 300, 310};
	ship_coor_x = malloc(13*sizeof(double));
	ship_coor_y = malloc(13*sizeof(double));
        int i=0;
        num_coor=13;
        // holds the x coordinates of ship
        // holds the y coordinates of ship
	for(i = 0; i < num_coor; i++) {
	    ship_coor_x[i] = x[i];
	    ship_coor_y[i] = y[i];
	}
    }
    else{
	double x[9] = {310, 315, 295, 300, 310, 300, 295, (295+315)/2, 315};
        double y[9] = {100, 120, 120, 100, 100, 100, 120, 125, 120};
        ship_coor_x = malloc(9*sizeof(double));
        ship_coor_y = malloc(9*sizeof(double));
        int i=0;
        num_coor=5;
	thrust_coor = 9;
        // holds the x coordinates of ship
        // holds the y coordinates of ship
        for(i = 0; i < thrust_coor; i++) {
            ship_coor_x[i] = x[i];
            ship_coor_y[i] = y[i];
        }
	/*
	double y[13] = {310, 315, 295, 300, 310, 315, (315+295)/2, 295};
	double x[13] = {100, 120, 120, 100, 100, 120, 125, 120};
	ship_coor_x = malloc(13*sizeof(double));
	ship_coor_y = malloc(13*sizeof(double));
	num_coor = 13;
	int i = 0;
	for(i = 0; i < num_coor; i++) {
	    ship_coor_x[i] = x[i];
            ship_coor_y[i] = y[i];
	}
	num_coor = 8;
	*/
    }
 
    thrust = thrust/200;
    gravity = gravity/50;	
    debug_output = fopen("debug_log", "w");
    executable = popen(Exec_name, "w");
    //Error Handle 
    if (executable == NULL) {
        printf("could not open pipe: %s\n", Exec_name);
        exit(1);
    }
    // allocate memory to hold landcape coordinates
    delta_x = malloc( IMAGE_LEN * sizeof(double));
    delta_y = malloc( IMAGE_LEN * sizeof(double));
    setup_curses();

    move(5, 10);
    printw("Press any key to start.\n");
    printw("(Then press arrow keys to rotate, space for thrust, 'q' to quit.)\n");
    refresh();
    int c = getch(); 

    nodelay(stdscr, true);
    erase();
	
//    while (fgets(line, LINE_LEN, input) !=NULL) {
//    	land_coor++;
    
    // draw the ship and landscape
    land_coor = draw_landscape(input, executable, debug_output, delta_x, delta_y, enviro_coor);
    draw_ship(executable, debug_output, ship_coor_x, ship_coor_y, num_coor);
    angle_radian = 0.0 * PI/180.0;
    rotate_ship(debug_output, executable, angle_radian, ship_coor_x, ship_coor_y, num_coor);

    move(5, 10);
    printw("Press arrow keys, 'q' to quit.\n");
    printw("Left arrow key rotate counter-clockwise, right clockwise, space for thrust\n");
    refresh();
    if((c == 'l')){
	exit_code++;
    }
    // start timer / gravity
    timer(); 
    
    free(delta_x);
    free(delta_y); 
    free(ship_coor_x);
    free(ship_coor_y);
    unset_curses();

    fprintf(executable, " clearScreen\n");
    fprintf(executable, " end\n");

    fclose(input);
    pclose(executable);
    fclose(debug_output);  
    exit(EXIT_SUCCESS);

}

void setup_curses() {
    // use return values.  see man pages.  likely just useful for error
    // checking (NULL or non-NULL, at least for init_scr)
    initscr();
    cbreak();
    noecho();
    // needed for cursor keys (even though says keypad)
    keypad(stdscr, true);
}

void unset_curses() {
    keypad(stdscr, false);
    nodelay(stdscr, false);
    nocbreak();
    echo();
    endwin();
}



int timer() {
    // just initializes so it's empty (man 3 sigemptyset)
    sigemptyset(&block_mask_g);
    // block_mask_g will now indicate to signal functions that something
    // (i.e., blocking) must be to SIGALRM signal
    sigaddset(&block_mask_g, SIGALRM);

    // set up signal handler (man 2 sigaction)

    // will store information for our alarm
    struct sigaction handler;

    // handle_timeout is function to call when signal (in our case an
    // alarm) is sent
    handler.sa_handler = handle_timeout;
    // empties sa_mask, meaning no signals will be blocked when our
    // signal_handler (handle_timeout) is executing
    sigemptyset(&handler.sa_mask);
    // if non-zero, indicates various options ... we don't want to use
    // them
    handler.sa_flags = 0;

    // sets up a signal handler as specified in handler struct.
    // when SIGALRM occurs (set up below), handle_timeout will now be
    // called.
    if (sigaction(SIGALRM, &handler, NULL) == -1)
        exit(EXIT_FAILURE);

    // set up timer
    /* *********************************************************** */

    // sets up values for timer (intervals between sending SIGALRM).
    // times in microseconds (1/1 millionth of a second).
    // note although close enough for us, this is only approximate.
    // (man 2 setitimer)
    struct itimerval timer;

    // value for time between sending SIGALRM
    // The sample program set tv_usec to be 50000 (and tv_sec to be
    // zero), which meant the ship was redrawn approximately every
    // 50000 microseconds
    struct timeval time_delay;
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000;

    timer.it_interval = time_delay;

    // value for initial time (before 1st alarm is triggered)
    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000; 
  
    timer.it_value = start;

    // will execute repeatedly, sending SIGALRM every timer.it_interval
    // microseconds, who initial value is timer.it_value.
    // should handle error better (and should use a macro).  should use
    // errno.  note it may save you some hassle if you do this, too
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
        exit(EXIT_FAILURE); 
    
    // infinite loop
    for ( ; ; )
        ;

    // should never execute
    exit(EXIT_SUCCESS);
}


// called when SIGALRM is sent.
// could be called for other signals, so signal parameter indicates
// which one.
void handle_timeout(int signal)
{
    int bound_check = 0;
    // called because of SIGALRM
    if (signal == SIGALRM)
    {

        bound_check = block_signal(); //executable, ship_coor_x, ship_coor_y, num_coor, gravity);

        //--------------------------------------------------------------
        // gets timer, puts it into timer (man 2 getitimer)
        struct itimerval timer;
        if (getitimer(ITIMER_REAL, &timer) == -1)
            exit(EXIT_FAILURE);

        // stops timer when the
        // game is done (e.g., ship has crashed or landed)
        if (bound_check == 1)
        {
	    free(delta_x);
    	    free(delta_y);
	    free(ship_coor_x);
	    free(ship_coor_y);
    	    unset_curses();
    	    fprintf(executable, " clearScreen\n");
    	    fprintf(executable, " end\n");

    	    fclose(input);
    	    pclose(executable);
            fclose(debug_output);
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_usec = 0;
            exit(EXIT_SUCCESS);
        }

        if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
            exit(EXIT_FAILURE);
    }

}

// critical code is code that must not be interrupted.  In your asn3,
// examples will include code that updates global data structures, and
// code that erases an old ship and draws a new ship (i.e., moves the
// ship).
//
// The only way code can be interrupted in asn3 code is via our timer
// (i.e., by SIGALRM).
//
// We protect critical code by blocking the signal.
//
// The following is just an example of signal blocking; there is not
// critical code to block.


// WHERE TO RUN THIS FUNCTION IN ABOVE??//



int block_signal() {
//FILE* executable, double* ship_coor_x, double* ship_coor_y, int num_coor, double gravity)

    sigset_t old_mask;
    //--------------------------------------------------------------
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
        exit(EXIT_FAILURE);
    }
    // remember old list of signals to block (none, in our case)
    // set blocked signal set to block_mask_g (man 2 sigprocmask).
    // so now block SIGALRM.
    // CRITICAL CODE GOES HERE ... can call other functions here, and
    // they will not be interrupted
    
    double the_new_y = 0.0;
    double the_old_y = 0.0;
    double the_old_velocity = 0.0;
    static double t = 0.05;
    double the_new_velocity = 0.0;
    static double vert_vel = 0;
    static double horz_vel = 0;
    static int old_thrust = 0;
    int c;
    int k = 0;
    int thrust_check=0;
    int exit_code=0;
    static int vel = 0;
    static int fuel = 0;

   
    c = getch();

    // read key strokes
    if (c != ERR) {
           
        if (c == KEY_LEFT) { 
	   thrust_check = 0;
           angle -= 10.0;
           angle_radian = -10.0 * PI/180.0;
           rotate_ship(debug_output, executable, angle_radian, ship_coor_x, ship_coor_y, thrust_coor);
// 	   rotate_ship(debug_output, executable, angle_radian, thrust_x, thrust_y, 3);
        }

        else if (c == KEY_RIGHT) {
	   thrust_check = 0;
           angle += 10.0;
           angle_radian = 10.0 * PI/180.0;
           rotate_ship(debug_output, executable, angle_radian, ship_coor_x, ship_coor_y, thrust_coor);
//	   rotate_ship(debug_output, executable, angle_radian, thrust_x, thrust_y, 3);
        }

        else if (c == KEY_SPACE) {
	   // if ship is out of view set velocity to 0 to makereturn to screen
	   if (vel == 1 ) {
     		thrust_check = 0;
		
           }
	   // if fuel has run out
           else if(fuel == 1) {
               	thrust_check = 0;
           }
           else {
//		velocity_up(executable, thrust, gravity, angle, ship_coor_x, ship_coor_y, num_coor);
//		velocity_up(executable, thrust, gravity, angle, thrust_x, thrust_y, 3);
       	        thrust_check = 1;
	   }
        }

        else if (c == 'q') {
	    exit_code = 1;
	}
    }
    if( improvements == 1){
	wrap_around_screen_x();
	vel = wrap_around_screen_y();
        fuel = fuel_gauge(thrust_check);
    }
    // checking if ship has crossed line
    bounds_check = line_intersection(ship_coor_x, ship_coor_y, delta_x, 
		   delta_y, num_coor, land_coor, debug_output);
    
    while(angle>360){
	angle-=360;
    }
    while(angle<0){
	angle+=360; 
    }
	
    the_new_y = the_old_y + the_old_velocity * t + 1/2 * gravity * t*t;
    the_new_velocity = the_old_velocity + gravity * t;
    the_old_y = the_new_y;
    the_old_velocity = the_new_velocity;
    t += 0.05;
	
    if(thrust_check == 1){ 
	double xA = 0;
        double yA = 0;
        double angle_radian = 0.0;

        angle_radian = angle * PI/180.0;

        xA = thrust * cos(angle_radian);
        yA = gravity + thrust * sin(angle_radian);

        vert_vel += yA + thrust *sin(angle_radian);
        horz_vel += xA + thrust *cos(angle_radian);;
    }
	
    if (bounds_check==1) {
	exit_code=1;
        fflush(stdout);
	move(5, 10);
	if(angle > 95 || angle<85) {
	    printw("CRASHED!!!\n");
	}
	else if( the_new_velocity+vert_vel > 1.25){
	    printw("CRASHED!!!\n");
	}
	else if(horz_vel>1){
	    printw("CRASHED!!!\n");	
        }
	else{
	    printw("LANDED!!!\n");
	}
	int d = getch();
	
        while(d!= 'q'){
	    d=getch();
	}
        return exit_code;
    }
    refresh();
	
    // simulate gravity
    
    fprintf(debug_output,"NewV: %lf, Vert_vel: %lf, NewY: %lf, OldY: %lf, t: %lf, gravity: %lf\n", the_new_velocity, vert_vel, the_new_y, the_old_y, t, gravity);
 

    if(old_thrust == 1) {
	eraseSegment(executable, ship_coor_x, ship_coor_y, thrust_coor);
    }
    else {
        eraseSegment(executable, ship_coor_x, ship_coor_y, num_coor);
    }

    for (k = 0; k < thrust_coor; k++) {
        ship_coor_y[k] += the_new_velocity + vert_vel;
        ship_coor_x[k] += (horz_vel-(horz_vel/1.25)); 
    }
    
    if(thrust_check == 1){
        drawSegment(ship_coor_x, ship_coor_y, thrust_coor);
    }
    else{
        drawSegment(ship_coor_x, ship_coor_y, num_coor);
    } 
    fflush(executable);

    old_thrust = thrust_check;
    // unblock signal by setting mask to old value (which in our
    
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
        exit(EXIT_FAILURE);    
	
	
    return(exit_code);
}

int wrap_around_screen_x() {
	
    int k = 0 ;
	
    for( k = 0; k < num_coor; k++) {
	if(ship_coor_x[k]<0) {
	    continue;
	} 
	else if(ship_coor_x[k]>640) {
		if(k+1 == num_coor) {
		    for( k = 0; k < num_coor; k++) {
		        ship_coor_x[k] -= 640;
		    }
		    return 1;
   		 }
  	    }
	else{
            return 0;
	    
	}
    }
	
    for( k = 0; k < num_coor; k++) {
	ship_coor_x[k] += 640;
    }
    return 1;
}

int wrap_around_screen_y() {
	
    int k = 0;

    for( k = 0; k < num_coor; k++) {
	if( ship_coor_y[k] < 0 ) {
	    continue;
	} 
	else {
            return 0;
	}
    }
	
    return 1;	
}

int fuel_gauge(int thrust){	
    int k = 0;
    long x1_r, x2_r, y1_r, y2_r;
    static double fuel_level_x[2] = {110,110};
    static double fuel_level_y[2] = {10,40};

    double gauge_x[5] = {10, 110, 110, 10, 10};
    double gauge_y[5] = {10, 10, 40, 40, 10};
    int num_gauge = 5;

    for( k = 1; k < num_gauge; k++) {
    	x1_r = lround(gauge_x[k-1]);
	y1_r = lround(gauge_y[k-1]);
	x2_r = lround(gauge_x[k]);
	y2_r = lround(gauge_y[k]);
        
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
		x1_r, y1_r, x2_r, y2_r); 
    }

    fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
		lround(fuel_level_x[0]), lround(fuel_level_y[0])
	        ,lround(fuel_level_x[1]), lround(fuel_level_y[1]));
    if(thrust == 1){   
    	fuel_level_x[0] = fuel_level_x[0]-0.5;
    	fuel_level_x[1] = fuel_level_x[1]-0.5;
    }
    if( fuel_level_x[0] < 10) {
	return 1;
    }
    return 0;
}

void drawSegment(double* ship_coor_x, double* ship_coor_y, int num_coor) {
    long x1_r, x2_r, y1_r, y2_r;

    for(k = 1 ; k < num_coor; k++) {

        x1_r = lround(ship_coor_x[k-1]);
        y1_r = lround(ship_coor_y[k-1]);
        x2_r = lround(ship_coor_x[k]);
        y2_r = lround(ship_coor_y[k]);

        // send
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n"
                ,x1_r, y1_r, x2_r, y2_r);
    }
}
