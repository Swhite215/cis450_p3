#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <assert.h>

double GetTime() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double)t.tv_sec + (double)t.tv_usec/1e6;
}

void Spin(int howlong) {
    double t = GetTime();
    while ((GetTime() - t) < (double)howlong)
	; // do nothing in loop
}

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

void ArriveIntersection(char *dir) {
    // Obtain Semaphore
    sem_wait(&semaphoreArrival);

    // Log Arriving Information
    
}

void ExitIntersection(char *dir) {
    // Obtain Semaphore
    sem_post(&semaphoreArrival);

    // Log Exiting Information
}

void CrossIntersection(char *dir) {

    if (strcmp(dir, "straight") == 0) {
        // Handle Straight Driving
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreWestToNorth);
        sem_wait(&semaphoreEastToWest);
        sem_wait(&semaphoreNorthToEast);
        sem_wait(&semaphoreEastToNorth);

        // Wait for 2 Seconds Using Spin
        Spin(2);

        // Log Crossing Information
        printf("GOING STRAIGHT");
        printf("\n");

        // Release
        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreWestToNorth);
        sem_post(&semaphoreEastToWest);
        sem_post(&semaphoreNorthToEast);
        sem_post(&semaphoreEastToNorth);

    } else if (strcmp(dir, "left") == 0) {
        // Handle Left Turn
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreWestToNorth);
        sem_wait(&semaphoreEastToWest);
        sem_wait(&semaphoreNorthToSouth);

        // Wait for 3 Seconds Using Spin
        Spin(3);

        // Log Crossing Information
        printf("TURNING LEFT");
        printf("\n");

        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreWestToNorth);
        sem_post(&semaphoreNorthToSouth);
        sem_post(&semaphoreEastToWest);
    } else {
        // Handle Right Turn
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreNorthToWest);

        // Wait for 1 Seconds Using Spin
        Spin(1);

        // Log Crossing Information
        printf("TURNING RIGHT");
        printf("\n");

        sem_post(&semaphoreNorthToWest);
        sem_post(&semaphoreWestToEast);
    }
}

void threadfunc(void* direction) {
    directions *dir = (directions *)direction;
    printf(&dir->dir_original);
    printf(&dir->dir_target);

    return;

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
    //char cars[8][10] = {"straight", "straight", "left", "straight", "right", "straight", "right", "left"};
    directions cars[8] = {{'N', 'N'}, {'N', 'N'}, {'N', 'W'}, {'S', 'S'}, {'S', 'E'}, {'N', 'N'}, {'E', 'N'}, {'W', 'N'}};
    
    for (int i = 0; i <= 7; i++) {
        // allocate thread and pass cars[i]

        directions *directionsPtr;
        directionsPtr = (directions *) malloc(sizeof(directions));
        directionsPtr->dir_original = cars[i].dir_original;
        directionsPtr->dir_target = cars[i].dir_target;

        // TODO = Call Based on Timing*
        pthread_t *mythread;
        mythread = (pthread_t *)malloc(sizeof(*mythread));
        //pthread_create(mythread, NULL,  (void*)threadfunc, (void*)cars[i]);
        pthread_create(mythread, NULL,  (void*)threadfunc, (void*)directionsPtr);

    }
    
    return 0;
}