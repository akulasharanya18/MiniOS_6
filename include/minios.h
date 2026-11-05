#ifndef MINIOS_H
#define MINIOS_H

#include <stddef.h>

#define MINIOS_MAX_INPUT 256
#define MAX_ARGS 32
#define QUEUE_SIZE 5
#define WORKER_COUNT 3

void show_help(void);
void show_system_info(void);
void show_processes(void);

int run_program(char **args);

void submit_job(const char *job);

void show_workers_demo(void);
void show_race_demo(void);
void show_sync_demo(void);
void show_queue_demo(void);

#endif
