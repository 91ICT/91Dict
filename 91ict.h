#ifndef __91_ICT__
#define __91_ICT__

#include "support.h"

////////// For DB
// check exist file
int check_file_exist(const char *file_name);

// create db file from foldoc data file
void foldoc_load_2_tree(BTA *tree, const char *foldoc_file);

// Create soundex db file from word db file
void gen_soundex_db(BTA *soundex_t, BTA *word_t);

// soundex find similar string
char *soundex(const char *in);

///////// Support API for singal processing
// Support for Add function in add dialog
gboolean add_word_to_dict(ChData *data, char *word, char *mean);

// Support for edit function in edit dialog
gboolean edit_mean_word_from_dict(ChData *data, char *word, char *mean);

// Support for delete function in edit dialog
gboolean delete_word_from_dict(ChData *data, char *word);

// Support for bookmark function for bookmark btn
gboolean add_word_to_bookmark(ChData *data, char *word);

// Support for bookmark function for del bookmark btn
gboolean del_word_bookmark(ChData *data, char *word);

/* Clear the Entry */
void reset_Entry(GtkWidget *Entry);

// bookmark init
void bookmark_init(ChData *data);

/* Clear the TextView */
void reset_TextView(GtkWidget *TextView);

void status_dialog(GtkWindow *parent, gchar *message);

#endif
