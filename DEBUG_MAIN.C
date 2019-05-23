#include <stdio.h>
#include <stdlib.h>
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

char menu_select = 0;
char d = 0;
char NUM_CHEMICALS = 17;

struct json_chemical_file
{
	char chem_name[255];
	int melting_point;
	int boiling_point;
	char state_name[255];
	short state_id;
	int mass;
};

struct json_chemical_file holder;
/*50 Structs*/
struct json_chemical_file s_1;
struct json_chemical_file s_2;
struct json_chemical_file s_3;
struct json_chemical_file s_4;
struct json_chemical_file s_5;
struct json_chemical_file s_6;
struct json_chemical_file s_7;
struct json_chemical_file s_8;
struct json_chemical_file s_9;
struct json_chemical_file s_10;
struct json_chemical_file s_11;
struct json_chemical_file s_12;
struct json_chemical_file s_13;
struct json_chemical_file s_14;
struct json_chemical_file s_15;
struct json_chemical_file s_16;
struct json_chemical_file s_17;
struct json_chemical_file s_18;
struct json_chemical_file s_19;
struct json_chemical_file s_20;
struct json_chemical_file s_21;
struct json_chemical_file s_22;
struct json_chemical_file s_23;

void set_text_color(char color,char mode)
{
	union REGS regs;
	/*Entire screen*/
	if(mode == 0x0)
	{
		regs.h.ah = 0x09;
		regs.h.al = 0x20;
		regs.h.bl = color;
	}
	/*Only a line*/
	if(mode == 0x1)
	{
		regs.h.ah = 0x09;
		regs.x.cx = 0x50;
		regs.h.al = 0x20;
		regs.h.bl = color;
	}
	int86(0x10, &regs, &regs);
}

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
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder.mass);
		}
		gotoxy(0,8+t);
		printf("\xBA %s",holder.chem_name);
		gotoxy(30,9+t);
		printf("\xBA %d",holder.melting_point);
		gotoxy(40,10+t);
		printf("\xBA %d",holder.boiling_point);
		gotoxy(50,11+t);
		printf("\xBA %d",holder.mass);
		gotoxy(60,12+t);
		printf("\xBA %s",holder.state_name);
		gotoxy(70,13+t);
		printf("\xBA %d",holder.state_id);
		gotoxy(80,14+t);
		printf("\xBA");
		gotoxy(0,8+t);
		if(d == t)
		{
			gotoxy(0,8+t);
			set_text_color(0x4F,0x1);
		}
		else
		{
			set_text_color(0x1F,0x0);
		}
	}
	return 1;
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

int main()
{
	char select_mode = 0;
	for(;;)
	{
		set_video_to_text();
		set_text_color(0x1F,0x0);
		/*1st to 4th line*/
		gotoxy(0,0);
		print_line_of('\xC9','\xCD','\xBB');
		printf("\xBA Chemixer 3.0 for DOS");
		gotoxy(79,2);
		printf(" \xBA");
		print_line_of('\xCC','\xCD','\xB9');
		if(select_mode != 1)
		{
			printf("\xBA (S)elect (C)redits (E)xit");
		}
		if(select_mode == 1)
		{
			printf("\xBA (E)xit");
		}
		gotoxy(79,4);
		printf(" \xBA");
		/*5th line*/
		print_line_of('\xCC','\xCD','\xB9');
		gotoxy(30,5);
		printf("\xCB");
		gotoxy(40,5);
		printf("\xCB");
		gotoxy(50,5);
		printf("\xCB");
		gotoxy(60,5);
		printf("\xCB");
		gotoxy(70,5);
		printf("\xCB");
		gotoxy(80,5);
		printf("\xB9");
		/*6th Line*/
		gotoxy(0,6);
		printf("\xBA Chemical Name");
		gotoxy(30,6);
		printf("\xBA Melting Point");
		gotoxy(40,6);
		printf("\xBA Boiling Point");
		gotoxy(50,6);
		printf("\xBA Mass");
		gotoxy(60,6);
		printf("\xBA State");
		gotoxy(70,6);
		printf("\xBA ID");
		gotoxy(80,6);
		printf("\xBA");
		gotoxy(0,7);
		/*7th Line*/
		print_line_of('\xCC','\xCD','\xB9');
		gotoxy(30,7);
		printf("\xCE");
		gotoxy(40,7);
		printf("\xCE");
		gotoxy(50,7);
		printf("\xCE");
		gotoxy(60,7);
		printf("\xCE");
		gotoxy(70,7);
		printf("\xCE");
		gotoxy(80,7);
		printf("\xB9");
		/*8th and upper lines*/
		gotoxy(0,8);
		/*Now make a list of the chemicals and also
		*copy the data into the structures*/
		parse_chemical_json("CHEMICAL.JSO");
		/*
		getch();
		set_video_to_graphic();
		printf("Chemical 1: XYZ\nComposition: XYZ\nState of Matter: XYZ\nPercentage in beaker by mass: XYZ\n");
		read_image_file_new("BALL.EGI",64,64);
		getch();
		*/
		menu_select = getch();
		/*Select Mode*/
		if(select_mode != 1)
		{
			if(menu_select == 'S'||menu_select == 's')
			{
				select_mode = 1;
				menu_select = getch();
			}
			/*Credits*/
			if(menu_select == 'C'||menu_select == 'c')
			{
				/*TBD*/
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
			if(menu_select == KEY_UP)
			{
				d--;
			}
			if(menu_select == KEY_DOWN)
			{
				d++;
			}
			if(d < 0)
			{
				d = 0;
			}
			if(d > NUM_CHEMICALS)
			{
				d = NUM_CHEMICALS;
			}
		}
	}
}
