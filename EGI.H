#ifndef EGI_H
#define EGI_H
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

#endif
