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
