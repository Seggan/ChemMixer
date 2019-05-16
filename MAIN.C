#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#ifndef VIDEO_INT
	#define VIDEO_INT 0x10
#elif
	#undef VIDEO_INT
	#define VIDEO_INT 0x10
#endif
#ifndef WRITE_DOT
	#define WRITE_DOT 0x0C
#elif
	#undef WRITE_DOT
	#define WRITE_DOT 0x0C
#endif
#ifndef SCREEN_WIDTH
	#define SCREEN_WIDTH 320
#elif
	#undef SCREEN_WIDTH
	#define SCREEN_WIDTH 320
#endif
#ifndef SCREEN_HEIGHT
	#define SCREEN_HEIGHT 200
#elif
	#undef SCREEN_HEIGHT
	#define SCREEN_HEIGHT 200
#endif
#ifndef NUM_COLORS
	#define NUM_COLORS 256
#elif
	#undef NUM_COLORS
	#define NUM_COLORS 256
#endif

/*
EGI Library

Reads and parses EGI files
Functions:

test_draw(void)
Draws a test screen with a complete palette of all VGA colors

read_image_file(const char* egi_file,int ppos_x,int ppos_y)
Parses and then draws a specified image, then ppos_x and ppos_y
allows image to be placed anywhere you want (starting at corner 0x0).
*/

/*Is video int already declared?, then undefine it and
 * put our own to avoid conflicts*/

/*File data type*/
FILE *fps;
FILE *fp;
FILE *m_file;
/*Typedefs*/
typedef const char* str;
typedef char byte;
typedef short word;
typedef long dword;
typedef long long qword;
/*Based on VGA basics programing and in ASM tutorials*/
byte *VGA=(byte *)0xA0000000L;
word *CPU_CLOCK=(word *)0x0000046C;
static unsigned char f_read_pixel[2],f_read_header[1],read_header[2];
static int g,i,m,j;
static char read_pixel;
/*Set the 256 VGA color mode*/
void set_video_to_graphic(void)
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x13;
	int86(0x10,&regs,&regs);
}
/*Set video back to text*/
void set_video_to_text(void)
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x03;
	int86(0x10,&regs,&regs);
}
/*Write a pixel*/
void write_pixel(int x,int y,char color)
{
	union REGS regs;
	regs.h.ah = 0x0C;
	regs.x.cx = x;
	regs.x.dx = y;
	regs.h.al = color;
	int86(0x10, &regs, &regs);
}
/*Test that graphic mode
 * is already set*/
void test_draw(void)
{
	set_video_to_graphic();
	for(i=0;i<SCREEN_HEIGHT;i++)
	{
		for(j=0;j<SCREEN_WIDTH;j++)
		{
			write_pixel(j,i,m);
		}
		m++;
	}
}
/*Used for even more bakcward compatibility
 * and is suggested to be used on small programs
 * or test programs, since this one is volatile,
 * unescesarily big and can cause stream errors*/
void read_image_file(const char* egi_file,unsigned char ppos_x,unsigned char ppos_y)
{
	fps = fopen(egi_file,"rb");
	if(!fps)
	{
		printf("File not found, Cant find file %s",egi_file);
		return;
	}
	fread(&f_read_header,1,2,fps);
	for(g = 0;g < f_read_header[0]*f_read_header[1];g++)
    {
		fread(&f_read_pixel,1,3,fps);
		write_pixel(f_read_pixel[0]+ppos_x,f_read_pixel[1]+ppos_y,f_read_pixel[2]);
    }
	fclose(fps);
}

/*Used for backwards compatibility and small
 * exceptions to the memcpy*/
void read_image_file_new(const char* egi_file,unsigned int ppos_x,unsigned int ppos_y)
{
	fps = fopen(egi_file,"rb");
	if(!fps)
	{
		printf("File not found, Cant find file %s",egi_file);
		return;
	}
	fread(&read_header,1,3,fps);
	if(read_header[0] != 33)
	{
		read_image_file(egi_file,ppos_x,ppos_y);
		return;
	}
	for(g = 0;g < read_header[1]+1;g++)
    {
		for(i = 0;i < read_header[2]+1;i++)
		{
			fread(&read_pixel,1,1,fps);
			if(read_pixel > 1)
			{
				write_pixel(i+ppos_x,g+ppos_y,read_pixel);
			}
		}
	}
	fclose(fps);
}

/*End of EGI library*/

/*Used along with keys*/
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_ENTER 13
#define MAX_NUM_CHEMICALS 64;
char NUM_CHEMICALS = 25;

char menu_select = 0;
char d = 0;

struct json_chemical_file
{
	char chem_name[255];
	short melting_point;
	short boiling_point;
	char state_name[255];
	short state_id;
	short mass;
	char id;
};

/*Data (around 4 kb)*/
char jcfs_data_chem_name[32][255];
short jcfs_data_melting_point[32];
short jcfs_data_boiling_point[32];
char jcfs_data_state_name[32][255];
short jcfs_data_mass[32];

