#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <gtk/gtk.h>
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
     GtkWidget *word_tree;
     GtkWidget *search_word;
     GtkWidget *txt_meaning;
     BTA *tree; //  tree
     /* Insert more widgets to use */
};

#endif /* __SUPPORT_H__ */


