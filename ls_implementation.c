#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include <zconf.h>
#include <stdbool.h>

#define MAX 1024

void read_and_parse(char *formatted_string, char *user_input, char **argv);
void parse_user_input(char *user_input, char **argv);
char *setup(void);
void porapats_ls(char ** argv);


int main(){
	char *formatted_string = setup();
	char user_input[MAX];
	char *argv[64];
	while (1) {
		read_and_parse(formatted_string, user_input, argv);
		if (strcmp(argv[0], "exit") == 0) {
			break;
		}
		if (strcmp(argv[0], "cd") == 0) {
			chdir(argv[1]);
			continue;
		}
		if (strcmp(argv[0], "ls") == 0){
			porapats_ls(argv);
		}
	}
}

void porapats_ls(char ** argv){
	// Readdir returns a pointer to a direct structure not "direct structure" - Whoops
	struct dirent ** identifier;
	struct dirent * readdir_ptr;

	//struct dirent {
	//    ino_t          d_ino;       /* inode number */
	//	off_t          d_off;       /* offset to the next dirent */
	//	unsigned short d_reclen;    /* length of this record */
	//	unsigned char  d_type;      /* type of file; not supported
	//                                   by all file system types */
	//	char           d_name[256]; /* filename */
	//};

	// Scanndir returns the number of directory entities.
	int num_directory_entities;
	// TODO - test if you can glob and find dotfiles .*
	//malloced strings sorted through qsort filtered for nonzeros into a string
	//Taken from the man pages example
//	int scandir(const char *dirp, struct dirent ***namelist,
//	            int (*filter)(const struct dirent *),
//	            int (*compar)(const struct dirent **, const struct dirent **));

		//                               director,   dirent,       filter?,   compar
		num_directory_entities = scandir(".", &identifier, NULL, alphasort);
		DIR *curr_dirr;
		//50 files feels reasonable
		char files[50][MAX];
		curr_dirr = opendir(".");
		if (curr_dirr == NULL){
			perror("Something Went Wrong");
		}
		for(int i=0; i < num_directory_entities; i++){
			readdir_ptr = readdir(curr_dirr);
			if (readdir_ptr == NULL){
				perror("Somethings wrong again");
			}
			strcpy(files[i], readdir_ptr->d_name);
		}
		closedir(curr_dirr);
		for(int i=0;i<num_directory_entities;i++)
		{
			if(files[i][0]!='.')
				printf("%s\n", files[i]);
		}
}


char *setup(void) {
	char *name = getenv("LOGNAME");
	char *cwd = getenv("PWD");
	char *Formatted = malloc(strlen(name) + strlen(cwd) + 17);
	// Move Pointer to remove /home/username
	cwd += (strlen(getenv("OLDPWD")));
	// 18 Including null terminators and characters manually added
	// Trying out manual mallocing
	sprintf(Formatted, "λ %s@ESE333 [%s] \n→ ", name, cwd);
	return Formatted;
}



void parse_user_input(char *user_input, char **argv) {
	bool mid_string = false;
	while (*user_input != '\0') {
		while (*user_input == ' ' || *user_input == '\t' || *user_input == '\n') {
			if (!mid_string) {
				*user_input++ = '\0';
			} else {
				*user_input++;
			}
		}
		if (!mid_string && *user_input != '\0') {
			*argv++ = user_input;
		}
		while (*user_input != '\0' && *user_input != ' ' && *user_input != '\t' &&
		       *user_input != '\n') {
			if (*user_input == '\"' || *user_input == '\'') {
				mid_string = !mid_string;
			}
			user_input++;
		}
	}
	*argv = '\0';
}

void read_and_parse(char *formatted_string, char *user_input, char **argv) {
	fputs(formatted_string, stdout);
	fgets(user_input, MAX, stdin);
	// Skipline
	fputs("\n", stdout);
	parse_user_input(user_input, argv);
}