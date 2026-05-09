#include "types.h"
#include "user.h"
#include "proc.h"
#include "param.h"

// State names matching enum procstate order:
// UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
static char *states[] = {
  "UNUSED  ",
  "EMBRYO  ",
  "SLEEPING",
  "RUNNABLE",
  "RUNNING ",
  "ZOMBIE  "
};

int
main(void)
{
  struct pinfo table[NPROC];
  int n, i;

  n = pinfo(table);
  if(n < 0){
    printf(2, "ps: pinfo failed\n");
    exit();
  }

  printf(1, "%-6s %-6s %-10s %-10s %s\n",
         "PID", "PPID", "STATE", "SIZE", "NAME");
  printf(1, "----------------------------------------------\n");

  for(i = 0; i < n; i++){
    char *state;
    if(table[i].state >= 0 && table[i].state < 6)
      state = states[table[i].state];
    else
      state = "???     ";

    printf(1, "%-6d %-6d %-10s %-10d %s\n",
           table[i].pid,
           table[i].ppid,
           state,
           table[i].sz,
           table[i].name);
  }

  exit();
}
