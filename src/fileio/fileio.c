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
#include "../vars/pconst.h"
#include "fileio.h"
#include "parser.h"
#include "../perror.h"
#include "../stringsm.h"

void fileio_gotoline(FILE** fp, int ln)
{
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    for(int i = 0; i < ln; i++)
    {
        if(fgets(buf, P_MAX_BUF_SIZE - 1, *fp) == NULL)
        {
            perror_disp("FILE_LINE_NF", 1);
        } else
        {
            stringsm_chomp(buf);
            stringsm_rtab(buf);
        }
    }
    free(buf);
}

void fileio_getln(int* ln, char* s)
{
    FILE *fp = fopen("txt/rooms.txt", "r");
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    //char buf[500];
    int i = 0;
    *ln = 0;
    *buf = '\0';
    while(fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL)
    {
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        if(strcmp(s, buf) == 0)
        {
            *ln = i + 1;
            break;
        }
        i++;
    }
    fclose(fp);
    free(buf);
}

// Execute all the instructions until the end of the block
void fileio_execuntilend(int startln)
{
    _Bool inblock = 0;
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    char* type = NULL;
    char* arg = NULL;
    FILE* fp = fopen("txt/rooms.txt", "r");

    *buf = '\0';
    fileio_gotoline(&fp, startln);
    while(fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL)
    {
        type  = malloc((P_MAX_BUF_SIZE - 1 ) * sizeof(char));
        arg = malloc((P_MAX_BUF_SIZE - 1) * sizeof(char));
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        parser_splitline(type, arg, buf);
        if(strcmp(type, "END") != 0)
        {
            parser_execins(type, arg, &inblock);
            free(type);
            free(arg);
        } else 
        {
            free(type);
            free(arg);
            break;
        }
    }
    free(buf);
    fclose(fp);
}
