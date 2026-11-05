
#include <stdio.h>
#include <string.h>
#include "minios.h"

#define MAX_JOBS 10
#define JOB_NAME_LENGTH 100

static char jobs[MAX_JOBS][JOB_NAME_LENGTH];
static int job_count = 0;

void submit_job(void)
{
    char name[JOB_NAME_LENGTH];

    if (job_count >= MAX_JOBS)
    {
        printf("\nJob list is full!\n");
        return;
    }

    printf("\nEnter job name: ");

    if (fgets(name, sizeof(name), stdin) == NULL)
    {
        printf("Unable to read job name.\n");
        return;
    }

    name[strcspn(name, "\n")] = '\0';

    if (name[0] == '\0')
    {
        printf("Job name cannot be empty.\n");
        return;
    }

    strcpy(jobs[job_count], name);

    job_count++;

    printf("\nJob submitted successfully!\n");
    printf("Job ID: %d\n", job_count);
}

void show_jobs(void)
{
    printf("\n========== JOB MANAGEMENT ==========\n");

    if (job_count == 0)
    {
        printf("No jobs submitted yet.\n");
    }
    else
    {
        for (int i = 0; i < job_count; i++)
        {
            printf("Job ID: %d | Name: %s | Status: SUBMITTED\n",
                   i + 1, jobs[i]);
        }
    }

    printf("====================================\n");
}

