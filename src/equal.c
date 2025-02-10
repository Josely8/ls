#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "equal.h"

bool IsEqual(const char* name_one, const char* name_two){
FILE* file_one = fopen(name_one, "r");
    FILE* file_two = fopen(name_two, "r");

    //checkin whether the files are ok
    /*if(!file_one)
    {
        if(file_two){
           fclose(file_two); 
        }
        return false;
    }
    if (file_two == NULL){
        if(file_one){
           fclose(file_one); 
        }
        return false;
    }*/
    char elem, check;
    while (1) 
    {
        do 
        {
            elem = fgetc(file_one);
        } 
        while ((elem == '\n') || (elem == ' ') ||(elem == '\t'));

        do 
        {
            check = fgetc(file_two);
        } 
        while ((check == '\n') || (check == ' ') || (check == '\t'));
        
        if ((elem == EOF) && (check == EOF)) 
        {
            fclose(file_one);
            fclose(file_two);
            
            return true;
        }

        else if(elem == EOF){
            fclose(file_one);
            fclose(file_two);
            
            return false;
        }
        else if(check == EOF){
            fclose(file_one);
            fclose(file_two);
            
            return false;
        }

        else if(elem != check){
            fclose(file_one);
            fclose(file_two);
            fprintf(stdout, "  %c ", elem);
            fprintf(stdout, "   %c  ", check);
            
            return false;
        }
    
    }
    
}