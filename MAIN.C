/*
*---------------------------------------------------------------*
*---------------------------------------------------------------*
*--- CHEMIXER 0.4.0b - (C) Seggan - Programmed by Jesus Diaz ---*
*--- ------------------------------------------------------- ---*
* This new port to MS DOS provides CheMixer features in a GUI   *
* styled program, with a selection menu, and a built-in JSON    *
* parser.                                                       *
*                                                               *
* If you have suggestions or troubles related to the port of    *
* the MS-DOS port, just contact me in GitHub.                   *
*---------------------------------------------------------------*
*---------------------------------------------------------------*
* This program uses the C89 standart, so be careful what you    *
* implement or modify to this code, take in account, that one   *
* the most important features of the C89 standart is not        *
* allowing variables to be declared after functions.            *
*---------------------------------------------------------------*
*---------------------------------------------------------------*
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <dos.h>

#define TEXT_MODE_SCREEN_WIDTH		80
#define DATA_NUM_CHEMICALS		24
#define DATA_NUM_REACTIONS		27
#define STYLE_COLOR_SELECTED 		0xF0
#define STYLE_COLOR_NOT_SELECTED	0x07
#define MAX_PAGES	9

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long long qword;

extern void gotoxy(int x, int y);

void switch_mode(char __mode);
void print_c(unsigned char __color, const char* __text);
unsigned char locate(unsigned char __char, FILE *stream);
char json_parse_reactions(const char* __filename, FILE *stream);

/*Defined scales*/
unsigned char cur_page = 0;
unsigned char chem_per_page = 18;

/*Data*/
char json_reactions_data_name[27][255];
char json_reactions_data_reacts[27][24];
short *json_reactions_data_temp_max;
short *json_reactions_data_temp_min;
char json_reactions_data_results[27][24];

char json_chemicals_data_name[27][255];
short *json_chemicals_data_temp_max;
short *json_chemicals_data_temp_min;
char *json_chemicals_data_state;
short *json_chemicals_data_mass;

/*Other variables*/
/*This one for general porpouses*/
char *str;
/*We will use this str_ for mallocs*/
char *str_;
short *_nnum;
unsigned char t;
unsigned char g;
unsigned int a;
unsigned int b;
char c;
unsigned char d;

union REGS regs;

void switch_mode(char __mode)
{
	regs.h.ah = 0x00;
	regs.h.al = __mode;
	int86(0x10,&regs,&regs);
}

void print_c(unsigned char __color, const char* __text)
{
	a = 0;
	for(;;)
	{
		if(!(__text[a] != 0))
		{
			return;
		}
		regs.h.ah = 0x0E;
		regs.h.al = __text[a];
		regs.h.bl = __color;
		int86(0x10,&regs, &regs);
		a++;
	}
}

unsigned char locate(unsigned char __char, FILE *stream)
{
	for(;;)
	{
		c = fgetc(stream);
		if(c == __char)
		{
			return 0;
		}
		if(c == EOF||c == 0)
		{
			return 1;
		}
	}
}

char json_parse_chemicals(const char* __filename,FILE *stream)
{
	stream = fopen(__filename,"r");
	if(!stream)
	{
		printf("Couldn't open file");
		return -1;
	}
	json_chemicals_data_temp_max = malloc(DATA_NUM_CHEMICALS);
	json_chemicals_data_temp_min = malloc(DATA_NUM_CHEMICALS);
	json_chemicals_data_state = malloc(DATA_NUM_CHEMICALS);
	json_chemicals_data_mass = malloc(DATA_NUM_CHEMICALS);
	str = malloc(255);
	str_ = malloc(512);
	_nnum = malloc(4);
	for(t = 0; t < DATA_NUM_CHEMICALS; t++)
	{
		locate('"',stream);
		/*Get the chemical name*/
		a = 0;
		for(;;)
		{
			str_[a] = fgetc(stream);
			if(str_[a] == '"')
			{
				str_[a] = 0;
				break;
			}
			if(str_[a] == 0)
			{
				printf("End of file");
				return -1;
			}
			a++;
		}
		printf("Chemical Name >> %s\n",str_);
		for(b = 0; b < 255; b++)
		{
			json_chemicals_data_name[t][b] = str_[b];
		}
		locate(':',stream);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[0]);
		printf("Temp max >> %d\n",_nnum[0]);
		json_chemicals_data_temp_max[t] = _nnum[0];
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[1]);
		printf("Temp min >> %d\n",_nnum[1]);
		json_chemicals_data_temp_min[t] = _nnum[1];
		locate(':',stream);
		locate('"',stream);
		a = 0;
		for(;;)
		{
			str[a] = fgetc(stream);
			if(str[a] == '"')
			{
				str[a] = 0;
				if(strcmp(str,"gaseous") == 0)
				{
					_nnum[3] = 1;
				}
				if(strcmp(str,"solid") == 0)
				{
					_nnum[3] = 2;
				}
				if(strcmp(str,"liquid") == 0)
				{
					_nnum[3] = 3;
				}
				break;
			}
			if(str[a] == 0)
			{
				printf("End of file");
				return -1;
			}
			a++;
		}
		printf("State >> %d\n",_nnum[3]);
		json_chemicals_data_state[t] = _nnum[3];
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[2]);
		printf("Mass >> %d\n",_nnum[2]);
		json_chemicals_data_mass[t] = _nnum[2];
	}
	free(str);
	free(str_);
	free(_nnum);
}

