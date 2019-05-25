#ifndef EGI_H
#define EGI_H

#define VIDEO_MODE_MONOCHROME	0x11
#define VIDEO_MODE_VGA_256 	0x13
#define VIDEO_MODE_TEXT		0x03

#define EGI_VERSION		1.3

/*Function prototypes*/
extern void * malloc(size_t _size);
extern void * realloc(void * ptr,size_t _size);

void set_video(char mode);
char get_video(void);
void write_pixel(int x,int y,char color);
char egi_open_file(const char* filename,FILE *stream,int preset_coord_x,int preset_coord_y);

/*Used in mallocs and frees, to not take
 *uneeded memory space, remember that this
 *is MS-DOS!*/
static unsigned char *mem_hand;
static unsigned char e_signature;
static int e_width;
static int e_height;
static int pixel_count;
static int pixel_track;
static int coord_x;
static int coord_y;
union REGS regs;

#if defined (__MSDOS__) || defined (_MSDOS) || defined (MS_DOS) 
/*Set the desired video mode*/
void set_video(char mode)
{
	regs.h.ah = 0x00;
	regs.h.al = mode;
	int86(0x10,&regs,&regs);
	return;
}
/*Get video mode*/
char get_video(void)
{
	regs.h.ah = 0x0F;
	int86(0x10,&regs,&regs);
	return regs.h.al;
}
/*Write a pixel*/
void write_pixel(int x,int y,char color)
{
	regs.h.ah = 0x0C;
	regs.x.cx = x;
	regs.x.dx = y;
	regs.h.al = color;
	int86(0x10, &regs, &regs);
	return;
}
#elif defined (__APPLE2__)
void set_video(void)
{
  	printf("Your system is not supported");
  	return;
}
void get_video(void)
{
  	printf("Your system is not supported");
  	return;
}
void write_pixel(void)
{
  	printf("Your system is not supported");
  	return;
}
#endif

char egi_open_file(const char* filename,FILE *stream,int preset_coord_x,int preset_coord_y)
{
	stream = fopen(filename,"rb");
	if(!stream)
	{
		printf("Error: File not found");
		return 0;
	}
	mem_hand = malloc(3);
	fread(mem_hand,1,3,stream);
	e_signature = mem_hand[0];
	if(mem_hand[1]+1 == mem_hand[2]+1)
	{
		pixel_count = ((mem_hand[1]+1)<<4);
	}
	else
	{
		pixel_count = (mem_hand[1]+1)*(mem_hand[2]+1);
	}
	e_width = mem_hand[1];
	e_height = mem_hand[2];
	if(pixel_count < 1)
	{
		printf("Null Dimensions");
		return -1;
	}
	mem_hand = realloc(mem_hand,mem_hand[1]*mem_hand[2]);
	fread(mem_hand,1,pixel_count,stream);
	coord_x = 0; coord_y = 0;
	for(pixel_track = 0; pixel_track < (pixel_count+1); pixel_track++)
	{
		write_pixel(coord_x+preset_coord_x,coord_y+preset_coord_y,mem_hand[pixel_track]);
		coord_x++;
		if(coord_x > e_width)
		{
			coord_x = 0;
			coord_y++;
		}
	}
	free(mem_hand);
	fclose(stream);
	return 0;
}

#endif
