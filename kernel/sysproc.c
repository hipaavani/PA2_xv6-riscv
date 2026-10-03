#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "pstree_bonus.h"
extern  uint ticks;
extern struct spinlock tickslock;
extern struct proc proc[NPROC];
struct pinfo table[NPROC];
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_getuptime(void)
{
   uint xticks;

   acquire(&tickslock);
   xticks=ticks;
   release(&tickslock);
   return xticks;
}

uint64
sys_activecount(void)
{
  int count = 0;
  struct proc *p;

  for (p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);

    if (p->state != UNUSED)
      count++;

    release(&p->lock);
  }

  return count;
}
uint64
sys_lineage(void)
{
  int pid;
  argint(0, &pid);

  struct proc *p;
  int count = 0;
  int found = 0;
  struct proc *current;

  for (p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);

    if (p->pid == pid && p->state != UNUSED) {
      found = 1;
      current = p;
      release(&p->lock);
      break;
    }

    release(&p->lock);
  }

  if (!found)
    return -1;

  while (current != 0) {
    char name[16];
    int current_pid;
    struct proc *parent;
    
    acquire(&current->lock);
    
    if (current->state == UNUSED) {
      release(&current->lock);
      break;
    }
    
    current_pid = current->pid;
    parent = current->parent;
    safestrcpy(name, current->name, sizeof(name));
    
    release(&current->lock);
    
    printk("PID %d: %s\n", current_pid, name);
    count++;
    
    if (current_pid == 1)
    break;
  
    current = parent;
}

return count;
}

uint64
sys_getprocsize(void)
{
  int pid;
  argint(0, &pid);

  struct proc *p;

  for (p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);

    if (p->pid == pid && p->state != UNUSED) {
      uint64 size = p->sz;
      release(&p->lock);
      return size;
    }

    release(&p->lock);
  }

  return -1;
}

uint64
sys_familyheadcount(void)
{ 
  int pid;
  struct proc *p;
  int count=0;

  argint(0, &pid);


  for(p=proc; p<&proc[NPROC]; p++){
   acquire(&p->lock);
  if(p->parent && p->parent->pid ==pid && p->state !=UNUSED && p->state!= ZOMBIE)
{
   count++;}
  
release(&p->lock);
  }
  return count;
}

uint64
sys_getprocess(void){
 
uint64 addr;
struct proc *p;

argaddr(0, &addr);

struct pinfo table[NPROC];
int ind=0;

for(p=proc; p<&proc[NPROC]; p++)
{
	acquire(&p->lock);
	if(p->state !=UNUSED){
		table[ind].pid=p->pid;
		table[ind].sz= p->sz;

		if(p->parent){
			table[ind].ppid= p->parent->pid;}
		else{
			table[ind].ppid=0;}
		
		safestrcpy(table[ind].name, p->name, 16);

	ind++;
}
	release(&p->lock);

}
if(copyout(myproc()->pagetable,
		myproc()->sz,
		addr,
		(char *)table,
		sizeof(table))<0)
{
	return -1;}

return ind;
}
