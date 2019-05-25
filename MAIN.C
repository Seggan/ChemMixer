#include <stdio.h>
#include "EGI.H"
#include "DEF.H"

char check_for(char check)
{
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
		memccpy(&jcfs_data_chem_name[t],&holder.chem_name,strlen(holder.chem_name)+1);
		jcfs_data_melting_point[t] = holder.melting_point;
		jcfs_data_boiling_point[t] = holder.boiling_point;
		memccpy(&jcfs_data_state_name[t],&holder.state_name,strlen(holder.state_name)+1);
		jcfs_data_mass[t] = holder.mass;
	}
	return 1;
}

char parse_reaction_json(const char* json_chemical_filename)
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
	for(t = 0; t < NUM_REACTIONS; t++)
	{
		/*Seek for the reaction name*/
		ch_piec = check_for('"');
		if(ch_piec == 1)
		{
			for(o = 0;o < 255;o++)
			{
				if(ch_piec == 1)
				{
					holder2.react_name[o] = fgetc(fp);
					if(holder2.react_name[o] == '"')
					{
						/*Set ch_piec to 0, so we dont write anything else*/
						/*Plus remove the " from it, so its clean*/
						holder2.react_name[o] = 0;
						ch_piec = 0;
						break;
					}
				}
			}
		}
		/*Be sure to skip the : character!*/
		ch_piec = check_for(':');
		/*Finaly, lets seek out the reactants
		 *or components, btw*/
		ch_piec = check_for('[');
		if(ch_piec == 1)
		{
			for(o = 0;o < 255;o++)
			{
				if(ch_piec == 1)
				{
					holder2.reactants[o] = fgetc(fp);
					if(holder2.reactants[o] == ']')
					{
						/*Set ch_piec to 0, so we dont write anything else*/
						/*Plus remove the " from it, so its clean*/
						holder2.reactants[o] = 0;
						/*Number of string*/
						mj = 0;
						gh_check = 1;
						for(u = 0; u < 255; u++)
						{
							if(holder2.reactants[u] == '"'&&holder2.reactants[u+1] != ']')
							{
								holder2.reactants[u] = 0;
								if(holder2.reactants[u+1] == ','||holder2.reactants[u+1] == '"')
								{
									holder2.reactants[u+1] = 0;
								}
								if(holder2.reactants[gh_check] != 0)
								{
									if(mj != 0)
									{
										n = 0;
										for(k = gh_check; k < u; k++)
										{
											holder2.string_hold[n] = holder2.reactants[k];
											n++;
										}
										n = 0;
										for(k = 0; k < NUM_CHEMICALS; k++)
										{
											if(strcmp(holder2.string_hold,jcfs_data_chem_name[k]) == 0)
											{
												jrfs_data_reactants[t][k]++;
											}
										}
										/*Set back string to its normal form*/
										for(k = 0; k < 255; k++)
										{
											holder2.string_hold[k] = 0;
										}
									}
									mj++;
									jrfs_data_reactants_num[t] = mj;
								}
								gh_check = u+1;
							}
						}
						/*
						printf("\nRESULTS: %s\n",holder2.result);
						*/
						ch_piec = 0;
						break;
					}
				}
			}
		}
		/*Max Temperature*/
		ch_piec = check_for('"');
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder2.max_temp);
		}
		/*Min Temperature*/
		ch_piec = check_for('"');
		ch_piec = check_for(':');
		if(ch_piec == 1)
		{
			fscanf(fp,"%d",&holder2.min_temp);
		}
		/*Finaly, lets seek out the reactants
		 *or components, btw*/
		ch_piec = check_for('[');
		if(ch_piec == 1)
		{
			for(o = 0;o < 255;o++)
			{
				if(ch_piec == 1)
				{
					holder2.result[o] = fgetc(fp);
					if(holder2.result[o] == ']')
					{
						/*Set ch_piec to 0, so we dont write anything else*/
						/*Plus remove the " from it, so its clean*/
						holder2.result[o] = 0;
						/*Number of string*/
						mj = 0;
						gh_check = 1;
						for(u = 0; u < 255; u++)
						{
							if(holder2.result[u] == '"'&&holder2.result[u+1] != ']')
							{
								holder2.result[u] = 0;
								if(holder2.result[u+1] == ','||holder2.result[u+1] == '"')
								{
									holder2.result[u+1] = 0;
								}
								if(holder2.result[gh_check] != 0)
								{
									if(mj != 0)
									{
										n = 0;
										for(k = gh_check; k < u; k++)
										{
											holder2.string_hold[n] = holder2.result[k];
											n++;
										}
										n = 0;
										for(k = 0; k < NUM_CHEMICALS; k++)
										{
											if(strcmp(holder2.string_hold,jcfs_data_chem_name[k]) == 0)
											{
												jrfs_data_result[t][k]++;
											}
										}
										/*Set back string to its normal form*/
										for(k = 0; k < 255; k++)
										{
											holder2.string_hold[k] = 0;
										}
									}
									mj++;
								}
								gh_check = u+1;
							}
						}
						/*
						printf("\nRESULTS: %s\n",holder2.result);
						*/
						ch_piec = 0;
						break;
					}
				}
			}
		}
		memccpy(&jrfs_data_react_name[t],&holder2.react_name,strlen(holder2.react_name)+1);
		jrfs_data_max_temp[t] = holder2.max_temp;
		jrfs_data_min_temp[t] = holder2.min_temp;
		printf("\n------ REACTION %s DATA ------",jrfs_data_react_name[t]);
		printf("\nREACTS : ");
		for(k = 0; k < NUM_CHEMICALS; k++)
		{
			if(jrfs_data_reactants[t][k] > 0)
			{
				printf("%s, ",jcfs_data_chem_name[k]);
			}
		}
		printf("\nRESULTS : ");
		for(k = 0; k < NUM_CHEMICALS; k++)
		{
			if(jrfs_data_result[t][k] > 0)
			{
				printf("%s, ",jcfs_data_chem_name[k]);
			}
		}
		printf("\nMAX TEMPERATURE : %d",jrfs_data_max_temp[t]);
		printf("\nMIN TEMPERATURE : %d",jrfs_data_min_temp[t]);
		getch();
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

