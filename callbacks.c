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

 // for tree_view_selection
G_MODULE_EXPORT void 
on_changed(GtkTreeSelection *treeselection, ChData *data) {

  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *word;

  if (gtk_tree_selection_get_selected(treeselection, &model, &iter)) {

	gtk_tree_model_get(model, &iter, 0, &word,  -1);
	int rsize;
	char meaning[100000];
	GtkTextBuffer *Buffer;
	Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning));
	gtk_text_buffer_set_text(Buffer, "", -1);

	if (btsel(data->tree_word, word, meaning, sizeof(meaning), &rsize) == 0)
		gtk_text_buffer_set_text(Buffer, meaning, -1);

	g_free(word);
  }
}

// The “search-changed” signal fpr search entry
G_MODULE_EXPORT void 
func_search_word (GtkSearchEntry *entry, ChData *data){
	gchar *search_word = gtk_entry_get_text(GTK_ENTRY(entry));
	GtkTextBuffer *Buffer;



	Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning));
	gtk_text_buffer_set_text(Buffer, "", -1);
	
	int rsize;
	char meaning[100000];
	if (btsel(data->tree_word, search_word, meaning, sizeof(meaning), &rsize) == 0){
		gtk_list_store_clear(data->list_store);
		GtkTreeIter  iter;
		gtk_list_store_append(data->list_store, &iter);
		gtk_list_store_set (data->list_store, &iter, 0, search_word, -1);
		gtk_text_buffer_set_text(Buffer, meaning, -1);
	} else {
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

/* ADD button (on main window) handler */
G_MODULE_EXPORT void
on_btn_edit_clicked(GtkButton *btn_Add, ChData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *word;

	if (gtk_tree_selection_get_selected(data->tree_view_selection, &model, &iter)) {

		gtk_tree_model_get(model, &iter, 0, &word,  -1);
		gtk_label_set_text(GTK_LABEL(data->word_label_edit_dlg), word);
		gtk_dialog_run(GTK_DIALOG(data->dlg_edit));
		gtk_widget_hide(data->dlg_edit);
		g_free(word);
	} else {
		gchar *search_word = gtk_entry_get_text(GTK_ENTRY(data->search_word));
		if(strcmp(search_word, "") == 0)
			status_dialog( (GtkWindow*) data->main_window, "Please select word on list !");
		else{
			gtk_label_set_text(GTK_LABEL(data->word_label_edit_dlg), search_word);
			gtk_dialog_run(GTK_DIALOG(data->dlg_edit));
			gtk_widget_hide(data->dlg_edit);
		}
	}
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
/*************************
*      EDIT DIALOG       *
**************************/

/* Edit button handler */
G_MODULE_EXPORT void
on_btn_edit_clicked_edit_dlg(GtkButton *btn_, ChData *data)
{
	gchar *word = gtk_label_get_text(GTK_LABEL(data->word_label_edit_dlg));

	/* Get text from GtkTextView */
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer ( (GtkTextView*) (data->meaning_txt_edit_dlg) );
	gchar *meaning;
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	meaning = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	/* Edit the word into the tree */
	if(edit_mean_word_from_dict(data, word, meaning)){
		status_dialog( (GtkWindow*) data->dlg_Add, "Successfully!");
	}else{
		status_dialog( (GtkWindow*) data->dlg_Add, "An error has occured!\nPlease try again");
	}
	/* reset text field */
	reset_TextView(data->meaning_txt_edit_dlg);
	gtk_widget_hide(data->dlg_edit);

	reset_Entry(data->search_word);
	reset_TextView(data->txt_meaning);
	gtk_list_store_clear(data->list_store);
	/* free memory */
	g_free(meaning);

}

/* CANCEL button handler */
 G_MODULE_EXPORT void
 on_btn_Cancel_clicked_edit_dlg(GtkButton *btn_Cancel_add, ChData *data)
 {
	gtk_widget_hide(data->dlg_edit);     
	reset_TextView(data->meaning_txt_edit_dlg);
 }

 /*************************
*      DELETE DIALOG       *
**************************/

/* Delete button handler */
G_MODULE_EXPORT void
on_btn_del_clicked_del_dlg(GtkButton *btn_, ChData *data)
{
	
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *word;
	gint flag = 0;
	if (gtk_tree_selection_get_selected(data->tree_view_selection, &model, &iter)) {
		gtk_tree_model_get(model, &iter, 0, &word,  -1);
		flag = 1;
	} else {
		word = gtk_entry_get_text(GTK_ENTRY(data->search_word));
		if(strcmp(word, "") == 0)
			status_dialog( (GtkWindow*) data->main_window, "Please select word on list !");
	}
	
	/* Edit the word into the tree */
	if(delete_word_from_dict(data, word)){
		status_dialog( (GtkWindow*) data->dlg_Add, "Successfully!");
	}else{
		status_dialog( (GtkWindow*) data->dlg_Add, "An error has occured!\nPlease try again");
	}
	/* reset text field */

	gtk_widget_hide(data->dlg_Delete);
	reset_Entry(data->search_word);
	gtk_list_store_clear(data->list_store);

	/* free memory */
	if(flag == 1)
		g_free(word);
	reset_TextView(data->txt_meaning);
}

/* CANCEL button handler */
 G_MODULE_EXPORT void
 on_btn_Cancel_clicked_del_dlg(GtkButton *btn_Cancel_add, ChData *data)
 {
	gtk_widget_hide(data->dlg_Delete);
 }