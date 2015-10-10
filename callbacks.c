#include "support.h"
#include "91ict.h"

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
    
    // GtkTextMark *mark;
    // GtkTextIter iter;
    // mark = gtk_text_buffer_get_insert (buffer);
    // gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);

    if (btsel(data->tree_word, search_word, meaning, sizeof(meaning), &rsize) == 0)
    		gtk_text_buffer_set_text(clear, meaning, -1);


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
     printf("Luong\n");
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

/* Clear the Entry */
void reset_Entry(GtkWidget *Entry)
{
	GtkEntryBuffer *buffer = gtk_entry_buffer_new (NULL, 0);
	gtk_entry_set_buffer( (GtkEntry*) Entry, buffer);
}


/* Clear the TextView */
void reset_TextView(GtkWidget *TextView)
{
	GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
	gtk_text_view_set_buffer( (GtkTextView*) TextView, buffer);
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
	printf("Add button clicked\n");
	const gchar *word = gtk_entry_get_text(GTK_ENTRY(data->word_entry_add_dlg));
	printf("Word Entry: %s\n", word);

	/* Get text from GtkTextView */
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer ( (GtkTextView*) (data->meaning_txt_add_dlg) );
	gchar *meaning;
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	meaning = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
	printf("Text View: %s\n", meaning);

	/* Insert the word into the tree */
	int result = btins(data->tree, word, meaning, sizeof(meaning));
	printf("Result: %d\n", result);
	if(result == 0){
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