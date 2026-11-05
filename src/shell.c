
#include <stdio.h>
#include "minios.h"

void show_help(void)
{
    printf("\n========== MINIOS COMMANDS ==========\n");
    printf("1. System Information\n");
    printf("2. Process Management\n");
    printf("3. IPC Demonstration\n");
    printf("4. Submit a Job\n");
    printf("5. View Jobs\n");
    printf("6. Worker Threads\n");
    printf("7. Race Condition\n");
    printf("8. Semaphore Demo\n");
    printf("9. Producer Consumer\n");
    printf("0. Exit\n");
    printf("====================================\n");
}

void run_shell(void)
{
    int choice;

    while (1)
    {
        show_help();

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Invalid input. Enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                show_system_info();
                break;

            case 2:
                process_demo();
                break;

            case 3:
                ipc_demo();
                break;

            case 4:
                submit_job();
                break;

            case 5:
                show_jobs();
                break;

            case 6:
                start_workers();
                break;

            case 7:
                race_demo();
                break;

            case 8:
                semaphore_demo();
                break;

            case 9:
                producer_consumer_demo();
                break;

            case 0:
                return;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

