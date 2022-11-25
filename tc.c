#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;

void threadfunc() {
    while (1) {
        sem_wait(&semaphore);
        printf("Hello from da thread!\n");
        sem_post(&semaphore);
        sleep(1);
    }
}

typedef struct _directions {
    char dir_original;
    char dir_target;
} directions;

int main(void) {
    
    // initialize semaphore, only to be used with threads in this process, set value to 1
    sem_init(&semaphore, 0, 1);

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
    // int cars[8] = {"straight", "straight", "left", "straight", "right", "straight", "right", "left"};
    directions cars[8] = {{'N', 'N'}, {'N', 'N'}, {'N', 'W'}, {'S', 'S'}, {'S', 'E'}, {'N', 'N'}, {'E', 'N'}, {'W', 'N'}};

    for (int i = 0; i < cars.length; i++) {
        // allocate thread and pass cars[i]

    }
    pthread_t *mythread;
    
    mythread = (pthread_t *)malloc(sizeof(*mythread));
    
    // start the thread
    printf("Starting thread, semaphore is unlocked.\n");
    pthread_create(mythread, NULL,  (void*)threadfunc, NULL);
    
    getchar();
    
    sem_wait(&semaphore);
    printf("Semaphore locked.\n");
    
    // do stuff with whatever is shared between threads
    getchar();
    
    printf("Semaphore unlocked.\n");
    sem_post(&semaphore);
    
    getchar();
    
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