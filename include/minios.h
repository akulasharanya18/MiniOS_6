
#ifndef MINIOS_H
#define MINIOS_H

/* Main shell */
void run_shell(void);
void show_help(void);

/* Member 2: System monitoring */
void show_system_info(void);

/* Member 3: Process management */
void process_demo(void);

/* Member 4: IPC and jobs */
void ipc_demo(void);
void submit_job(void);
void show_jobs(void);

/* Member 5: Threads and synchronization */
void start_workers(void);
void race_demo(void);
void semaphore_demo(void);
void producer_consumer_demo(void);

#endif

