#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_setpriority(void)
{
  int priority;

  if(argint(0, &priority) < 0)
    return -1;

  myproc()->priority = priority;
  return 0;
}

int
sys_pinfo(void)
{
  struct pinfo *table;
  int count = 0;
  struct proc *p;

  // Get the pointer to user-supplied pinfo array
  if(argptr(0, (char**)&table, NPROC * sizeof(struct pinfo)) < 0)
    return -1;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;

    table[count].pid   = p->pid;
    table[count].ppid  = p->parent ? p->parent->pid : 0;
    table[count].state = p->state;
    table[count].sz    = p->sz;
    safestrcpy(table[count].name, p->name, sizeof(p->name));
    count++;
  }

  release(&ptable.lock);
  return count;
}
