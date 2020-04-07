/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "perror.h"
#include "pstrings.h"
#include "stringsm.h"
#include "fileio.h"
#include "room/room.h"
#include "room/find.h"


static void display_choices(int roomln);

/*Execute the appropriate action according to the type and the arg received*/
void parser_execins(char* type, char* arg, bool* inif, bool* ifcond)
{
    //TODO: refactor condition checking
    if(!strcmp(type, "PRINT"))
    {
        printf("\n");
        pstrings_display(arg);
        printf("\n");
    } else if (strcmp(type, "IF") == 0)
    {
        *inif = 1;
    } else if (*inif == 0 && *ifcond == false && !strcmp(type, "DISPLAY"))
    {    
        if(!strcmp(arg, "CHOICES"))
        {
            int roomln = 0;
            char* croomid = calloc((P_MAX_BUF_SIZE - 1), sizeof(char));

            pvars_getstdvars("currentroom", &croomid);
            find_roomline(croomid, &roomln);
            display_choices(roomln);

            free(croomid);
        }
    } else if (*inif == 0 && *ifcond == 0 && strcmp(type, "END") == 0)
    {
    } else
    {
        printf("arg: %s\n", type);
        perror_disp("UNK_INS_TYPE", 0);
    }
    
}

/*Extract the type and the argument from a string*/
void parser_splitline(char** type, char** arg, char* ins)
{
    int i = 0;
    int len = 0;
    char* argtocpy = calloc(P_MAX_BUF_SIZE, sizeof(char));
    
    stringsm_chomp(ins);
    len = strlen(ins);
    ins[len] = '\0';
    stringsm_getfw(type, ins, &i);
    if(len != (int)strlen(*type))
    {
        int findex = 0;

        for(int index = i; index < len; index++)
        {
            argtocpy[findex] = ins[index];
            findex++;
        }
        strcpy(*arg, argtocpy);
    } else
    {
        strcpy(*arg, "ARGNULL");
    }

    free(argtocpy);
}


static void display_choicetext(int choiceln, int num);

static void display_choices(int roomln)
{
    int choicesln = 0;
    bool choicesremain = true;
    bool choicesexist = find_choicesline(&choicesln, roomln);
    
    if(!choicesexist)
    {
        perror_disp("NO_CHOICES_INS", 1);
    }
    for(int i = 0; choicesremain; i++)
    {
        int onechoiceln = 0;
        bool choicesexist = find_onechoiceline((i+1), choicesln, &onechoiceln);
        
        if(!choicesexist)
        {
            if(i == 0)
            {
                perror_disp("NO_CHOICE_FND", 1);
            }
            choicesremain = false;
        } else
        {
            display_choicetext(onechoiceln, (i+1));
        }
    }
}

/*Display the text attributed to a choice ; choiceln must point to the
line number of the beginning of the choice block*/
static void display_choicetext(int choiceln, int num)
{
    bool textfound = false;
    bool inif = false;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* roomfile = calloc(P_MAX_BUF_SIZE, sizeof(char));
    FILE* fp = NULL;

    pvars_getgcvars("roomfile", &roomfile);
    fileio_setfileptr(&fp, roomfile);
    fileio_gotoline(&fp, choiceln);

    free(roomfile);

    for(int i = 0; !textfound; i++)
    {
        char* arg = calloc(P_MAX_BUF_SIZE - 1, sizeof(char));
        char* type = calloc(P_MAX_BUF_SIZE - 1, sizeof(char));

        fgets(buf, (P_MAX_BUF_SIZE - 1), fp);
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        parser_splitline(&type, &arg, buf);

        if(!strcmp(type, "TEXT"))
        {
            textfound = true;
            printf("\n%i - ", num);
            pstrings_display(arg);
        }
        else if(!strcmp(type, "END"))
        {
            if(inif)
            {
                inif = false;
            } else
            {
                free(buf);
                free(type);
                free(arg);
                perror_disp("NO_CHOICE_TEXT", 1);

                break;
            }
        } else if(buf[0] == 'I' && buf[1] == 'F')
        {
            inif = true;
        }

        free(type);
        free(arg);
    }

    free(buf);
}
