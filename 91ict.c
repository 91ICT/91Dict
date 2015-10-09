#include "91ict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

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
     if(f == NULL){
          fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
          exit(1);
     }
     
     int n=0;
     char word[100],
          mean[100000];
     char temp[100];

     while(!feof(f))
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
     mean[strlen(mean)-1] = '\0';
     btins(tree, word, mean, strlen(mean)+1);
     
     fclose(f);
}

char *soundex(const char *in) {
   int code[] =
      {  0,1,2,3,0,1,2,0,0,2,2,4,5,5,0,1,2,6,2,3,0,1,0,2,0,2 };
      /* a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z */
   char *key = malloc(sizeof(strlen(in)));
   register char ch;
   register int last;
   register int count;

   /* Set up default key, complete with trailing '0's */
   strcpy(key, "Z000");

   /* Advance to the first letter.  If none present, 
      return default key */
   while (*in != '\0'  &&  !isalpha(*in))
      ++in;
   if (*in == '\0')
      return key;

   /* Pull out the first letter, uppercase it, and 
      set up for main loop */
   key[0] = toupper(*in);
   last = code[key[0] - 'A'];
   ++in;

   /* Scan rest of string, stop at end of string or 
      when the key is full */
   for (count = 1;  count < 4  &&  *in != '\0';  ++in) {
      /* If non-alpha, ignore the character altogether */
      if (isalpha(*in)) {
         ch = tolower(*in);
         /* Fold together adjacent letters sharing the same code */
         if (last != code[ch - 'a']) {
            last = code[ch - 'a'];
            /* Ignore code==0 letters except as separators */
            if (last != 0)
               key[count++] = '0' + last;
         }
      }
   }

   return key;
}