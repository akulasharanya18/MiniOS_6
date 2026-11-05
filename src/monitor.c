#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "minios.h"

static long read_kb_value(const char *key)
{
    FILE *fp = fopen("/proc/meminfo", "r");

    char name[64];
    long value;
    char unit[32];

    if (fp == NULL)
    {
        return -1;
    }

    while (fscanf(fp, "%63s %ld %31s", name, &value, unit) == 3)
    {
        if (strcmp(name, key) == 0)
        {
            fclose(fp);
            return value;
        }
    }

    fclose(fp);

    return -1;
}

void show_system_info(void)
{
    FILE *fp = fopen("/proc/uptime", "r");

    double uptime = 0.0;
    double idle = 0.0;

    long total_memory = read_kb_value("MemTotal:");
    long available_memory = read_kb_value("MemAvailable:");

    printf("\n");
    printf("============= SYSTEM INFORMATION =============\n");

    if (fp != NULL)
    {
        if (fscanf(fp, "%lf %lf", &uptime, &idle) == 2)
        {
            printf("System uptime    : %.2f seconds\n", uptime);
            printf("CPU idle time    : %.2f seconds\n", idle);
        }

        fclose(fp);
    }
    else
    {
        perror("/proc/uptime");
    }

    if (total_memory >= 0)
    {
        printf("Total memory     : %ld kB\n", total_memory);
    }

    if (available_memory >= 0)
    {
        printf("Available memory : %ld kB\n", available_memory);
    }

    printf("===============================================\n\n");
}

static int is_number(const char *s)
{
    if (*s == '\0')
    {
        return 0;
    }

    for (; *s != '\0'; s++)
    {
        if (!isdigit((unsigned char)*s))
        {
            return 0;
        }
    }

    return 1;
}

void show_processes(void)
{
    DIR *dir = opendir("/proc");

    struct dirent *entry;

    int shown = 0;

    if (dir == NULL)
    {
        perror("opendir /proc");
        return;
    }

    printf("\n");
    printf("================ PROCESSES ====================\n");
    printf("%-10s %-25s\n", "PID", "COMMAND");
    printf("-----------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL && shown < 15)
    {
        if (is_number(entry->d_name))
        {
            char path[512];

            char name[128] = "?";

            FILE *fp;

            snprintf(
                path,
                sizeof(path),
                "/proc/%s/comm",
                entry->d_name
            );

            fp = fopen(path, "r");

            if (fp != NULL)
            {
                if (fgets(name, sizeof(name), fp) != NULL)
                {
                    name[strcspn(name, "\n")] = '\0';
                }

                fclose(fp);
            }

            printf(
                "%-10s %-25s\n",
                entry->d_name,
                name
            );

            shown++;
        }
    }

    closedir(dir);

    printf("-----------------------------------------------\n");
    printf("Showing %d processes from /proc.\n", shown);
    printf("===============================================\n\n");
}
