#include <stdio.h>
#include <limits.h>

#define MAX 10

void dijkstra(int graph[MAX][MAX], int n, int source, int destination)
{
    int distance[MAX], visited[MAX], parent[MAX];
    int i, count, u, v, min;

    for (i = 0; i < n; i++)
    {
        distance[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    distance[source] = 0;

    for (count = 0; count < n - 1; count++)
    {
        min = INT_MAX;
        u = -1;

        for (i = 0; i < n; i++)
        {
            if (!visited[i] && distance[i] < min)
            {
                min = distance[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        for (v = 0; v < n; v++)
        {
            if (!visited[v] && graph[u][v] > 0 &&
                distance[u] != INT_MAX &&
                distance[u] + graph[u][v] < distance[v])
            {
                distance[v] = distance[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    if (distance[destination] == INT_MAX)
    {
        printf("No route found.\n");
        return;
    }

    printf("Shortest distance = %d km\n", distance[destination]);

    {
        int route[MAX], length = 0, current = destination;

        while (current != -1)
        {
            route[length++] = current;
            current = parent[current];
        }

        printf("Route: ");
        for (i = length - 1; i >= 0; i--)
        {
            printf("%d", route[i]);
            if (i != 0)
                printf(" -> ");
        }
        printf("\n");
    }
}

int main()
{
    int graph[MAX][MAX] = {
        {0, 4, 0, 0},
        {4, 0, 5, 0},
        {0, 5, 0, 3},
        {0, 0, 3, 0}
    };

    dijkstra(graph, 4, 0, 3);
    return 0;
}
