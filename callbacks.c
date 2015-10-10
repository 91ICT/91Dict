#include "support.h"
#include "91ict.h"
#include <stdlib.h>
#include <string.h>
#define MAX_SUGGEST 30

/* Stop application when closed */
G_MODULE_EXPORT void
on_main_window_destroy(GtkWidget *main_window)
{
		 gtk_main_quit();
}


/***********************
 *     MAIN WINDOW     *
 ***********************/

// The “search-changed” signal
G_MODULE_EXPORT void 
func_search_word (GtkSearchEntry *entry, ChData *data){
	gchar *search_word = gtk_entry_get_text(GTK_ENTRY(entry));
	GtkTextBuffer *clear;


	clear = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning));
		gtk_text_buffer_set_text(clear, "", -1);
		
		int rsize;
		char meaning[100000];
		if (btsel(data->tree_word, search_word, meaning, sizeof(meaning), &rsize) == 0)
				gtk_text_buffer_set_text(clear, meaning, -1);
		else{

				char *soundex_str = soundex(search_word);
				char series_word[10000];
				int size_series_word_rev;
				if (btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &size_series_word_rev) != 0)
						return;
				// clear
				gtk_list_store_clear(data->list_store);
				GtkTreeIter  iter;

				char str[1000];
				int count = 0;
				strcpy(str, series_word);
				char *wordi = strtok (str, ";");
				while (wordi != NULL)  {
					gtk_list_store_append(data->list_store, &iter);
					gtk_list_store_set (data->list_store, &iter, 0, wordi, -1);
					wordi = strtok (NULL, ";");
					count++;
					if(count > (MAX_SUGGEST))
						break;
			 }
			 free(soundex_str);
		}
}



/* ABOUT button handler */
G_MODULE_EXPORT void
on_btn_About_clicked(GtkButton *btn_About, ChData *data)
{
		 gtk_dialog_run(GTK_DIALOG(data->dlg_About));
		 gtk_widget_hide(data->dlg_About);
}

/* ADD button (on main window) handler */
G_MODULE_EXPORT void
on_btn_Add_clicked(GtkButton *btn_Add, ChData *data)
{

		 gtk_dialog_run(GTK_DIALOG(data->dlg_Add));
		 gtk_widget_hide(data->dlg_Add);
}

/* DELETE button handler */
G_MODULE_EXPORT void
on_btn_Delete_clicked(GtkButton *btn_Delete, ChData *data)
{
		 gtk_dialog_run(GTK_DIALOG(data->dlg_Delete));
		 gtk_widget_hide(data->dlg_Delete);
}


/*************************
*      ADD DIALOG        *
**************************/

/* CANCEL button handler */
 G_MODULE_EXPORT void
 on_btn_Cancel_add_clicked(GtkButton *btn_Cancel_add, ChData *data)
 {

      gtk_widget_hide(data->dlg_Add);     
      reset_Entry(data->word_entry_add_dlg);
      reset_TextView(data->meaning_txt_add_dlg);
 }


/* ADD button handler */
G_MODULE_EXPORT void
on_btn_Add_add_clicked(GtkButton *btn_Add_add, ChData *data)
{
	const gchar *word = gtk_entry_get_text(GTK_ENTRY(data->word_entry_add_dlg));

	/* Get text from GtkTextView */
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer ( (GtkTextView*) (data->meaning_txt_add_dlg) );
	gchar *meaning;
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	meaning = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	/* Insert the word into the tree */
	if(add_word_to_dict(data, word, meaning)){
		status_dialog( (GtkWindow*) data->dlg_Add, "Successfully!");
	}else{
		status_dialog( (GtkWindow*) data->dlg_Add, "An error has occured!\nPlease try again");
	}
	/* reset text field */
	reset_Entry(data->word_entry_add_dlg);
    reset_TextView(data->meaning_txt_add_dlg);

    /* free memory */
    g_free(meaning);
    //g_free(word);
}
