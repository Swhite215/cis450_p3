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
    float arrival_time;
    int cid;
} directions;

void ArriveIntersection(directions *dir) {

    // Obtain Semaphore
    sem_wait(&semaphoreArrival);

    // Log Arriving Information
    printf("\nTime %.1f: Car %d (->%c ->%c) arriving", dir->arrival_time, dir->cid, dir->dir_original, dir->dir_target);

    // Check Crossing Here - Handle Special Situations

    // Handle Allocation of Semphores
     if (dir->dir_original == dir->dir_target) {
        // Handle Straight Driving
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreWestToNorth);
        sem_wait(&semaphoreEastToWest);
        sem_wait(&semaphoreNorthToEast);
        sem_wait(&semaphoreEastToNorth);

    }  else if ((dir->dir_original == 'N' && dir->dir_original == 'E') || (dir->dir_original == 'S' && dir->dir_original == 'W') || (dir->dir_original == 'W' && dir->dir_original == 'N') || (dir->dir_original == 'E' && dir->dir_original == 'S')) {
        // Handle Left Turn        
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreEastToSouth);
        sem_wait(&semaphoreWestToNorth);
        sem_wait(&semaphoreEastToWest);
        sem_wait(&semaphoreNorthToSouth);
    } else {
        // Handle Right Turn
        sem_wait(&semaphoreWestToEast);
        sem_wait(&semaphoreNorthToWest);
    }

    // Special Situations
    // Straight North to North or South to South can cross at same time.
    // Straight East to East and West to West can cross at same time.
    
}

void ExitIntersection(directions *dir) {

    // Log Exiting Information
    printf("\nEXITING\n");
    printf("\nTime %.1f: Car %d (->%c ->%c)                  exiting", dir->arrival_time, dir->cid, dir->dir_original, dir->dir_target);

    // Handle Release of Semaphores
     if (dir->dir_original == dir->dir_target) {
        // Handle Straight Driving
        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreWestToNorth);
        sem_post(&semaphoreEastToWest);
        sem_post(&semaphoreNorthToEast);
        sem_post(&semaphoreEastToNorth);

    }  else if ((dir->dir_original == 'N' && dir->dir_original == 'E') || (dir->dir_original == 'S' && dir->dir_original == 'W') || (dir->dir_original == 'W' && dir->dir_original == 'N') || (dir->dir_original == 'E' && dir->dir_original == 'S')) {
        // Handle Left Turn        
        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreEastToSouth);
        sem_post(&semaphoreWestToNorth);
        sem_post(&semaphoreEastToWest);
        sem_post(&semaphoreNorthToSouth);
    } else {
        // Handle Right Turn
        sem_post(&semaphoreWestToEast);
        sem_post(&semaphoreNorthToWest);
    }

    // Final Action Release Semaphore
    sem_post(&semaphoreArrival);
}

void CrossIntersection(directions *dir) {

    if (dir->dir_original == dir->dir_target) {
        printf("\nSTRAIGHT\n");

        // Spin 
        Spin(2);

        // Log Straight Turn
        printf("\nTime %.1f: Car %d (->%c ->%c)          crossing", dir->arrival_time, dir->cid, dir->dir_original, dir->dir_target);

    }  else if ((dir->dir_original == 'N' && dir->dir_original == 'E') || (dir->dir_original == 'S' && dir->dir_original == 'W') || (dir->dir_original == 'W' && dir->dir_original == 'N') || (dir->dir_original == 'E' && dir->dir_original == 'S')) {
        printf("\nLEFT\n");
        
        // Spin 
        Spin(3);

        // Log LEft Turn
        printf("\nTime %.1f: Car %d (->%c ->%c)          crossing", dir->arrival_time, dir->cid, dir->dir_original, dir->dir_target);
    } else {
        printf("\nRIGHT\n");

        //Spin
        Spin(1);

        // Log Right Turn
        printf("\nTime %.1f: Car %d (->%c ->%c)          crossing", dir->arrival_time, dir->cid, dir->dir_original, dir->dir_target);
    }

}

void threadfunc(void* direction) {
    directions *dir = (directions *)direction;
    
    // Arrival Semaphore
    ArriveIntersection(dir);

    printf("\nPRE CROSSING\n");

    // Cross Semaphores
    CrossIntersection(dir);

    printf("\nPOST CROSSING\n");

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
    directions cars[8] = {{'N', 'N', 1.1, 0}, {'N', 'N', 2.0, 1}, {'N', 'W', 3.3, 2}, {'S', 'S', 3.5, 3}, {'S', 'E', 4.2, 4}, {'N', 'N', 4.4, 5}, {'E', 'N', 5.7, 6}, {'W', 'N', 5.9, 7}};
    
    for (int i = 0; i < 8; i++) {

        // Sleep
        switch(i){
            case 0:
                usleep(1100);
                break;
            case 1:
                usleep(900);
                break;
            case 2:
                usleep(1300);
                break;
            case 3:
                usleep(200);
                break;
            case 4:
                usleep(700);
                break;
            case 5:
                usleep(200);
                break;
            case 6:
                usleep(1300);
                break;
            case 7:
                 usleep(200);
                break;
            default:
                break;
        }

        // Allocate Directions Object
        directions *directionsPtr;
        directionsPtr = (directions *) malloc(sizeof(directions));
        directionsPtr->dir_original = cars[i].dir_original;
        directionsPtr->dir_target = cars[i].dir_target;
        directionsPtr->arrival_time = cars[i].arrival_time;
        directionsPtr->cid = cars[i].cid;

        // Allocate Thread and Initialize
        pthread_t *mythread;
        mythread = (pthread_t *)malloc(sizeof(*mythread));
        pthread_create(mythread, NULL,  (void*)threadfunc, (void*)directionsPtr);

    }
    
    return 0;
}