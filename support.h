#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <gtk/gtk.h>
#ifdef TRUE
#undef TRUE
#undef FALSE
#endif
#include <btree.h>

/* Macros for obtaining objects from UI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
data->name = type ( gtk_builder_get_object(builder, #name ))
#define CH_GET_WIDGET( builder, name, data )                    \
CH_GET_OBJECT( builder, name, GTK_WIDGET, data )

/* Main data structure definition */
typedef struct _ChData ChData;
struct _ChData
{
	/* Widget */
	GtkWidget *main_window; /* Main application window */
	GtkWidget *dlg_Add;     /* Add new word dialog */
	GtkWidget *dlg_About;   /* About dialog */
	GtkWidget *dlg_Delete;  /* Delete dialog */
	GtkWidget *word_entry_add_dlg; // add
	GtkWidget *meaning_txt_add_dlg;
	GtkWidget *search_word;
	GtkWidget *txt_meaning;
	GtkListStore *bookmark_list_store;
	GtkListStore *list_word_list_store;
	GtkListStore *entry_completion_list_store;
	BTA *tree_word; //  key word, value mean
	BTA *tree_soundex; // key string of soundex, string contain list of word has same string of soundex sperat separated by ";"
	BTA *tree_bookmark;
	BTA *tree_suggests;
	GtkWidget *word_label_edit_dlg;
	GtkWidget *meaning_txt_edit_dlg;
	GtkWidget *dlg_edit;
	GtkTreeSelection *list_word_tree_view_selection;
	GtkTreeSelection *bookmark_tree_view_selection;
	GtkEntryCompletion *entry_completion;
	GtkWidget *btn_Add;
	GtkWidget *btn_Edit;
	GtkWidget *btn_Delete;
	GtkWidget *btn_bookmark;
	GtkWidget *btn_del_bookmark;
	gboolean word_meaningful_search_entry; // if word_meaningful = TRUE --> current word in search entry is has mean otherwise is false

	/* Insert more widgets to use */
};

#endif /* __SUPPORT_H__ */


