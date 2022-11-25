#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphoreArrival;
sem_t semaphoreWestToEast;
sem_t semaphoreEastToSouth;
sem_t semaphoreWestToNorth;
sem_t semaphoreNorthToSouth;
sem_t semaphoreEastToWest;
sem_t semaphoreNorthToEast;
sem_t semaphoreEastToNorth;
sem_t semaphoreNorthToWest;

typedef struct _directions {
    char dir_original;
    char dir_target;
} directions;

directions carDirections[8] = {{'N', 'N'}, {'N', 'N'}, {'N', 'W'}, {'S', 'S'}, {'S', 'E'}, {'N', 'N'}, {'E', 'N'}, {'W', 'N'}};

void ArriveIntersection(char *dir) {
    sem_wait(&semaphoreArrival);
}

void ExitIntersection(char *dir) {
    sem_post(&semaphoreArrival);
}

void CrossIntersection(char *dir) {
    if (strcmp(dir, "straight") == 0) {
        // Handle Straight Driving
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreNorthToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreEastToWest);
        sem_wait(&semaphoreEastToNorth);
        sem_wait(&semaphoreWestToNorth);

        // WAIT for Seconds Using Spin
        printf("GOING STRAIGHT");
        printf("\n");

        // Release
        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreNorthToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreEastToWest);
        sem_post(&semaphoreEastToNorth);
        sem_post(&semaphoreWestToNorth);

    } else if (strcmp(dir, "left") == 0) {
        // Handle Left Turn
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreWestToNorth);
        sem_wait(&semaphoreNorthToSouth);
        sem_wait(&semaphoreEastToWest);

        printf("LEFT");
        printf("\n");

        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreWestToNorth);
        sem_post(&semaphoreNorthToSouth);
        sem_post(&semaphoreEastToWest);
    } else {
        // Handle Right Turn
        sem_wait(&semaphoreNorthToWest);
        sem_wait(&semaphoreWestToEast);

        printf("RIGHT");
        printf("\n");

        sem_post(&semaphoreNorthToWest);
        sem_post(&semaphoreWestToEast);
    }
}

void threadfunc(void* direction) {
    char *dir = direction;

    // Arrival Semaphore
    ArriveIntersection(dir);

    // Cross Semaphores
    CrossIntersection(dir);

    // Exit Sempahore
    ExitIntersection(dir);
}

int main(void) {
    
    // initialize semaphore, only to be used with threads in this process, set value to 1
    sem_init(&semaphoreArrival, 0, 1);
    sem_init(&semaphoreWestToEast, 0, 1);
    sem_init(&semaphoreEastToSouth, 0, 1);
    sem_init(&semaphoreWestToNorth, 0, 1);
    sem_init(&semaphoreNorthToSouth, 0, 1);
    sem_init(&semaphoreEastToWest, 0, 1);
    sem_init(&semaphoreNorthToEast, 0, 1);
    sem_init(&semaphoreEastToNorth, 0, 1);
    sem_init(&semaphoreNorthToWest, 0, 1);

    // Locks - Turning Left
    // Arriving - First Car
    // Crossing - Traffic Crossing from West to East - DUPLICATE
    // Crossing - Traffic Crossing from East to South - DUPLICATE
    // Crossing - Traffic Crossing from West to North - DUPLICATE
    // Crossing - Traffic Crossing from North to South
    // Crossing - Traffic Crossing from East to West - DUPLICATE

    // Locks - Straight
    // Arriving - First Car
    // Crossing - Traffic Crossing from West to East - DUPLICATE
    // Crossing - Traffic Crossing from North to East
    // Crossing - Traffic Crossing from East to South - DUPLICATE
    // Crossing - Traffic Crossing from East to West - DUPLICATE
    // Crossing - Traffic Crossing from East to North
    // Crossing - Traffic Crossing from West to North - DUPLICATE

    // Locks Turning - Right
    // Arriving - First Car
    // Crossing - Traffic Crossing from North to West
    // Crossing - Traffic Crossing from West to East - DUPLICATE

    // Queue of Vehicles - 
    char cars[8][10] = {"straight", "straight", "left", "straight", "right", "straight", "right", "left"};

    for (int i = 0; i < 8; i++) {
        // allocate thread and pass cars[i]

        // TODO = Call Based on Timing*
        pthread_t *mythread;
        mythread = (pthread_t *)malloc(sizeof(*mythread));
        pthread_create(mythread, NULL,  (void*)threadfunc, (void*)cars[i]);
    }
    
    return 0;
}