#include <stdio.h>

int getTrafficWeight(int trafficLevel)
{
    if (trafficLevel == 1)
        return 0;
    else if (trafficLevel == 2)
        return 2;
    else if (trafficLevel == 3)
        return 4;
    else
        return -1;
}

int calculateRoadCost(int distance, int trafficLevel)
{
    int weight = getTrafficWeight(trafficLevel);

    if (distance <= 0 || weight == -1)
        return -1;

    return distance + weight;
}

int main()
{
    int distance, trafficLevel, cost;

    printf("Enter road distance (km): ");
    scanf("%d", &distance);

    printf("Enter traffic level (1-Low, 2-Medium, 3-High): ");
    scanf("%d", &trafficLevel);

    cost = calculateRoadCost(distance, trafficLevel);

    if (cost == -1)
        printf("Invalid input.\n");
    else
        printf("Effective road cost = %d\n", cost);

    return 0;
}
