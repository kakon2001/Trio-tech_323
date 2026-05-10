#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3){
    printf(2, "Usage: %s mask command [arg...]\n", argv[0]);
    exit();
  }

  int mask;
  if(strcmp(argv[1], "all") == 0) {
    mask = 2147483647; // Trace every syscall
  } else {
    mask = atoi(argv[1]);
  }

  if (trace(mask) < 0) {
    printf(2, "%s: trace failed\n", argv[0]);
    exit();
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[i-2] = 0;
  exec(nargv[0], nargv);
  exit();
}
