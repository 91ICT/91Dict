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
     BTA *tree_word, *tree_soundex;
     btinit();

     if(check_file_exist("dict_db"))
          tree_word = btopn("dict_db", 0, FALSE);
     else{
          tree_word = btcrt("dict_db", 0, FALSE);
          foldoc_load_2_tree(tree_word, "FOLDOC");
     }

     if(check_file_exist("soundex_db"))
          tree_soundex = btopn("soundex_db", 0, FALSE);
     else{
          tree_soundex = btcrt("soundex_db", 0, FALSE);
          gen_soundex_db(tree_soundex, tree_word);
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
     GW(dlg_edit);
     GW(word_entry_add_dlg);
     GW(meaning_txt_add_dlg);
     GW(word_label_edit_dlg);
     GW(meaning_txt_edit_dlg);
     GW(tree_view);
     GW(search_word);
     GW(txt_meaning);
     GW(btn_Add);
     GW(btn_Edit);
     GW(btn_Delete);
     data->tree_word = tree_word;
     data->tree_soundex = tree_soundex;
     /* Get more objects to use */
#undef GW
     CH_GET_OBJECT(builder, list_store, GTK_LIST_STORE, data);
     CH_GET_OBJECT(builder, tree_view_selection, GTK_TREE_SELECTION, data);
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
     btcls(tree_soundex);
     btcls(tree_word);


     return(0);
}