/*Get the total amount of chemicals per ID*/
short jcfs_use_amount[32];

/*After this goes the structures*/

struct json_chemical_file holder;

char check_for(char check)
{
	int o;
	char chars;
	for(;;)
	{
		chars = fgetc(fp);
		/*The FGETS hit the nail!, lets tell it to our master function!*/
		if(chars == check)
		{
			return 1;
		}
		/*End of file? Well, we found nothing but ashes!*/
		if(!chars||chars == EOF||chars == 0)
		{
			return 0;
		}
	}
}

char parse_chemical_json(const char* json_chemical_filename)
{
	int ch_piec;
	int o;
	int t = 0;
	fp = fopen(json_chemical_filename,"r");
	if(!fp)
	{
		printf("Cant open file %s!",json_chemical_filename);
		/*Return Error Code 0*/
		return 0;
	}
	for(t = 0; t < NUM_CHEMICALS; t++)
	{
		/*Seek for the chemical name*/
		ch_piec = check_for('"');
		if(ch_piec == 1)
		{
			for(o = 0;o < 255;o++)
			{
				if(ch_piec == 1)
				{
					holder.chem_name[o] = fgetc(fp);
					if(holder.chem_name[o] == '"')
					{
						/*Set ch_piec to 0, so we dont write anything else*/
						/*Plus remove the " from it, so its clean*/
						holder.chem_name[o] = 0;
						ch_piec = 0;
						break;
					}
				}
			}
		}
		/*Be sure to skip the : character!*/
		ch_piec = check_for(':');
		/*Seek for the melting point
		*Lets see where the number starts!*/
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder.melting_point);
		}
		/*Now lets seek out for the boiler points!
		*This wasn't accidentaly, i like to mess
		*up names :P
		*Lets see where the number starts!*/
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder.boiling_point);
		}
		/*Finaly, lets seek out the state*/
		ch_piec = check_for(':');
		ch_piec = check_for('"');
		if(ch_piec == 1)
		{
			for(o = 0;o < 255;o++)
			{
				if(ch_piec == 1)
				{
					holder.state_name[o] = fgetc(fp);
					if(holder.state_name[o] == '"')
					{
						/*Set ch_piec to 0, so we dont write anything else*/
						/*Plus remove the " from it, so its clean*/
						holder.state_name[o] = 0;
						/*Let's set the state ID*/
						if(holder.state_name[0] == 'g')
						{
							holder.state_id = 1;
						}
						if(holder.state_name[0] == 's')
						{
							holder.state_id = 2;
						}
						if(holder.state_name[0] == 'l')
						{
							holder.state_id = 3;
						}
						ch_piec = 0;
						break;
					}
				}
			}
		}
		/*Finaly for the mass*/
		ch_piec = check_for('"');
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder.mass);
		}
		/*Copy the contents into the data structure*/
		memccpy(jcfs_data_chem_name[t],holder.chem_name,holder.chem_name,strlen(holder.chem_name)+1);
		jcfs_data_melting_point[t] = holder.melting_point;
		jcfs_data_boiling_point[t] = holder.boiling_point;
		memccpy(jcfs_data_state_name[t],holder.state_name,holder.state_name,strlen(holder.state_name)+1);
		jcfs_data_mass[t] = holder.mass;
	}
	return 1;
}

void set_text_color_fullscreen(char color)
{
	union REGS regs;
	regs.h.ah = 0x09;
	regs.h.bl = color;
	regs.h.al = 0x20;
	regs.x.cx = 0x1000;
	int86(0x10, &regs, &regs);
}

void print_line_of(char c1,char c2,char c3)
{
	int o;
	printf("%c",c1);
	for(o = 0; o < 78;o++)
	{
		printf("%c",c2);
	}
	printf("%c",c3);
	return;
}

struct s_cursor_pos
{
	char x;
	char y;
};

struct s_cursor_pos cursor_pos;

