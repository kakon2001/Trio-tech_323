#include "types.h"
#include "param.h"    // must come before proc.h so NPROC is defined
#include "mmu.h"      // provides struct taskstate and NSEGS for proc.h
#include "user.h"
#include "proc.h"

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

struct pinfo table[NPROC];

int
main(void)
{
  int n, i, j;

  n = pinfo(table);
  if(n < 0){
    printf(2, "ps: pinfo failed\n");
    exit();
  }

  // Header
  printf(1, "PID\tPPID\tSTATE\tSIZE\tRUNS\tTICKS\tNAME\tCHILDREN\n");
  printf(1, "------------------------------------------------------------------------\n");

  for(i = 0; i < n; i++){
    char *state;
    if(table[i].state >= 0 && table[i].state < 6)
      state = states[table[i].state];
    else
      state = "???     ";

    // Print fixed columns using tabs for basic alignment
    printf(1, "%d\t%d\t%s\t%d\t%d\t%d\t%s\t",
           table[i].pid,
           table[i].ppid,
           state,
           table[i].sz,
           table[i].run_count,
           table[i].runtime,
           table[i].name);

    // Print children list
    if(table[i].nchildren == 0){
      printf(1, "none");
    } else {
      for(j = 0; j < table[i].nchildren; j++){
        if(j > 0) printf(1, ",");
        printf(1, "%d", table[i].children[j]);
      }
    }
    printf(1, "\n");
  }

  exit();
}
