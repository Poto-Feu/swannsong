/*
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
#include "vars/pvars.h"
#include "vars/pconst.h"
#include "pstrings.h"
#include "stringsm.h"

void open_strfile(FILE **f)
{
    if (strcmp(pvars_userlang, "fr") == 0)
    {
        *f = fopen("txt/fr.txt", "r");
    } else
    {
        *f = fopen("txt/en.txt", "r");
    }
    if (*f == NULL)
    {
        printf("\nfopen is null : your game files might be corrupted.");
        exit(1);
    }
}

void pstrings_fetch(char id[], char *rstr)
{
    int index = 0;
    int len = 0;
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    char* ustr = malloc(P_MAX_BUF_SIZE * sizeof(char));
    char* fstring = NULL;
    char* id_found = NULL;
    _Bool id_exist = 0;
    FILE* fp = NULL;

    *buf = '\0';
    *ustr = '\0';
    open_strfile(&fp);
    while (fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL) {
        id_found = malloc(P_MAX_BUF_SIZE * sizeof(char));
        *id_found = '\0';
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

        if(strcmp(id, id_found) == 0)
        {
            id_exist = 1;
            free(id_found);
            break;
        } else
        {
            free(id_found);
        }
    }
    fstring = malloc(len * sizeof(char));
    if (id_exist == 1)
    {
        int findex = 0;
        _Bool quote_inc = 0;
        
        for(int i = index + 1; i < len; i++)
        {
            if(quote_inc == 0)
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
        printf("idfound: %s\n", id);
        strcpy(fstring, "ERR_STR_NULL");
    }
    strcpy(rstr, fstring);
    fclose(fp);
    free(ustr);
    free(buf);
}

void pstrings_display(char *id)
{
    char* rstring = (char*) malloc(150*sizeof(char));;
    pstrings_fetch(id, rstring);
    printf("%s", rstring);
    free(rstring);
}
