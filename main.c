#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_DAYS_LEN 100
#define MAX_WORKERS 100

typedef struct
{
    char name[MAX_NAME_LEN];
    char days[MAX_DAYS_LEN];
} Worker;

void print_menu()
{
    printf("1. Add worker\n");
    printf("2. Modify worker\n");
    printf("3. Delete worker\n");
    printf("4. Print workers for a day\n");
    printf("5. Print all workers\n");
    printf("6. Exit\n");
}

void read_string(char *str, int max_len)
{
    fgets(str, max_len, stdin);
    str[strcspn(str, "\n")] = '\0';
}

void add_worker(Worker *workers, int *num_workers)
{
    if (*num_workers == MAX_WORKERS)
    {
        printf("Maximum number of workers reached\n");
        return;
    }
    printf("Enter worker name: ");
    read_string(workers[*num_workers].name, MAX_NAME_LEN);
    printf("Enter worker days: ");
    read_string(workers[*num_workers].days, MAX_DAYS_LEN);
    *num_workers += 1;
    printf("Worker added successfully\n");
}

void modify_worker(Worker *workers, int *num_workers)
{
    printf("Enter worker name: ");
    char name[MAX_NAME_LEN];
    read_string(name, MAX_NAME_LEN);
    int index = -1;
    for (int i = 0; i < *num_workers; i++)
    {
        if (strcmp(workers[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        printf("Worker not found\n");
        return;
    }
    printf("Enter new worker name: ");
    read_string(workers[index].name, MAX_NAME_LEN);
    printf("Enter new worker days: ");
    read_string(workers[index].days, MAX_DAYS_LEN);
    printf("Worker modified successfully\n");
}

void delete_worker(Worker *workers, int *num_workers)
{
    printf("Enter worker name: ");
    char name[MAX_NAME_LEN];
    read_string(name, MAX_NAME_LEN);
    int index = -1;
    for (int i = 0; i < *num_workers; i++)
    {
        if (strcmp(workers[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        printf("Worker not found\n");
        return;
    }
    for (int i = index; i < *num_workers - 1; i++)
    {
        workers[i] = workers[i + 1];
    }
    *num_workers -= 1;
    printf("Worker deleted successfully\n");
}

void print_workers_for_day(Worker *workers, int num_workers)
{
    printf("Enter day: ");
    char day[MAX_NAME_LEN];
    read_string(day, MAX_NAME_LEN);
    printf("Workers available on %s:\n", day);
    for (int i = 0; i < num_workers; i++)
    {
        char *token = strtok(workers[i].days, " ");
        while (token != NULL)
        {
            if (strcmp(token, day) == 0)
            {
                printf("- %s\n", workers[i].name);
                break;
            }
            token = strtok(NULL, " ");
        }
    }
}

void print_all_workers(Worker *workers, int num_workers)
{
    printf("All workers:\n");
    for (int i = 0; i < num_workers; i++)
    {
        printf("- Name: %s, Days: %s\n", workers[i].name, workers[i].days);
    }
}

void save_workers(Worker *workers, int num_workers, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Failed to open file '%s' for writing\n", filename);
        return;
    }
    for (int i = 0; i < num_workers; i++)
    {
        fprintf(fp, "%s,%s\n", workers[i].name, workers[i].days);
    }
    fclose(fp);
    printf("Workers saved to file '%s'\n", filename);
}

void load_workers(Worker *workers, int *num_workers, char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error: could not open file %s for reading\n", filename);
        return;
    }
    char line[MAX_NAME_LEN + MAX_DAYS_LEN + 2];
    *num_workers = 0;
    while (fgets(line, sizeof(line), f) != NULL)
    {
        char name[MAX_NAME_LEN], days[MAX_DAYS_LEN];
        if (sscanf(line, "%[^,],%[^\n]", name, days) != 2)
        {
            continue;
        }
        strncpy(workers[*num_workers].name, name, MAX_NAME_LEN);
        strncpy(workers[*num_workers].days, days, MAX_DAYS_LEN);
        (*num_workers)++;
    }
    fclose(f);
}

int main()
{
    int choice;
    int num_workers = 0;
    Worker workers[MAX_WORKERS];

    load_workers(workers, &num_workers, "workers.txt");

    while (1)
    {
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            add_worker(workers, &num_workers);
            break;
        case 2:
            modify_worker(workers, &num_workers);
            break;
        case 3:
            delete_worker(workers, &num_workers);
            break;
        case 4:
            print_workers_for_day(workers, num_workers);
            break;
        case 5:
            print_all_workers(workers, num_workers);
            break;
        case 6:
            printf("Exiting program\n");
            save_workers(workers, num_workers, "workers.txt");
            exit(0);
        default:
            printf("Invalid choice\n");
            break;
        }
    }
    return 0;
}
