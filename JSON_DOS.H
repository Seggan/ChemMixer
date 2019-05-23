#ifndef JSON_DOS_H
#define JSON_DOS_H
#include <stdio.h>
#include <dos.h>

FILE *fp;
char json_file_string[255];
short json_file_number;
float json_file_float;
static char ch_piece;
static short o;

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

void json_dos_lib_struct_header(void)
{
	ch_piece = check_for('"');
	if(ch_piece == 1)
	{
		for(o = 0;o < 255;o++)
		{
			if(ch_piece == 1)
			{
				json_file_string[o] = fgetc(fp);
				if(json_file_string[o] == '"')
				{
					/*Set ch_piec to 0, so we dont write anything else*/
					/*Plus remove the " from it, so its clean*/
					json_file_string[o] = 0;
					ch_piece = 0;
					break;
				}
			}
		}
	}
}

void json_dos_lib_struct_string(void)
{
	ch_piece = check_for('"');
	if(ch_piece == 1)
	{
		for(o = 0;o < 255;o++)
		{
			if(ch_piece == 1)
			{
				json_file_string[o] = fgetc(fp);
				if(json_file_string[o] == '"')
				{
					/*Set the last string (that is a "), to zero
					 *so we have our raw string*/
					json_file_string[o] = 0;
					ch_piece = 0;
					break;
				}
			}
		}
	}
}

void json_dos_lib_struct_number(void)
{
	ch_piece = check_for(':');
	if(ch_piece == 1)
	{
		fscanf(fp,"%d",&json_file_number);
	}
}

void json_dos_lib_struct_float(void)
{
	ch_piece = check_for(':');
	if(ch_piece == 1)
	{
		fscanf(fp,"%f",&json_file_float);
	}
}
#endif
