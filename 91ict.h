#ifndef __91_ICT__
#define __91_ICT__
#include <sys/stat.h>
#include <gtk/gtk.h>
#include "support.h"

// check exist file
int check_file_exist(const char *file_name);

// create db file from foldoc data file
void foldoc_load_2_tree(BTA *tree, const char *foldoc_file);

#endif
