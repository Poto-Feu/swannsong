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

extern "C" 
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vars/pvars.h"
#include "vars/pconst.h"
#include "perror.h"
#include "stringsm.h"
#include "fileio/fileio.h"
}

#include <string>
#include <vector>
#include <iostream>
#include "pstrings.h"

typedef struct
{
    std::string id;
    std::string val;
} PstringsElement;

static std::vector<PstringsElement> pstrings_arr {};

static void open_strfile(FILE **f);
static void split_file_line(std::string* r_id, std::string* r_val, char* buf);
static void add_pstring_to_vec(std::string p_id, std::string p_val);

void pstrings_copy_file_to_vec()
{
    FILE* fp = NULL;
    char buf[P_MAX_BUF_SIZE];

    open_strfile(&fp);
    while(fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL)
    {
        std::string r_id;
        std::string r_val;

        stringsm_chomp(buf);
        stringsm_rtab(buf);

        if(buf[0] != '\0')
        {
            split_file_line(&r_id, &r_val, buf);
            add_pstring_to_vec(r_id, r_val);
        } else continue;
    }
    fclose(fp);
}

static void split_file_line(std::string* r_id, std::string* r_val, char* buf)
{
    int sp_ind = 0;
    int quote_ind = 0;
    bool quote_inc = false;

    for(int i = 0; buf[i] != '\0'; i++)
    {
        if(buf[i] == ' ' || buf[i] == '\t') break;
        sp_ind++;
        *r_id += buf[i];
    }
    for(int i = sp_ind; buf[i] != '\0'; i++)
    {
        if(buf[i] == '"')
        {
            quote_inc = true;
            quote_ind = i;
            break;
        }
    }

    if(!quote_inc)
    {
        perror_disp((char*)"wrong pstring format", 1);
    }
    for(int i = quote_ind+1; buf[i] != '\0'; i++)
    {
        if(buf[i] == '"') break;
        else *r_val += buf[i];
    }
}

static void add_pstring_to_vec(std::string p_id, std::string p_val)
{
    PstringsElement new_el = {p_id, p_val};
    pstrings_arr.push_back(new_el);
}

void pstrings_fetch(char* id, char** rstr);

/*Display the string corresponding to the id*/
void pstrings_display(char *id)
{
    char* rstring = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));;

    pstrings_fetch(id, &rstring);
    printf("%s", rstring);

    free(rstring);
}


/*Check if a string is defined in the lang file*/
bool pstrings_check_exist(char* id)
{
    bool isfnd = false;
    auto str_id(id);

    for(const auto& it : pstrings_arr)
    {
        if(str_id == it.id)
        {
            isfnd = true;
            break;
        }
    }
    
    return isfnd;
}
/*Copy the corresponding string into the pointer of a char pointer*/
void pstrings_fetch(char* id, char** rstr)
{
    int index = 0;
    int len = 0;
    bool id_exist = false;
    char* buf = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* ustr = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* fstring = NULL;
    char* id_found = NULL;
    FILE* fp = NULL;

    open_strfile(&fp);
    while (fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL) {
        id_found = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
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
    fstring = (char*)calloc(len, sizeof(char));

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
    char* langdir = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* lang = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* langfile = (char*)calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* txt = (char*)".txt";
    int langdirln = 0;

    pvars_getgcvars((char*)"langdir", &langdir);
    pvars_getstdvars((char*)"lang", &lang);
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

