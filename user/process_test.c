#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstree_bonus.h"

#define MAXPROC 64
int main(void)
{
	struct pinfo table[MAXPROC];

	int n= getprocess(table);
	
	printf("%d\n", n);

	for(int i=0; i<n; i++){
		printf("pid =%d ppid= %d sz=%lu name=%s\n",
			table[i].pid, table[i].ppid, table[i].sz, table[i].name);
}
	exit(0);
}
