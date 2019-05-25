#ifndef DEF_H
#define DEF_H

FILE *fp;

/*Used along with keys*/
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_ENTER 13
#define MAX_NUM_CHEMICALS 64;
char NUM_REACTIONS = 28;
char NUM_CHEMICALS = 25;

extern size_t strlen(const char * _s);
extern void * memccpy(void * __dest, const void * __src, size_t __n);
extern int strcmp(const char *__s1, const char *__s2);
char check_for(char __check);
char parse_chemical_json(const char* __json_chemical_filename);
char parse_reaction_json(const char* __json_chemical_filename);
void set_text_color_fullscreen(char __color);
void set_text_color_8(char __color);
void print_line_of(char __c1,char __c2,char __c3);
unsigned char pgetch(void);

typedef struct json_chemical_file
{
	char chem_name[255];
	short melting_point;
	short boiling_point;
	char state_name[255];
	short state_id;
	short mass;
	char id;
}json_chemical_file;

typedef struct json_react_file
{
	char react_name[255];
	char reactants[255];
	char result[255];
	short max_temp;
	short min_temp;
	char type[255];
	char string_hold[255];
}json_react_file;

struct s_cursor_pos
{
	char x;
	char y;
};

/*After this goes the structures*/
struct json_chemical_file holder;
struct json_react_file holder2;
struct s_cursor_pos cursor_pos;

/*Used for calculation and formulas*/
char menu_select = 0;
short u,k,mj,gh_check,n;

/*In game data*/
short igd_data_temp;

/*Data (around 4 kb)*/
char jcfs_data_chem_name[32][255];
short jcfs_data_melting_point[32];
short jcfs_data_boiling_point[32];
char jcfs_data_state_name[32];
short jcfs_data_mass[32];

/*React Data (other 4kb!)*/
char jrfs_data_react_name[32][255];
char jrfs_data_reactants[32][32];
char jrfs_data_result[32][32];
short jrfs_data_max_temp[32];
short jrfs_data_min_temp[32];
char jrfs_data_reactants_num[32];
char holder__3[255];

char page_number;

/*Get the total amount of chemicals per ID*/
short jcfs_use_amount[32];

#endif
