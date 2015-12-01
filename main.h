#ifndef Lunar_Lander_main_h
#define Lunar_Lander_main_h


# define PROG_LEN 128 // Length of program length

# define LINE_LEN 256 // Max length of line

#define PAUSE_LEN 5 // Length of pause instruction

# define PI acos(-1.0)

# define IMAGE_LEN 20

//# define PI 3.1415926535



void handle_timeout(int signal);
int block_signal(); //FILE* executable, double* ship_coor_x, double* ship_coor_y, int num_coor, double gravity);


extern const char Exec_name[];
extern char program_g[];

void *realloc(void *ptr, size_t(size));
int wrap_around_screen_x();
int wrap_around_screen_y();
int fuel_gauge(int thrust);

FILE *popen(const char *line, const char *type);
int pclose(FILE *stream);
long int lround(double x);
void drawSegment(double* ship_coor_x, double* ship_coor_y, int num_coor);
int draw_landscape(FILE* input, FILE* executable, FILE* debug_log, double* delta_x, double* delta_y, int land_coor);
void draw_ship(FILE* executable, FILE* debug_log, double* ship_coor_x, double* ship_coor_y, int num_coor);
void rotate_ship(FILE* debug_log, FILE* executable, double angle, double* ship_coor_x, double* ship_coor_y, int num_coor);
void eraseSegment(FILE* executable, double* ship_coor_x, double*ship_coor_y, int num_coor);
void velocity_up(FILE* executbale, double thrust, double gravity, double angle, double *ship_coor_x, double *ship_coor_y, int num_coor);
int line_intersection(double* ship_coor_x, double* ship_coor_y, double* delta_x, double *delta_y, int num_coor, int land_coor, FILE* debug_log);

void setup_curses();
void unset_curses();
int timer();

#endif

