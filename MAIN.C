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
#define DATA_NUM_CHEMICALS			24
#define DATA_NUM_REACTIONS			27

#define STYLE_COLOR_SELECTED 		0x0
#define STYLE_BACK_SELECTED			0xF

#define STYLE_COLOR_NOT_SELECTED	0x7
#define STYLE_BACK_NOT_SELECTED		0x0

#define MAX_PAGES	2

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
unsigned char chem_per_page = 24;

/*Data*/
char json_reactions_data_name[27][255];
char json_reactions_data_reacts[27][24];
short *json_reactions_data_temp_max;
short *json_reactions_data_temp_min;
char json_reactions_data_results[27][24];
char *json_reactions_data_chemicals;

char json_chemicals_data_name[27][255];
short *json_chemicals_data_temp_max;
short *json_chemicals_data_temp_min;
char *json_chemicals_data_state;
short *json_chemicals_data_mass;

short json_chemicals_game_usage[27];
short game_baker_temperature;

const char delim[2] = ",";
char *token;

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
		for(b = 0; b < 255; b++)
		{
			json_chemicals_data_name[t][b] = str_[b];
		}
		locate(':',stream);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[0]);
		json_chemicals_data_temp_max[t] = _nnum[0];
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[1]);
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
		json_chemicals_data_state[t] = _nnum[3];
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[2]);
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
	json_chemicals_data_temp_max = malloc(DATA_NUM_REACTIONS);
	json_chemicals_data_temp_min = malloc(DATA_NUM_REACTIONS);
	json_reactions_data_chemicals = malloc(DATA_NUM_REACTIONS);
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
		memcpy(json_reactions_data_name[t],str,strlen(str)+1);
		printf("\nReact Name >> %s\n",json_reactions_data_name[t]);
		locate(':',stream);
		locate('[',stream);
		str_ = malloc(512);
		a = 0;
		for(;;)
		{
			if(str_[a] == ']')
			{
				str_[a] = 0;
				break;
			}
			if(str_[a-1] == ']')
			{
				str_[a-1] = 0;
				break;
			}
			str_[a] = fgetc(stream);
			if(str_[a] == '"')
			{
				str_[a] = 0;
				a--;
			}
			a++;
		}
		json_reactions_data_chemicals[t] = 0;
		token = strtok(str_,delim);
		while(token != NULL)
		{
			printf("REACT %s ",token);
			for(b = 0; b < DATA_NUM_CHEMICALS; b++)
			{
				g = strcmp(token,json_chemicals_data_name[b]);
				if(g == 0)
				{
					json_reactions_data_reacts[t][b]++;
					json_reactions_data_chemicals[t]++;
					printf("(%d)",json_reactions_data_reacts[t][b]);
				}
			}
			token = strtok(NULL,delim);
		}
		_nnum = malloc(3);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[0]);
		locate(':',stream);
		fscanf(stream,"%d",&_nnum[1]);
		locate(':',stream);
		locate('[',stream);
		a = 0;
		for(;;)
		{
			str_[a] = fgetc(stream);
			if(str_[a] == ']'||str_[a-1] == ']')
			{
				str_[a] = 0;
				break;
			}
			if(str_[a] == '"')
			{
				str_[a] = 0;
				a--;
			}
			a++;
		}
		token = strtok(str_,delim);
		while(token != NULL)
		{
			printf("RESULTS %s ",token);
			for(b = 0; b < DATA_NUM_CHEMICALS; b++)
			{
				g = strcmp(token,json_chemicals_data_name[b]);
				if(g == 0)
				{
					json_reactions_data_results[t][b]++;
				}
			}
			token = strtok(NULL,delim);
		}
		json_chemicals_data_temp_max[t] = _nnum[0];
		json_chemicals_data_temp_min[t] = _nnum[1];
		printf(" Max temp: %d",json_chemicals_data_temp_max[t]);
		printf(" Min temp: %d",json_chemicals_data_temp_min[t]);
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
	"Reacts in:",
	"Quantity in baker:",
	"Results in:",
	"Press D to change temperature, then input the desired temperature:"
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
	json_parse_reactions("REACT.JSO",fp);
	getch();
	textbackground(STYLE_BACK_SELECTED);
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
				textbackground(STYLE_BACK_SELECTED);
				textcolor(STYLE_COLOR_SELECTED);
				cprintf(string[t]);
			}
			else
			{
				textbackground(STYLE_BACK_NOT_SELECTED);
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
			for(n = 0; n < DATA_NUM_CHEMICALS;n += 2)
			{
				if(y_hover-2 == n)
				{
					textbackground(STYLE_BACK_SELECTED);
					textcolor(STYLE_COLOR_SELECTED);
					gotoxy(1,(n/2)+5);
					cprintf("\xBA \x3E %s (%d)",json_chemicals_data_name[n],json_chemicals_game_usage[n]);
				}
				else
				{
					textbackground(STYLE_BACK_NOT_SELECTED);
					textcolor(STYLE_COLOR_NOT_SELECTED);
					gotoxy(1,(n/2)+5);
					cprintf("\xBA %s (%d)",json_chemicals_data_name[n],json_chemicals_game_usage[n]);
				}
				if(json_chemicals_data_state[n] == 1)
				{
					cprintf(" (\xB0)");
				}
				if(json_chemicals_data_state[n] == 2)
				{
					cprintf(" (\xDB)");
				}
				if(json_chemicals_data_state[n] == 3)
				{
					cprintf(" (\xB1)");
				}
				if(json_chemicals_data_state[n] != 3&&json_chemicals_data_state[n] != 2&&json_chemicals_data_state[n] != 1)
				{
					cprintf(" (?)");
				}
				if(y_hover-2 == n+1)
				{
					textbackground(STYLE_BACK_SELECTED);
					textcolor(STYLE_COLOR_SELECTED);
					gotoxy(40,(n/2)+5);
					cprintf("\xBA \x3E %s (%d)",json_chemicals_data_name[n+1],json_chemicals_game_usage[n+1]);
				}
				else
				{
					textbackground(STYLE_BACK_NOT_SELECTED);
					textcolor(STYLE_COLOR_NOT_SELECTED);
					gotoxy(40,(n/2)+5);
					cprintf("\xBA %s (%d)",json_chemicals_data_name[n+1],json_chemicals_game_usage[n+1]);
				}
				if(json_chemicals_data_state[n+1] == 1)
				{
					cprintf(" (\xB0)");
				}
				if(json_chemicals_data_state[n+1] == 2)
				{
					cprintf(" (\xDB)");
				}
				if(json_chemicals_data_state[n+1] == 3)
				{
					cprintf(" (\xB1)");
				}
				if(json_chemicals_data_state[n+1] != 3&&json_chemicals_data_state[n+1] != 2&&json_chemicals_data_state[n+1] != 1)
				{
					cprintf(" (?)");
				}
			}
		}
		/*
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
					textbackground(STYLE_BACK_NOT_SELECTED);
					textcolor(STYLE_COLOR_NOT_SELECTED);
					cprintf(" %s",json_chemicals_data_name[t]);
				}
				else
				{
					textbackground(STYLE_BACK_SELECTED);
					textcolor(STYLE_COLOR_SELECTED);
					cprintf(" %s",json_chemicals_data_name[t]);
					textbackground(STYLE_BACK_NOT_SELECTED);
					textcolor(STYLE_COLOR_NOT_SELECTED);
					gotoxy(1,5);
					cprintf("\xBA %s %s",string[22],json_chemicals_data_name[t]);
					gotoxy(1,6);
					cprintf("\xBA %s %d",string[21],json_chemicals_data_mass[t]);
					gotoxy(1,7);
					cprintf("\xBA %s %d",string[23],t);
					gotoxy(1,8);
					cprintf("\xBA %s %d",string[25],json_chemicals_game_usage[t]);
					a = 0;
					for(n = 0; n < DATA_NUM_REACTIONS;n++)
					{
						if(json_reactions_data_reacts[n][t] > 0)
						{
							gotoxy(1,a+9);
							cprintf("\xBA %s %s",string[24],json_reactions_data_name[n]);
							a++;
						}
					}
					for(n = 0; n < DATA_NUM_REACTIONS;n++)
					{
						if(json_reactions_data_results[n][t] > 0)
						{
							gotoxy(1,a+9);
							cprintf("\xBA %s %s",string[26],json_reactions_data_name[n]);
							a++;
						}
					}
				}
				if(t > chem_per_page+(cur_page*chem_per_page))
				{
					break;
				}
				b++;
				if(b > 40)
				{
					b = 0;
				}
			}
		}
		*/
		for(t = 0; t < 20; t++)
		{
			gotoxy(TEXT_MODE_SCREEN_WIDTH,t+5);
			cprintf("\xBA");
		}
		for(t = 0; t < 20; t++)
		{
			gotoxy(1,t+5);
			cprintf("\xBA");
		}
		if(menu_select == 3)
		{
			gotoxy(1,5);
			cprintf("\xBA %s",string[27]);
		}
		if(menu_select == 4)
		{
			gotoxy(1,5);
			cprintf("\xBA %s",string[6]);
			gotoxy(1,6);
			cprintf("\xBA %s %d",string[7],game_baker_temperature);
			gotoxy(1,7);
			cprintf("\xBA %s",string[8]);
			gotoxy(1,8);
			printf("\xCC");
			for(t = 0; t < TEXT_MODE_SCREEN_WIDTH-2; t++)
			{
				printf("\xCD");
			}
			printf("\xB9");
			a = 0;
			for(n = 0; n < DATA_NUM_CHEMICALS;n += 2)
			{
				gotoxy(1,(a/2)+9);
				cprintf("\xBA %s (%d)",json_chemicals_data_name[n],json_chemicals_game_usage[n]);
				gotoxy(40,(a/2)+9);
				cprintf("\xBA %s (%d)",json_chemicals_data_name[n+1],json_chemicals_game_usage[n+1]);
				a += 2;
			}
		}
		t = getch();
		/*
		if(t == 'N'||t == 'n')
		{
			cur_page++;
		}
		if(t == 'P'||t == 'p')
		{
			cur_page--;
		}
		*/
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
			if(t == 0x50 && menu_select < 3)
			{
				y_hover++;
			}
			if(t == 0x48 && menu_select < 3)
			{
				y_hover--;
			}
		}
		if(menu_select == 1||menu_select == 2)
		{
			if(t == 'D'||t == 'd')
			{
				for(b = 0+(cur_page*chem_per_page); b < DATA_NUM_CHEMICALS-2; b++)
				{
					if(y_hover-2 == b)
					{
						if(menu_select == 2)
						{
							json_chemicals_game_usage[b]--;
						}
						if(menu_select == 1)
						{
							json_chemicals_game_usage[b]++;
						}
					}
				}
			}
		}
		if(menu_select == 3)
		{
			if(t == 'D'||t == 'd')
			{
				gotoxy(strlen(string[27])+3,5);
				scanf("%i",&game_baker_temperature);
			}
		}
		/*
		if(y_hover < cur_page*chem_per_page)
		{
			y_hover = (cur_page*chem_per_page)-(2);
		}
		if(cur_page > MAX_PAGES||cur_page < 0)
		{
			cur_page = 0;
		}
		*/
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
		for(t = 0; t < DATA_NUM_REACTIONS; t++)
		{
			if(game_baker_temperature >= json_reactions_data_temp_min[t] && game_baker_temperature <= json_reactions_data_temp_max[t])
			{
				b = 0;
				for(n = 0; n < DATA_NUM_CHEMICALS;n++)
				{
					if(json_reactions_data_reacts[t][n] > 0)
					{
						if(json_chemicals_game_usage[n] >= json_reactions_data_reacts[t][n])
						{
							b++;
							json_chemicals_game_usage[n] -= json_reactions_data_reacts[t][n];
						}
					}
				}
				for(n = 0; n < DATA_NUM_CHEMICALS;n++)
				{				
					if(json_reactions_data_results[t][n] > 0 && b == json_reactions_data_chemicals[t])
					{
						json_chemicals_game_usage[n] += json_reactions_data_results[t][n];
					}
				}
			}
		}
		for(t = 0; t < DATA_NUM_CHEMICALS; t++)
		{
			if(game_baker_temperature > json_chemicals_data_temp_max[t])
			{
				json_chemicals_data_state[t] = 1;
			}
			if(game_baker_temperature < json_chemicals_data_temp_min[t])
			{
				json_chemicals_data_state[t] = 2;
			}
			if(game_baker_temperature < json_chemicals_data_temp_min[t] && game_baker_temperature > json_chemicals_data_temp_max[t])
			{
				json_chemicals_data_state[t] = 3;
			}
			if(json_chemicals_game_usage[t] < 0)
			{
				json_chemicals_game_usage[t] = 0;
			}
		}
		gotoxy(1,5);
		for(b = 0; b < TEXT_MODE_SCREEN_WIDTH*40; b++)
		{
			printf(" ");
		}
	}
}
