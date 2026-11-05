
#include <stdio.h>
#include <string.h>
#include "minios.h"

void show_system_info(void)
{
    char line[256];
    FILE *fp;

    printf("\n======= SYSTEM INFORMATION =======\n");

    /* Read system uptime */

    fp = fopen("/proc/uptime", "r");

    if (fp != NULL)
    {
        if (fgets(line, sizeof(line), fp) != NULL)
            printf("System Uptime: %s seconds\n", line);

        fclose(fp);
    }
    else
    {
        printf("Cannot read /proc/uptime\n");
    }

    /* Read memory information */

    printf("\n------- MEMORY INFORMATION -------\n");

    fp = fopen("/proc/meminfo", "r");

    if (fp != NULL)
    {
        int count = 0;

        while (fgets(line, sizeof(line), fp) != NULL && count < 5)
        {
            printf("%s", line);
            count++;
        }

        fclose(fp);
    }
    else
    {
        printf("Cannot read /proc/meminfo\n");
    }

    printf("==================================\n");
}
