#include <stdio.h>

int compareRoutes(int route1Cost, int route2Cost)
{
    if (route1Cost < route2Cost)
        return 1;
    else if (route2Cost < route1Cost)
        return 2;
    else
        return 0;
}

int main()
{
    int route1, route2, result;

    printf("Enter cost of Route 1: ");
    scanf("%d", &route1);

    printf("Enter cost of Route 2: ");
    scanf("%d", &route2);

    result = compareRoutes(route1, route2);

    if (result == 1)
        printf("Route 1 has lower cost.\n");
    else if (result == 2)
        printf("Route 2 has lower cost.\n");
    else
        printf("Both routes have equal cost.\n");

    return 0;
}
