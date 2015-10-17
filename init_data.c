#include "support.h"
#include "91ict.h"

#define UI_FILE "Dict.glade"

int
main(int argc, char** argv) {

	/* Init btree */
	BTA *tree_word, *tree_soundex;
	btinit();

#define INIT_FILE_DATA(name) 	{ \
	if (!check_file_exist("./data/"#name"-dict.data")) {\
		tree_word = btcrt("./data/"#name"-dict.data", 0, TRUE);\
		name##_load_2_tree(tree_word, #name);\
		if (check_file_exist("./data/"#name"-soundex.data"))\
			tree_soundex = btopn("./data/"#name"-soundex.data", 0, TRUE);\
		else {\
			tree_soundex = btcrt("./data/"#name"-soundex.data", 0, TRUE);\
			gen_soundex_db(tree_soundex, tree_word);\
		}\
		btcls(tree_soundex);\
		btcls(tree_word);\
	}\
}
	printf("%s\n", "Begin create data for ENG VN");
	INIT_FILE_DATA(ENG_VN);
	printf("%s\n", "Create data for ENG VN Done");
	printf("%s\n", "Begin create data for VN ENG");
	INIT_FILE_DATA(VN_ENG);
	printf("%s\n", "Create data for VN ENG Done");

#undef INIT_FILE_DATA

	return (0);
}