void set_text_color_8(char color)
{
	union REGS regs;
	regs.h.ah = 0x09;
	regs.h.bl = color;
	regs.h.al = 0x20;
	regs.x.cx = 0x8;
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

int main(void)
{
	char select_mode = 0;
	char y;
	/*Now make a list of the chemicals and also
	*copy the data into the structures*/
	parse_chemical_json("CHEMICAL.JSO");
	parse_reaction_json("REACT.JSO");
	igd_data_temp = 20;
	/*After this point we dont need to bother
	 * about opening files because everything
	 * of our data relies on the memory
	 * and is allocated within the 64 KB
	 * memory*/
	cursor_pos.x = 0;
	cursor_pos.y = 0;
	for(k = 0; k < NUM_CHEMICALS; k++)
	{
		jcfs_use_amount[k] = 0;
	}
	for(;;)
	{
		set_video_to_text();
		/*1st to 4th line*/
		set_text_color_fullscreen(0x1F);
		for(y = 0; y < NUM_CHEMICALS;y++)
		{
			/*Becomes Liquid*/
			if(jcfs_data_melting_point[y] <= igd_data_temp)
			{
				jcfs_data_state_name[y] = 0;
			}
			/*Becomes Gaseous*/
			if(jcfs_data_boiling_point[y] <= igd_data_temp)
			{
				jcfs_data_state_name[y] = 1;
			}
			/*Becomes Solid*/
			if(jcfs_data_melting_point[y] > igd_data_temp)
			{
				jcfs_data_state_name[y] = 2;
			}
		}
		/*Now lets check if its compatible with any reaction*/
		for(mj = 0; mj < NUM_REACTIONS; mj++)
		{
			u = 0;
			if(igd_data_temp > jrfs_data_min_temp[mj]&&igd_data_temp < jrfs_data_max_temp[mj])
			{
				/*Check how many reactants available whe have*/
				for(k = 0; k < NUM_CHEMICALS; k++)
				{
					if(jrfs_data_reactants[mj][k] > 0 && jcfs_use_amount[k] > 0)
					{
						u++;
					}
				}
				/*If we equal to u, then whe can do a reaction*/
				if(jrfs_data_reactants_num[mj] == u)
				{
					for(k = 0; k < NUM_CHEMICALS; k++)
					{
						if(jrfs_data_reactants[mj][k] > 0&&jcfs_use_amount[k] > 0)
						{
							jcfs_use_amount[k]--;
						}
					}
					for(k = 0; k < NUM_CHEMICALS; k++)
					{
						if(jrfs_data_result[mj][k] > 0)
						{
							jcfs_use_amount[k]++;
						}
					}
				}
			}
		}
		gotoxy(0,0);
		print_line_of('\xC9','\xCD','\xBB');
		printf("\xBA Chemixer 0.6.2");
		gotoxy(79,2);
		printf(" \xBA");
		print_line_of('\xBA',' ','\xBA');
		gotoxy(2,3);
		printf(" Recipent Temperature: %d Celsius degrees [",igd_data_temp);
		/*Values that need constant update*/
		if(igd_data_temp < -49)
		{
			printf("Frozen]");
		}
		if(igd_data_temp > -50&&igd_data_temp < 1)
		{
			printf("Very Freezing]");
		}
		if(igd_data_temp > 0&&igd_data_temp < 26)
		{
			printf("Freezing]");
		}
		if(igd_data_temp > 25&&igd_data_temp < 50)
		{
			printf("Average]");
		}
		if(igd_data_temp > 49&&igd_data_temp < 200)
		{
			printf("Hot]");
		}
		if(igd_data_temp > 199)
		{
			printf("Burning]");
		}
		gotoxy(1,4);
		if(select_mode == 0)
		{
			printf("\xBA (A)dd chemical (R)emove chemical Change (T)emperature (C)redits (E)xit");
		}
		if(select_mode == 1||select_mode == 5)
		{
			if(select_mode == 1)
			{
				printf("\xBA (Intro)Add (E)xit");
			}
			if(select_mode == 5)
			{
				printf("\xBA (Intro)Remove (E)xit");
			}
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
		if(select_mode == 2||select_mode == 4)
		{
			printf("\xBA (E)xit");
		}
		if(cursor_pos.x > 1)
		{
			cursor_pos.x = 1;
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
		switch(select_mode)
		{
			case 0:
			{
				print_line_of('\xCC','\xCD','\xB9');
				gotoxy(40,7);
				printf("\xCB");
				gotoxy(80,7);
				printf("\xB9");
				for(y = 0; y < NUM_CHEMICALS; y += 2)
				{
					gotoxy(0,8+(y/2));
					printf("\xBA");
					if(jcfs_use_amount[y] > 0)
					{
						printf(" (%d) %s",jcfs_use_amount[y],jcfs_data_chem_name[y]);
						switch(jcfs_data_state_name[y])
						{
							/*Liquid*/
							case 0:
								printf("( \xB2 )");
								break;
							/*Gaseous*/
							case 1:
								printf("( \xB0 )");
								break;
							/*Solid*/
							case 2:
								printf("( \xDB )");
								break;
							default:
								printf("( ? )");
								break;
						}
					}
					gotoxy(40,8+(y/2));
					printf("\xBA");
					if(jcfs_use_amount[y+1] > 0)
					{
						printf(" (%d) %s",jcfs_use_amount[y+1],jcfs_data_chem_name[y+1]);
						switch(jcfs_data_state_name[y+1])
						{
							/*Liquid*/
							case 0:
								printf("( \xB2 )");
								break;
							/*Gaseous*/
							case 1:
								printf("( \xB0 )");
								break;
							/*Solid*/
							case 2:
								printf("( \xDB )");
								break;
							default:
								printf("( ? )");
								break;
						}
					}
					gotoxy(79,8+(y/2));
					printf(" \xBA");
				}
				break;
			}
			case 1:
			{
				print_line_of('\xCC','\xCD','\xB9');
				gotoxy(40,7);
				printf("\xCB");
				gotoxy(80,7);
				printf("\xB9");
				for(y = 0; y < NUM_CHEMICALS; y += 2)
				{
					gotoxy(0,8+(y/2));
					printf("\xBA %s",jcfs_data_chem_name[y]);
					gotoxy(40,8+(y/2));
					printf("\xBA %s",jcfs_data_chem_name[y+1]);
					gotoxy(79,8+(y/2));
					printf(" \xBA");
				}
				if(cursor_pos.x <= 0)
				{
					gotoxy(cursor_pos.x*40+3,cursor_pos.y+8);
				}
				else
				{
					gotoxy(cursor_pos.x*40+2,cursor_pos.y+8);
				}
				printf("\xDB");
				break;
			}
			case 2:
			{
				print_line_of('\xCC','\xCD','\xB9');
				gotoxy(0,8);
				printf("\xBA Ported to DOS by: Jesus Antonio Diaz (JesusAnt5)");
				gotoxy(79,8);
				printf(" \xBA");
				gotoxy(0,9);
				printf("\xBA Original Game: Seggan");
				gotoxy(79,9);
				printf(" \xBA");
				break;
			}
			case 3:
			{
				printf("Debug Mode");
				break;
			}
			case 4:
			{
				gotoxy(0,8);
				printf("\xBA Set new temperature value:");
				gotoxy(0,9);
				printf("\xBA ");
				scanf("%d",&igd_data_temp);
				select_mode = 0;
				break;
			}
			case 5:
			{
				print_line_of('\xCC','\xCD','\xB9');
				gotoxy(40,7);
				printf("\xCB");
				gotoxy(80,7);
				printf("\xB9");
				for(y = 0; y < NUM_CHEMICALS; y += 2)
				{
					gotoxy(0,8+(y/2));
					printf("\xBA %s",jcfs_data_chem_name[y]);
					gotoxy(40,8+(y/2));
					printf("\xBA %s",jcfs_data_chem_name[y+1]);
					gotoxy(79,8+(y/2));
					printf(" \xBA");
				}
				if(cursor_pos.x <= 0)
				{
					gotoxy(cursor_pos.x*40+3,cursor_pos.y+8);
				}
				else
				{
					gotoxy(cursor_pos.x*40+2,cursor_pos.y+8);
				}
				printf("\xDB");
				break;
			}
		}
		menu_select = getch();
		/*Select Mode*/
		switch(select_mode)
		{
			case 0:
			{
				if(menu_select == 'A'||menu_select == 'a')
				{
					select_mode = 1;
				}
				if(menu_select == 'C'||menu_select == 'c')
				{
					select_mode = 2;
				}
				if(menu_select == 'D'||menu_select == 'd')
				{
					select_mode = 3;
				}
				if(menu_select == 'R'||menu_select == 'r')
				{
					select_mode = 5;
				}
				if(menu_select == 'T'||menu_select == 't')
				{
					select_mode = 4;
				}
				if(menu_select == 'E'||menu_select == 'e')
				{
					set_video_to_text();
					return 0;
				}
				break;
			}
			case 1:
			{
				/*Exit*/
				if(menu_select == 'E'||menu_select == 'e')
				{
					select_mode = 0;
				}
				/*Arrow keys*/
				if(menu_select == KEY_ENTER)
				{
					jcfs_use_amount[cursor_pos.y*2+cursor_pos.x]++;
					select_mode = 0;
				}
				break;
			}
			case 2:
			{
				if(menu_select == 'E'||menu_select == 'e')
				{
					select_mode = 0;
				}
				break;
			}
			case 3:
			{
				if(menu_select == 'E'||menu_select == 'e')
				{
					select_mode = 0;
				}
				break;
			}
			case 5:
			{
				/*Exit*/
				if(menu_select == 'E'||menu_select == 'e')
				{
					select_mode = 0;
				}
				/*Arrow keys*/
				if(menu_select == KEY_ENTER && jcfs_use_amount[cursor_pos.y*2+cursor_pos.x] > 0)
				{
					jcfs_use_amount[cursor_pos.y*2+cursor_pos.x]--;
					select_mode = 0;
				}
				break;
			}
		}
	}
}
