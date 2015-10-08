#include "support.h"
#include "91ict.h"

#define UI_FILE "Dict.glade"

int
main(int argc, char** argv)
{
     ChData     *data;
     GtkBuilder *builder;
     GError     *error = NULL;

     /* Init btree */
     BTA *tree;
     btinit();

     if(check_file_exist("dict_db"))
          tree = btopn("dict_db", 0, FALSE);
     else{
          tree = btcrt("dict_db", 0, FALSE);
          foldoc_load_2_tree(tree, "FOLDOC");
     }

     /* Init GTK+ */
     gtk_init(&argc, &argv);

     /* Create new GtkBuilder object */
     builder = gtk_builder_new();
     if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
     {
          g_warning("%s", error->message);
          g_free(error);
          return 1;
     }

     /* Allocate data structure */
     data = g_slice_new(ChData);

     /* Get objects from UI */
#define GW(name) CH_GET_WIDGET(builder, name, data)
     GW(main_window);
     GW(dlg_Add);
     GW(dlg_About);
     GW(dlg_Delete);

     GW(word_entry_add_dlg);
     GW(meaning_txt_add_dlg);
     GW(tree_completion);
     GW(search_word);
     GW(txt_meaning);
     data->tree = tree;
     /* Get more objects to use */
#undef GW
     CH_GET_OBJECT(builder, list_store, GTK_LIST_STORE, data);
     /* Connect signals */
     gtk_builder_connect_signals(builder, data);

     /* Destroy builder, since we don't need it anymore */
     g_object_unref(G_OBJECT(builder));

     /* Show window */
     gtk_widget_show(data -> main_window);

     /* Start main loop */
     gtk_main();

     /* Free any allocated data */
     g_slice_free(ChData, data);

     return(0);
}

