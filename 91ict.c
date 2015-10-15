#include "91ict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <sys/stat.h>

////////// For DB
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
	if(tree == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
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

// Create soundex db file from word db file
void gen_soundex_db(BTA *soundex_t, BTA *word_t){
	if(word_t == NULL ||  soundex_t == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	// set tree word position
	btpos(word_t, ZSTART);

	while(1) {
		char word[200];
		char mean[200000];
		int size_mean_rev, size_series_word_rev;

		if(btseln(word_t, word, mean, sizeof(mean), &size_mean_rev) == QNOKEY)
			break;

		char *soundex_str = soundex(word);
		char series_word[200000];
		if(btsel(soundex_t, soundex_str, series_word, sizeof(series_word), &size_series_word_rev) != 0){
			// Don't soundex_str on soundex_tree 
			btins(soundex_t, soundex_str, word, strlen(word) + 1);
		} else {
			// have soundex_str on soundex_tree 
			char separated[100] = ";";
			strcat(separated, word);
			strcat(series_word, separated);
			btupd(soundex_t, soundex_str, series_word, strlen(series_word) + 1);
		}
		free(soundex_str);
	}
	btpos(word_t, ZSTART);

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

///////// Support API for singal processing
// Support for Add in add dialog
gboolean add_word_to_dict(ChData *data, char *word, char *mean){
	if (word == NULL || mean == NULL || data == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	
	char mean_rev[2000000];
	int size_mean_rev;

	if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &size_mean_rev) != 0){
		// Don't word on tree_word
		if(btins(data->tree_word, word, mean, strlen(mean) + 1) != 0)
			return FALSE;

		char *soundex_str = soundex(word);
		char series_word[200000];
		int size_series_word_rev;

		if(btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &size_series_word_rev) != 0){
			// Don't soundex_str on soundex_tree 
			if(btins(data->tree_soundex, soundex_str, word, strlen(word) + 1) != 0)
				return FALSE;
		} else {
			// have soundex_str on soundex_tree 
			char separated[100] = ";";
			strcat(separated, word);
			strcat(series_word, separated);
			if(btupd(data->tree_soundex, soundex_str, series_word, strlen(series_word) + 1) != 0)
				return FALSE;
		}
		free(soundex_str);

		return TRUE;
	} 
	return FALSE;
}

// Support for bookmark function for bookmark btn
gboolean add_word_to_bookmark(ChData *data, char *word){
	if (word == NULL || data == NULL){
			fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
			exit(1);
		}
		
		int value;
		if(bfndky(data->tree_bookmark, word, &value) != 0){
			// Don't word on tree_bookmark
			if(binsky(data->tree_bookmark, word, 1) != 0)
				return FALSE;

			return TRUE;
		} 
	return FALSE;
}

// Support for edit function in edit dialog
gboolean edit_mean_word_from_dict(ChData *data, char *word, char *mean){
	if (word == NULL || mean == NULL || data == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	
	char mean_rev[2000000];
	int size_mean_rev;

	if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &size_mean_rev) == 0){
		// have word on tree_word
		if(btupd(data->tree_word, word, mean, strlen(mean) + 1) != 0)
			return FALSE;

		return TRUE;
	} 

	return FALSE;
}


// Support for delete function in edit dialog
gboolean delete_word_from_dict(ChData *data, char *word){
	if (word == NULL || data == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	char mean_rev[2000000];
	int size_mean_rev;

	if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &size_mean_rev) == 0){
		// have word on tree_word
		if(btdel(data->tree_word, word) != 0)
			return FALSE;


		char *soundex_str = soundex(word);
		char series_word[200000];
		int size_series_word_rev;

		if(btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &size_series_word_rev) != 0){
			// haven't soundex_str on soundex_tree 
			fprintf(stderr, "ERROR: word haven't add soundex string %s:%d\n", __FILE__, __LINE__);
			return TRUE;
		} else {
			// have soundex_str on soundex_tree 

			char str[1000];
			int count = 0;

			strcpy(str, series_word);
			strcpy(series_word, "");
			char *wordi = strtok (str, ";");
			while (wordi != NULL)  {
;
				if(count == 0)
					if(strcmp(wordi, word) != 0)
						strcat(series_word, wordi);
				char separated[100] = ";";
				if(strcmp(wordi, word) == 0){
					wordi = strtok (NULL, ";");
					continue;
				}
				strcat(separated, wordi);
				strcat(series_word, separated);
				wordi = strtok (NULL, ";");
				count++;
				if (count == 50)
				{
					break;
				}
			}

			if(count == 1)
				if(btdel(data->tree_soundex, soundex_str) != 0){
					return FALSE;
					goto end;
				}

			if(btupd(data->tree_soundex, soundex_str, series_word, strlen(series_word) + 1) != 0)
				return FALSE;
		}
end:
		free(soundex_str);
		return TRUE;
	} 
	return FALSE;
}

// Support for bookmark function for del bookmark btn
gboolean del_word_bookmark(ChData *data, char *word){
	if (word == NULL || data == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	int value;

	if(bfndky(data->tree_bookmark, word, &value) == 0){
		// have word on tree_bookmark

		if(bdelky(data->tree_bookmark, word) != 0)
			return FALSE;

		return TRUE;
	} 

	return FALSE;
}

// bookmark init
void bookmark_init(ChData *data){
	if(data == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	if(data->tree_bookmark == NULL)
		return;

	btpos(data->tree_bookmark, ZSTART);
	while(1){

        char word[200];
        int value;
        if(bnxtky(data->tree_bookmark, word, &value) == QNOKEY)
            break;
		GtkTreeIter  iter;
		gtk_list_store_append(data->bookmark_list_store, &iter);
		gtk_list_store_set (data->bookmark_list_store, &iter, 0, word, -1);
   }
}

/* Clear the Entry */
void reset_Entry(GtkWidget *Entry)
{
	GtkEntryBuffer *buffer = gtk_entry_buffer_new (NULL, 0);
	gtk_entry_set_buffer( (GtkEntry*) Entry, buffer);
}

/* Clear the TextView */
void reset_TextView(GtkWidget *TextView)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer ((GTK_TEXT_VIEW(TextView)));
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	gtk_text_buffer_delete(buffer, &start, &end);
}

void status_dialog(GtkWindow *parent, gchar *message)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(parent,
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, message);
	gtk_window_set_title(GTK_WINDOW(dialog), "Status");
	gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
