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

    if (btsel(data->tree, search_word, meaning, sizeof(meaning), &rsize) == 0)
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

/***********************
 *     EDIT DIALOG     *
 ***********************/

/* CANCEL button handler */
 G_MODULE_EXPORT void
 on_btn_Cancel_add_clicked(GtkButton *btn_Cancel_add, ChData *data)
 {
      gtk_widget_hide(data->dlg_Add);
 }
