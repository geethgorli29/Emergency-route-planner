#include <stdio.h>

#define MAX 10

typedef struct
{
    int id;
    int priority;
} EmergencyRequest;

EmergencyRequest queue[MAX];
int size = 0;

void insert(int id, int priority)
{
    int i;

    if (size == MAX)
    {
        printf("Priority queue is full.\n");
        return;
    }

    i = size - 1;

    while (i >= 0 && queue[i].priority < priority)
    {
        queue[i + 1] = queue[i];
        i--;
    }

    queue[i + 1].id = id;
    queue[i + 1].priority = priority;
    size++;
}

void removeHighestPriority()
{
    int i;

    if (size == 0)
    {
        printf("Priority queue is empty.\n");
        return;
    }

    printf("Processing request ID: %d\n", queue[0].id);

    for (i = 1; i < size; i++)
        queue[i - 1] = queue[i];

    size--;
}

void display()
{
    int i;

    printf("Priority Queue:\n");

    for (i = 0; i < size; i++)
        printf("Request ID: %d  Priority: %d\n", queue[i].id, queue[i].priority);
}

int main()
{
    insert(101, 1);
    insert(102, 3);
    insert(103, 2);

    display();
    removeHighestPriority();
    display();

    return 0;
}
