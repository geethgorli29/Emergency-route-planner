#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student1_graph.h"

Edge *graph[MAX_LOCATIONS];
Location locations[MAX_LOCATIONS];

int locationCount = 16;

// Initialize graph and locations
void initializeGraph(void)
{
    int i;

    for (i = 0; i < MAX_LOCATIONS; i++) {
        graph[i] = NULL;
    }

    locations[0].locationId = 0;
    strcpy(locations[0].locationName,
           "Dwaraka Nagar RTC Complex");

    locations[1].locationId = 1;
    strcpy(locations[1].locationName,
           "Visakhapatnam Railway Station");

    locations[2].locationId = 2;
    strcpy(locations[2].locationName,
           "Jagadamba Junction");

    locations[3].locationId = 3;
    strcpy(locations[3].locationName,
           "King George Hospital (KGH)");

    locations[4].locationId = 4;
    strcpy(locations[4].locationName,
           "RK Beach");

    locations[5].locationId = 5;
    strcpy(locations[5].locationName,
           "Kailasagiri");

    locations[6].locationId = 6;
    strcpy(locations[6].locationName,
           "Andhra University");

    locations[7].locationId = 7;
    strcpy(locations[7].locationName,
           "MVP Colony");

    locations[8].locationId = 8;
    strcpy(locations[8].locationName,
           "Rushikonda Beach");

    locations[9].locationId = 9;
    strcpy(locations[9].locationName,
           "Visakhapatnam Airport");

    locations[10].locationId = 10;
    strcpy(locations[10].locationName,
           "Gajuwaka");

    locations[11].locationId = 11;
    strcpy(locations[11].locationName,
           "Simhachalam Temple");

    locations[12].locationId = 12;
    strcpy(locations[12].locationName,
           "NAD Junction");

    locations[13].locationId = 13;
    strcpy(locations[13].locationName,
           "Gopalapatnam");

    locations[14].locationId = 14;
    strcpy(locations[14].locationName,
           "SevenHills Hospital");

    locations[15].locationId = 15;
    strcpy(locations[15].locationName,
           "CARE Hospitals, Ramnagar");
}

// Add an undirected road
void addRoad(int src, int dest,
             int distance, int traffic)
{
    Edge *newNode;

    // Validate road details
    if (src < 0 || src >= locationCount ||
        dest < 0 || dest >= locationCount ||
        distance <= 0 ||
        traffic < 1 || traffic > 3) {

        printf("Invalid road details!\n");
        return;
    }

    // Source to destination
    newNode = (Edge *)malloc(sizeof(Edge));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->dest = dest;
    newNode->distance = distance;
    newNode->trafficFactor = traffic;

    newNode->next = graph[src];
    graph[src] = newNode;

    // Destination to source
    newNode = (Edge *)malloc(sizeof(Edge));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->dest = src;
    newNode->distance = distance;
    newNode->trafficFactor = traffic;

    newNode->next = graph[dest];
    graph[dest] = newNode;
}

// Load all 27 project roads
void loadGraph(void)
{
    addRoad(0, 1, 4, 2);
    addRoad(0, 2, 3, 3);
    addRoad(0, 12, 7, 2);

    addRoad(1, 2, 4, 2);
    addRoad(1, 9, 14, 2);

    addRoad(2, 3, 2, 3);
    addRoad(2, 4, 2, 2);
    addRoad(2, 14, 3, 2);

    addRoad(3, 4, 2, 2);
    addRoad(3, 15, 3, 3);

    addRoad(4, 5, 6, 2);
    addRoad(4, 6, 5, 2);
    addRoad(4, 7, 5, 3);

    addRoad(5, 6, 7, 2);
    addRoad(5, 7, 5, 2);

    addRoad(6, 7, 4, 2);

    addRoad(7, 8, 8, 3);

    addRoad(8, 9, 18, 2);
    addRoad(8, 10, 20, 3);

    addRoad(9, 12, 8, 3);

    addRoad(10, 12, 12, 2);
    addRoad(10, 13, 10, 2);

    addRoad(11, 12, 6, 2);
    addRoad(11, 13, 5, 2);

    addRoad(12, 13, 4, 3);

    addRoad(13, 1, 8, 2);

    addRoad(14, 15, 4, 2);
}

// Display all locations
void displayLocations(void)
{
    int i;

    printf("\n========== VIZAG LOCATIONS ==========\n");

    for (i = 0; i < locationCount; i++) {
        printf("%2d - %s\n",
               locations[i].locationId,
               locations[i].locationName);
    }
}

// Display the complete road network
void displayGraph(void)
{
    int i;
    Edge *temp;

    printf("\n========== ROAD NETWORK ==========\n");

    for (i = 0; i < locationCount; i++) {

        temp = graph[i];

        printf("\n%d - %s:\n",
               locations[i].locationId,
               locations[i].locationName);

        while (temp != NULL) {

            printf("  -> %d - %s"
                   " | Distance: %d km"
                   " | Traffic: %d\n",

                   temp->dest,
                   locations[temp->dest].locationName,
                   temp->distance,
                   temp->trafficFactor);

            temp = temp->next;
        }
    }
}