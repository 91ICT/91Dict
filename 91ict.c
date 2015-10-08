#include "91ict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// check exist file
int check_file_exist(const char *file_name){
     struct stat buffer;
     int exist = stat(file_name, &buffer);
     if(exist == 0)
          return 1;
     else // -1
          return 0;
}

// create db file from foldoc data file
void foldoc_load_2_tree(BTA *tree, const char *foldoc_file){
     FILE *f;
     f = fopen(foldoc_file, "r");

     int n=0;
     char word[100],
          mean[10000];
     char temp[100];
     while(!feof(F))
     {
          fgets(temp, 100, f);
          if(temp[0] != '\t' && temp[0] != '\n')
          { // if word
               if(n==0) // if first time
               {
                    n++;
                    strcpy(word, temp);
                    word[strlen(temp)-1] = '\0';
               }
               else
               {
                    mean[strlen(mean)-1] = '\0';
                    btins(tree, word, mean, strlen(mean)+1);
                    strcpy(mean, ""); // free
                    n++;
                    strcpy(word, temp);
                    word[strlen(temp)-1] = '\0';
               }
          } // mean
          else strcat(mean, temp);
     }
     defn[strlen(mean)-1] = '\0';
     btins(tree, word, mean, sizeof(mean));
     fclose(f);
}
