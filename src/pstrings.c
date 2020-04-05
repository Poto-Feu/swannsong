/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

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
#include "vars/pvars.h"
#include "vars/pconst.h"
#include "pstrings.h"
#include "stringsm.h"
#include "fileio/fileio.h"

void pstrings_fetch(char* id, char** rstr);

/*Display the string corresponding to the id*/
void pstrings_display(char *id)
{
    char* rstring = calloc(P_MAX_BUF_SIZE, sizeof(char));;

    pstrings_fetch(id, &rstring);
    printf("%s", rstring);

    free(rstring);
}

static void open_strfile(FILE **f);

/*Check if a string is defined in the lang file*/
bool pstrings_check_exist(char* id)
{
    bool isfnd = true;
    char* result_str = calloc(P_MAX_BUF_SIZE, sizeof(char));

    pstrings_fetch(id, &result_str);
    
    if(!strcmp(result_str, "ERR_STR_NULL"))
    {
        isfnd = false;
    }

    return isfnd;
}
/*Copy the corresponding string into the pointer of a char pointer*/
void pstrings_fetch(char* id, char** rstr)
{
    int index = 0;
    int len = 0;
    bool id_exist = false;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* ustr = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* fstring = NULL;
    char* id_found = NULL;
    FILE* fp = NULL;

    open_strfile(&fp);
    while (fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL) {
        id_found = calloc(P_MAX_BUF_SIZE, sizeof(char));
        stringsm_chomp(buf);
        strcpy(ustr, buf);
        len = strlen(ustr);

        for(int i = 0; i < len; i++)
        {
            if(ustr[i] == ' ')
            {
                index = i;
                strncpy(id_found, ustr, i);
                break;
            }
        }

        if(!strcmp(id, id_found))
        {
            id_exist = 1;
            free(id_found);
            break;
        } else
        {
            free(id_found);
        }
    }
    fstring = calloc(len, sizeof(char));

    if (id_exist == 1)
    {
        int findex = 0;
        bool quote_inc = false;
        
        for(int i = index + 1; i < len; i++)
        {
            if(!quote_inc)
            {
                if(ustr[i] == '"')
                {
                    quote_inc = 1;
                } else 
                {
                    strcpy(fstring, "ERR_BAD_STR_FRMT");
                    break;
                }
            } else if (ustr[i] != '"')
            {
                fstring[findex] = ustr[i];
                findex++;
            }
        }
    } else
    {
        strcpy(fstring, "ERR_STR_NULL");
    }
    strcpy(*rstr, fstring);

    fclose(fp);
    free(fstring);
    free(ustr);
    free(buf);
}

/*Set the file pointer to the file containing the strings correponding
to the selected language*/
static void open_strfile(FILE **f)
{
    char* langdir = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* lang = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* langfile = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* txt = ".txt";
    int langdirln = 0;

    pvars_getgcvars("langdir", &langdir);
    pvars_getstdvars("lang", &lang);
    strcpy(langfile, langdir);
    langdirln = strlen(langdir);

    for(int i = 0; i < (int)strlen(lang); i++)
    {
        langfile[langdirln] = lang[i];
        langdirln++;
    }
    for(int i = 0; i < (int)strlen(txt); i++)
    {
        langfile[langdirln] = txt[i];
        langdirln++;
    }
    fileio_setfileptr(f, langfile);
    
    free(langdir);
    free(lang);
    free(langfile);
}

