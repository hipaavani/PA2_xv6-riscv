#ifndef _PSTREE_H_
#define _PSTREE_H_

#define MAXPNAME 16
#define MAXPROC 64
struct pinfo{
	int pid;
	int ppid;
	uint64 sz;
	char name[MAXPNAME];
};
#endif
