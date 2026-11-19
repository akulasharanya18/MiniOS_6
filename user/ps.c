#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    struct processinfo info[64];

    int n = getprocessinfo(info);

    if (n < 0) {
        printf("getprocessinfo failed\n");
        exit(1);
    }

    printf("PID\tPPID\tSTATE\tNAME\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%s\n",
               info[i].pid,
               info[i].ppid,
               info[i].state,
               info[i].name);
    }

    exit(0);
}