int main()
{
	char select_mode = 0;
	char y,x;
	int o;
	char selection = 0;
	/*Now make a list of the chemicals and also
	*copy the data into the structures*/
	parse_chemical_json("CHEMICAL.JSO");
	/*After this point we dont need to bother
	 * about opening files because everything
	 * of our data relies on the memory
	 * and is allocated within the 64 KB
	 * memory*/
	cursor_pos.x = 0;
	cursor_pos.y = 0;
	for(;;)
	{
		set_video_to_text();
		/*1st to 4th line*/
		set_text_color_fullscreen(0x1F);
		gotoxy(0,0);
		print_line_of('\xC9','\xCD','\xBB');
		printf("\xBA Chemixer 3.0");
		gotoxy(79,2);
		printf(" \xBA");
		print_line_of('\xBA',' ','\xBA');
		if(select_mode == 0)
		{
			printf("\xBA (A)dd chemical (R)emove chemical Change (T)emperature (C)redits (E)xit");
		}
		if(select_mode == 1)
		{
			printf("\xBA (A)dd (E)xit");
			if(menu_select == KEY_UP)
			{
				cursor_pos.y--;
			}
			if(menu_select == KEY_DOWN)
			{
				cursor_pos.y++;
			}
			if(menu_select == KEY_LEFT)
			{
				cursor_pos.x--;
			}
			if(menu_select == KEY_RIGHT)
			{
				cursor_pos.x++;
			}
		}
		if(select_mode == 2)
		{
			printf("\xBA (E)xit");
		}
		if(cursor_pos.x > 3)
		{
			cursor_pos.x = 3;
		}
		if(cursor_pos.x < 0)
		{
			cursor_pos.x = 0;
		}
		if(cursor_pos.y < 0)
		{
			cursor_pos.y = 0;
		}
		gotoxy(79,4);
		printf(" \xBA");
		/*5th line*/
		print_line_of('\xCC','\xCD','\xB9');
		/*6th Line*/
		gotoxy(0,6);
		printf("\xBA Name");
		gotoxy(80,6);
		printf("\xBA");
		gotoxy(0,7);
		/*7th Line*/
		if(select_mode == 1||select_mode == 0)
		{
			print_line_of('\xCC','\xCD','\xB9');
			gotoxy(20,7);
			printf("\xCB");
			gotoxy(40,7);
			printf("\xCB");
			gotoxy(60,7);
			printf("\xCB");
			gotoxy(80,7);
			printf("\xB9");
		}
		if(select_mode == 2)
		{
			print_line_of('\xCC','\xCD','\xB9');
		}
		/*8th and upper lines*/
		gotoxy(0,8);
		/*
		getch();
		set_video_to_graphic();
		printf("Chemical 1: XYZ\nComposition: XYZ\nState of Matter: XYZ\nPercentage in beaker by mass: XYZ\n");
		read_image_file_new("BALL.EGI",64,64);
		getch();
		*/
		if(select_mode == 0)
		{
			for(y = 0; y < NUM_CHEMICALS; y += 3)
			{
				gotoxy(0,8+(y/4));
				printf("\xBA (%d) ",jcfs_use_amount[y]);
				printf("%s",jcfs_data_chem_name[y]);
				gotoxy(20,8+(y/4));
				printf("\xBA (%d) ",jcfs_use_amount[y+1]);
				printf("%s",jcfs_data_chem_name[y+1]);
				gotoxy(40,8+(y/4));
				printf("\xBA (%d) ",jcfs_use_amount[y+2]);
				printf("%s",jcfs_data_chem_name[y+2]);
				gotoxy(60,8+(y/4));
				printf("\xBA (%d) ",jcfs_use_amount[y+3]);
				printf("%s",jcfs_data_chem_name[y+3]);
				gotoxy(79,8+(y/4));
				printf(" \xBA");
			}
		}
		if(select_mode == 1)
		{
			for(y = 0; y < NUM_CHEMICALS; y += 3)
			{
				gotoxy(0,8+(y/4));
				printf("\xBA %s",jcfs_data_chem_name[y]);
				gotoxy(20,8+(y/4));
				printf("\xBA %s",jcfs_data_chem_name[y+1]);
				gotoxy(40,8+(y/4));
				printf("\xBA %s",jcfs_data_chem_name[y+2]);
				gotoxy(60,8+(y/4));
				printf("\xBA %s",jcfs_data_chem_name[y+3]);
				gotoxy(79,8+(y/4));
				printf(" \xBA");
			}
			if(cursor_pos.x <= 0)
			{
				gotoxy(cursor_pos.x*20+3,cursor_pos.y+8);
			}
			else
			{
				gotoxy(cursor_pos.x*20+2,cursor_pos.y+8);
			}
			printf("\xDB");
			gotoxy(3,25);
			printf("%d %d",cursor_pos.x,cursor_pos.y);
		}
		if(select_mode == 2)
		{
			gotoxy(0,8);
			printf("\xBA Ported to DOS by: Jesus Antonio Diaz (JesusAnt5)");
			gotoxy(79,8);
			printf(" \xBA");
			gotoxy(0,9);
			printf("\xBA Original Game: Seggan");
			gotoxy(79,9);
			printf(" \xBA");
		}
		menu_select = getch();
		/*Select Mode*/
		if(select_mode == 0)
		{
			if(menu_select == 'A'||menu_select == 'a')
			{
				select_mode = 1;
			}
			/*Credits*/
			if(menu_select == 'C'||menu_select == 'c')
			{
				select_mode = 2;
			}
			/*Exit*/
			if(menu_select == 'E'||menu_select == 'e')
			{
				set_video_to_text();
				return 0;
			}
		}
		if(select_mode == 1)
		{
			/*Exit*/
			if(menu_select == 'E'||menu_select == 'e')
			{
				select_mode = 0;
			}
			/*Arrow keys*/
			if(d < 0)
			{
				d = 0;
			}
			if(d > NUM_CHEMICALS)
			{
				d = NUM_CHEMICALS;
			}
		}
		if(select_mode == 2)
		{
			/*Exit*/
			if(menu_select == 'E'||menu_select == 'e')
			{
				select_mode = 0;
			}
		}
	}
}
