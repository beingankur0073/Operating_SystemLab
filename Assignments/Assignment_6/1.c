// Write a program for solving the bounded-buffer problem (with one producer and one consumer) using
// semaphores (three semaphores, viz., mutex, full and empty).

#include <stdio.h>          // Standard I/O functions (e.g., printf)
#include <stdlib.h>         // General utilities, including memory allocation and process control
#include <unistd.h>         // POSIX operating system API, necessary for fork and process control
#include <time.h>           // Time functions, e.g., srand for random seeding
#include <string.h>         // String handling functions, included for potential future use
#include <fcntl.h>          // File control options, needed for shared memory flags
#include <stdbool.h>        // Boolean data type support
#include <semaphore.h>      // Semaphore handling functions (e.g., sem_init, sem_wait, sem_post)
#include <sys/ipc.h>        // IPC (Inter-Process Communication) definitions
#include <sys/stat.h>       // Mode constants for permissions in shared memory
#include <sys/types.h>      // Data type definitions used in system calls
#include <sys/mman.h>       // Memory management declarations (e.g., mmap for shared memory)
#include <sys/wait.h>       // Wait functions to control process synchronization
#include <sys/shm.h>        // Shared memory operations (e.g., shm_open, shm_unlink)

// Define shared semaphores and shared array for producer-consumer
sem_t *mutex;   // Semaphore for mutual exclusion
sem_t *full;    // Semaphore to track filled slots
sem_t *empty;   // Semaphore to track empty slots
int *arr;       // Shared buffer array for produced items
int i;          // Shared index variable for buffer tracking

// Producer function
void producer(void)
{
    int num = rand() % 1001;  // Generate random number to produce

    sem_wait(empty);          // Wait if no empty slots
    sem_wait(mutex);          // Enter critical section for mutual exclusion

    printf("Producer produces %d at index %d.\n", num, i);
    arr[i] = num;             // Add produced number to shared buffer

    i++;                      // Increment index for next item
    sem_post(mutex);          // Exit critical section
    sem_post(full);           // Signal that a slot is now full
}

// Consumer function
void consumer(void)
{
    sem_wait(full);           // Wait if no full slots
    sem_wait(mutex);          // Enter critical section

    i--;                      // Decrement index to retrieve item
    printf("Consumer consumes %d at index %d.\n", arr[i], i);

    sem_post(mutex);          // Exit critical section
    sem_post(empty);          // Signal that a slot is now empty
}

int main(void)
{
    srand(time(NULL));  // Seed random number generator

    // Create shared memory for buffer array (100 integers)
    int fd1 = shm_open("shared memory", O_CREAT | O_RDWR, 0666);
    ftruncate(fd1, 100 * sizeof(int));
    arr = mmap(0, 100 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    // Create shared memory for mutex semaphore
    int fd2 = shm_open("mutex", O_CREAT | O_RDWR, 0666);
    ftruncate(fd2, sizeof(sem_t));
    mutex = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    // Create shared memory for full semaphore
    int fd3 = shm_open("full", O_CREAT | O_RDWR, 0666);
    ftruncate(fd3, sizeof(sem_t));
    full = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd3, 0);

    // Create shared memory for empty semaphore
    int fd4 = shm_open("empty", O_CREAT | O_RDWR, 0666);
    ftruncate(fd4, sizeof(sem_t));
    empty = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd4, 0);

    // Initialize semaphores
    sem_init(mutex, 1, 1);   // Binary semaphore for mutual exclusion
    sem_init(empty, 1, 100); // Semaphore for empty slots, initialized to buffer size
    sem_init(full, 1, 0);    // Semaphore for full slots, initialized to 0

    // Fork process to create producer and consumer
    pid_t pid = fork();
    if (pid != 0)  // Parent process (producer)
    {
        for (i = 0; i < 100; i++)
        {
            producer();       // Produce items
        }
    }
    else           // Child process (consumer)
    {
        for (i = 0; i < 100; i++)
        {
            consumer();       // Consume items
        }
    }

    wait(NULL); // Wait for child process to complete

    // Cleanup shared memory and semaphores
    shm_unlink("shared memory");
    shm_unlink("mutex");
    shm_unlink("full");
    shm_unlink("empty");
    sem_destroy(mutex);
    sem_destroy(full);
    sem_destroy(empty);

    return 0; // Exit program
}

/*
 * Explanation of Key Functions:
 * - sem_init(): Initializes the semaphore with a specified initial value.
 * - sem_wait(): Decrements (locks) the semaphore, blocking if the value is 0.
 * - sem_post(): Increments (unlocks) the semaphore, signaling availability.
 * - shm_open(): Creates or opens a shared memory object.
 * - ftruncate(): Sets the size of a shared memory object.
 * - mmap(): Maps a shared memory object to the address space of the calling process.
 * - fork(): Creates a new process (child process).
 * - wait(): Waits for the termination of a child process.
 * - shm_unlink(): Removes a shared memory object.
 * - sem_destroy(): Destroys a semaphore to free resources.

 * How the Code Works:
 * 1. Shared memory and semaphores are created for a producer-consumer system.
 * 2. A producer function adds items to a shared buffer with synchronization using semaphores.
 * 3. A consumer function retrieves items from the buffer with mutual exclusion.
 * 4. Semaphores track full and empty slots and enforce mutual exclusion.
 * 5. At the end, shared memory and semaphores are released.

 * Notes:
 * - The shared memory buffer is limited to 100 items.
 * - `sem_wait()` and `sem_post()` manage full and empty slots in the buffer.
 * - The `mutex` semaphore provides mutual exclusion to prevent data races.
 */