char json_parse_reactions(const char* __filename,FILE *stream)
{
	stream = fopen(__filename,"r");
	if(!stream)
	{
		printf("Couldn't open file");
		return -1;
	}
	/*Get the reaction name*/
	str = malloc(255);
	for(t = 0; t < DATA_NUM_REACTIONS; t++)
	{
		locate('"',stream);
		a = 0;
		for(;;)
		{
			str[a] = fgetc(stream);
			if(str[a] == '"')
			{
				str[a] = 0;
				break;
			}
			if(str[a] == 0)
			{
				printf("End of file");
				return -1;
			}
			a++;
		}
		printf("React Name >> %s\n",str);
		memcpy(json_reactions_data_name[t],str,strlen(str)+1);
		locate('[',stream);
		str_ = malloc(512);
		a = 0;
		for(;;)
		{
			printf(" (%s) \n",str_);
			str_[a] = fgetc(stream);
			if(str_[a] == ']')
			{
				str_[a] = 0;
				break;
			}
			if(str_[a] == '"')
			{
				str_[a] = 0;
				a--;
			}
			if(str_[a] == ',')
			{
				str_[a] = 0;
				for(b = 0; b < DATA_NUM_CHEMICALS; b++)
				{
					g = strcmp(str_,json_chemicals_data_name[b]);
					if(g == 0)
					{
						json_reactions_data_reacts[t][b]++;
					}
				}
				for(b = 0; b < 512; b++)
				{
					str_[b] = 0;
				}
				a = -1;
			}
			a++;
		}
		for(b = 0; b < DATA_NUM_CHEMICALS; b++)
		{
			if(json_reactions_data_reacts[t][b] > 0)
			{
				printf("Reacting with %s\n",json_chemicals_data_name[b]);
			}
		}
		_nnum = malloc(3);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[0]);
		printf("Temp max >> %d\n",_nnum[0]);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[1]);
		printf("Temp min >> %d\n",_nnum[1]);
		locate('[',stream);
		a = 0;
		for(;;)
		{
			str_[a] = fgetc(stream);
			if(str_[a] == ']')
			{
				str_[a] = 0;
				break;
			}
			if(str_[a] == 0)
			{
				printf("End of file");
				return -1;
			}
			if(str_[a] == '"')
			{
				str_[a] = 0;
				a--;
			}
			a++;
		}
		printf("Results >> %s\n",str_);
	}
	free(str);
	free(str_);
	free(_nnum);
}

void print_to_printer(const char* __text)
{
	fprintf(stdprn,__text);
	return;
}

FILE *fp;
const char* string[32] =
{
	"Add Chemical",
	"Remove Chemical",
	"Change Temperature",
	"Beaker Info",
	"Options",
	"Credits",
	"Baker Name:",
	"Temperature:",
	"Chemicals:",
	"Baker's Weight:",
	"Please input the desired temperature:",
	"Select the chemical you want to add:",
	"Select the chemical you want to remove:",
	"Audio",
	"Debug Mode",
	"Chemixer 0.7.3c - \xB8 Seggan - Ported to DOS by Jesus Diaz",
	"SoundBlaster Audio",
	"System Audio",
	"No Audio",
	"On",
	"Off",
	"Mass:",
	"Name:",
	"ID:",
	"Included in:"
};

unsigned char menu_select;
unsigned char y_hover;
unsigned char n;

