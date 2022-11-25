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


void threadfunc(void* direction) {
    char *dir = direction;
    printf(dir);
    printf("\n");
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
    //directions cars[8] = {{'N', 'N'}, {'N', 'N'}, {'N', 'W'}, {'S', 'S'}, {'S', 'E'}, {'N', 'N'}, {'E', 'N'}, {'W', 'N'}};

    for (int i = 0; i < 8; i++) {
        // allocate thread and pass cars[i]
        pthread_t *mythread;
        mythread = (pthread_t *)malloc(sizeof(*mythread));
        pthread_create(mythread, NULL,  (void*)threadfunc, (void*)cars[i]);
    }
    
    return 0;
}

    // Arriving - First Car
    // Crossing - Traffic Crossing from Left to Right
    // Crossing - Traffic Turning from Right to Left
    // Crossing - Traffic Turning from Left to Straight
    // Crossing - Traffic Coming Straight On
    // Crossing - Traffic Coming from Right to Left

    // Locks - Going Straight
    // Arriving - First Car
    // Crossing - Traffic Crossing from Left to Right
    // Crossing - Traffic Turning from Right to Left
    // Crossing - Traffic Turning from Left to Straight
    // Crossing - Traffic Coming Straight On
    // Crossing - Traffic Coming from Right to Left

    
       // Crossing - Traffic Crossing from West to East