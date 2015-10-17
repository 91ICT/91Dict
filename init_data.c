#include <btree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

static inline void loadBar(int x, int n, int r, int w)
{
	// Only update r times.
	if ( x % (n / r + 1) != 0 ) return;

	// Calculuate the ratio of complete-to-incomplete.
	float ratio = x / (float)n;
	int   c     = ratio * w;

	// Show the percentage complete.
	printf("%3d%% [", (int)(ratio * 100) );

	// Show the load bar.
	{
		int x;
		for (x = 0; x < c; x++)
			printf("=");
	}
	{
		int x;
		for (x = c; x < w; x++)
			printf(" ");
	}

	// ANSI Control codes to go back to the
	// previous line and clear it.
	printf("]\n\033[F\033[J");
}

char *soundex(const char *in);
int check_file_exist(const char *file_name);
void gen_soundex_db(BTA *soundex_t, BTA *word_t);
void FOLDOC_load_2_tree(BTA *tree, const char *foldoc_file);
void dict_load_2_tree(BTA *tree, const char *dict_file);

int main(int argc, char** argv) {

	/* Init btree */
	BTA *tree_word, *tree_soundex;
	btinit();

#define INIT_FILE_DATA(name) 	{ \
	if (!check_file_exist("./data/"#name"-dict.data")) {\
		printf("%s\n", "Begin create data for "#name);\
		tree_word = btcrt("./data/"#name"-dict.data", 0, TRUE);\
		dict_load_2_tree(tree_word, #name);\
		if (check_file_exist("./data/"#name"-soundex.data"))\
			tree_soundex = btopn("./data/"#name"-soundex.data", 0, TRUE);\
		else {\
			tree_soundex = btcrt("./data/"#name"-soundex.data", 0, TRUE);\
			gen_soundex_db(tree_soundex, tree_word);\
		}\
		btcls(tree_soundex);\
		btcls(tree_word);\
		printf("%s\n", "Create data for "#name" done");\
	}\
}


	INIT_FILE_DATA(FOLDOC);
	INIT_FILE_DATA(ENG_VN);
	INIT_FILE_DATA(VN_ENG);


#undef INIT_FILE_DATA

	return (0);
}

int check_file_exist(const char *file_name) {
	struct stat buffer;
	int exist = stat(file_name, &buffer);
	if (exist == 0)
		return 1;
	else // -1
		return 0;
}


// create db file from foldoc data file
void FOLDOC_load_2_tree(BTA *tree, const char *foldoc_file) {
	if (tree == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	FILE *f;
	f = fopen(foldoc_file, "r");
	if (f == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	int n = 0;
	char word[100],
	     mean[100000];
	char temp[100];
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	rewind(f);

	while (!feof(f))
	{
		fgets(temp, 100, f);
		if (temp[0] != '\t' && temp[0] != '\n')
		{	// if word
			if (n == 0) // if first time
			{
				n++;
				strcpy(word, temp);
				word[strlen(temp) - 1] = '\0';
			}
			else
			{
				mean[strlen(mean) - 1] = '\0';
				btins(tree, word, mean, strlen(mean) + 1);
				strcpy(mean, ""); // free
				n++;
				strcpy(word, temp);
				word[strlen(temp) - 1] = '\0';
			}
		} // mean
		else strcat(mean, temp);
		loadBar(ftell(f), sz, 100, 100);
	}
	printf("Load %d word\n", n + 2);
	mean[strlen(mean) - 1] = '\0';
	btins(tree, word, mean, strlen(mean) + 1);

	fclose(f);
}

// create db file from foldoc dict file
void dict_load_2_tree(BTA *tree, const char *dict_file) {
	if (strcmp(dict_file, "FOLDOC") == 0)
		return FOLDOC_load_2_tree(tree, dict_file);
	if (tree == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	FILE *f;

	f = fopen(dict_file, "r");
	if (f == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	rewind(f);
	int n = 0;
	char word[100],
	     mean[100000];
	char temp[1000];
	while (!feof(f))
	{
		fgets(temp, 1000, f);
		if (temp[0] == '@')
		{	// if word

			if (n == 0) // if first time
			{
				n++;
				strcat(mean, "\t");
				sscanf(temp, "@%[^/\n]%[^\n]", word, mean);
				strcat(mean, "\n");
				word[strlen(word)] = '\0';
			}
			else
			{
				mean[strlen(mean) - 1] = '\0';
				btins(tree, word, mean, strlen(mean) + 1);
				strcpy(mean, "\n"); // free
				n++;
				sscanf(temp, "@%[^/\n]%[^\n]", word, mean);
				strcat(mean, "\n");
				if (word[strlen(word) - 1] == ' ')
					word[strlen(word) - 1] = '\0';
				else
					word[strlen(word)] = '\0';
			}
		} // mean
		else {
			if (temp[0] != '*')
				strcat(mean, "\t\t");
			else
				strcat(mean, "\t");
			strcat(mean, temp);
		}
		loadBar(ftell(f), sz, 100, 100);
	}
	printf("Load %d word\n", n + 2);
	mean[strlen(mean) - 1] = '\0';
	btins(tree, word, mean, strlen(mean) + 1);

	fclose(f);
}

// Create soundex db file from word db file
void gen_soundex_db(BTA *soundex_t, BTA *word_t) {
	if (word_t == NULL ||  soundex_t == NULL) {
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	// set tree word position
	btpos(word_t, ZSTART);

	while (1) {
		char word[200];
		char mean[200000];
		int size_mean_rev, size_series_word_rev;

		if (btseln(word_t, word, mean, sizeof(mean), &size_mean_rev) == QNOKEY)
			break;

		char *soundex_str = soundex(word);
		char series_word[200000];
		if (btsel(soundex_t, soundex_str, series_word, sizeof(series_word), &size_series_word_rev) != 0) {
			// Don't soundex_str on soundex_tree
			btins(soundex_t, soundex_str, word, strlen(word) + 1);
		} else {
			// have soundex_str on soundex_tree
			char separated[100] = ";";
			strcat(separated, word);
			strcat(series_word, separated);
			btupd(soundex_t, soundex_str, series_word, strlen(series_word) + 1);
		}
		free(soundex_str);
	}
	btpos(word_t, ZSTART);
}

char *soundex(const char *in) {
	int code[] =
	{  0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0, 1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2 };
	/* a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z */
	char *key = malloc(sizeof(strlen(in)));
	register char ch;
	register int last;
	register int count;

	/* Set up default key, complete with trailing '0's */
	strcpy(key, "Z000");

	/* Advance to the first letter.  If none present,
	  return default key */
	while (*in != '\0'  &&  !isalpha(*in))
		++in;
	if (*in == '\0')
		return key;

	/* Pull out the first letter, uppercase it, and
	  set up for main loop */
	key[0] = toupper(*in);
	last = code[key[0] - 'A'];
	++in;

	/* Scan rest of string, stop at end of string or
	  when the key is full */
	for (count = 1;  count < 4  &&  *in != '\0';  ++in) {
		/* If non-alpha, ignore the character altogether */
		if (isalpha(*in)) {
			ch = tolower(*in);
			/* Fold together adjacent letters sharing the same code */
			if (last != code[ch - 'a']) {
				last = code[ch - 'a'];
				/* Ignore code==0 letters except as separators */
				if (last != 0)
					key[count++] = '0' + last;
			}
		}
	}

	return key;
}