int main(void)
{
	menu_select = 1;
	y_hover = 1;
	switch_mode(0x03);
	json_parse_chemicals("CHEMICALS.JSO",fp);
	getch();
	json_parse_reactions("REACT.JSO",fp);
	getch();
	textcolor(STYLE_COLOR_SELECTED);
	for(t = 30; t < 255; t++)
	{
		printf("%c HEX %x ",t,t);
	}
	getch();
	t = getch();
	printf("0x%x",t);
	getch();
	switch_mode(0x03);
	for(;;)
	{
		gotoxy(1,1);
		printf("\xC9");
		for(t = 0; t < TEXT_MODE_SCREEN_WIDTH-2; t++)
		{
			printf("\xCD");
		}
		printf("\xBB");
		printf("\xBA           ");
		printf(string[15]);
		printf("           \xBA");
		printf("\xBA ");
		for(t = 0; t < 6; t++)
		{
			printf(" ");
			if(menu_select == t+1 && y_hover == 1)
			{
				textcolor(STYLE_COLOR_SELECTED);
				cprintf(string[t]);
			}
			else
			{
				textcolor(STYLE_COLOR_NOT_SELECTED);
				cprintf(string[t]);
			}
		}
		printf(" \xBA");
		printf("\xCC");
		for(t = 0; t < TEXT_MODE_SCREEN_WIDTH-2; t++)
		{
			printf("\xCD");
		}
		printf("\xB9");
		if(menu_select == 1||menu_select == 2)
		{
			b = 0;
			for(t = 0+(cur_page*chem_per_page); t < DATA_NUM_CHEMICALS-2; t++)
			{
				gotoxy(40,b+5);
				cprintf("\xBA");
				gotoxy(42,b+5);
				if(y_hover-2 != t)
				{
					textcolor(STYLE_COLOR_NOT_SELECTED);
					cprintf(" %s",json_chemicals_data_name[t]);
				}
				else
				{
					textcolor(STYLE_COLOR_SELECTED);
					cprintf(" %s",json_chemicals_data_name[t]);
					textcolor(STYLE_COLOR_NOT_SELECTED);
					gotoxy(1,5);
					cprintf("\xBA %s %s",string[22],json_chemicals_data_name[t]);
					gotoxy(1,6);
					cprintf("\xBA %s %d",string[21],json_chemicals_data_mass[t]);
					gotoxy(1,7);
					cprintf("\xBA %s %d",string[23],t);
					a = 0;
					for(n = 0; n < DATA_NUM_REACTIONS;n++)
					{
						if(json_reactions_data_reacts[n][t] > 0)
						{
							a++;
							gotoxy(1,a+8);
							cprintf("\xBA As reactant in: %s",json_reactions_data_name[n]);
						}
					}
				}
				if(t > chem_per_page+(cur_page*chem_per_page))
				{
					break;
				}
				b++;
			}
		}
		for(t = 0; t < MAX_PAGES*2; t += 2)
		{
			gotoxy(79,(t/2)+5);
			if(t/2 == cur_page)
			{
				printf("\xDB");
				gotoxy(79,(t/2)+6);
				printf("\xDB");
			}
			else
			{
				printf("\xB0");
				gotoxy(79,(t/2)+6);
				printf("\xB1");
			}
			printf("\xBA");
		}
		t = getch();
		if(t == 'N'||t == 'n')
		{
			cur_page++;
		}
		if(t == 'P'||t == 'p')
		{
			cur_page--;
		}
		if(t == 0)
		{
			t = getch();
			if(t == 0x4D)
			{
				y_hover = 1;
				menu_select++;
			}
			if(t == 0x4B)
			{
				y_hover = 1;
				menu_select--;
			}
			if(t == 0x50)
			{
				y_hover++;
			}
			if(t == 0x48)
			{
				y_hover--;
			}
		}
		if(y_hover < cur_page*chem_per_page)
		{
			y_hover = (cur_page*chem_per_page)-(2);
		}
		if(cur_page > MAX_PAGES||cur_page < 0)
		{
			cur_page = 0;
		}
		if(menu_select < 1)
		{
			menu_select = 6;
		}
		if(menu_select > 6)
		{
			menu_select = 1;
		}
		if(t == 'X'||t == 'x')
		{
			return 0;
		}
		gotoxy(1,5);
		for(b = 0; b < TEXT_MODE_SCREEN_WIDTH*40; b++)
		{
			printf(" ");
		}
	}
}
