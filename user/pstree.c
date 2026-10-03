#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstree_bonus.h"

void print_tree(struct pinfo table[], int n, int pid, int depth, int show_pid, int show_mem)
{
 int i;
 for(i=0; i<n; i++)

{ if (table[i].pid==pid)
	{ 
		int j;
		for(j=0; j<depth; j++){
			printf("    ");}
	
		if(show_pid && show_mem){
		     printf("%s(%d) [%lu]\n", table[i].name, table[i].pid, table[i].sz);}

		else if (show_pid){
			printf("%s(%d)\n", table[i].name, table[i].pid);}

		else if(show_mem){
			printf("%s [%lu]\n", table[i].name, table[i].sz);}

		else{
			printf("%s\n", table[i].name);}
	
		int k;
		for(k=0; k<n; k++){
				if(table[k].ppid == pid){
	
	
				print_tree(table, n, table[k].pid, depth+1, show_pid, show_mem	);}
}
	return;
	}
	
}

}

int main(int argc, char*argv[])
{
	struct pinfo table[MAXPROC];
	int n= getprocess(table);
	
	int show_pid=0;
	int show_mem=0;

	for (int i=0; i<argc; i++){
		if (strcmp(argv[1],"-p") ==0){
			show_pid=1;}
		else if(strcmp(argv[1], "-m")== 0){
			show_mem=1;}

		else if( strcmp(argv[1], "-pm")==0 || strcmp(argv[1], "-mp") ==0){
				show_pid=1;
				show_mem=1;}
}
	print_tree(table, n, 1, 0, show_pid, show_mem);
	exit(0);
}
