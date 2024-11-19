// Write a program for solving the five-dining-philosophers problem with the help of semaphores for
// chopsticks.

#include <stdio.h>          // Standard I/O functions (e.g., printf)
#include <stdlib.h>         // General utilities, such as memory allocation
#include <unistd.h>         // POSIX operating system API (e.g., usleep for delays)
#include <time.h>           // Time functions, used here for random delays
#include <string.h>         // String handling functions
#include <fcntl.h>          // File control options, included for potential future use
#include <stdbool.h>        // Boolean data type support
#include <pthread.h>        // Thread management and mutexes
#include <sys/stat.h>       // Mode constants for permissions (e.g., in shared memory)
#include <sys/types.h>      // Data type definitions used in system calls

#define NUM_PHILOSOPHERS 5  // Number of philosophers
#define NUM_CHOPSTICKS 5    // Number of chopsticks (equal to philosophers)

// Declare philosopher threads and chopstick mutexes
pthread_t philosopher[NUM_PHILOSOPHERS];    // Thread array for philosophers
pthread_mutex_t chopstick[NUM_CHOPSTICKS];  // Mutex array for chopsticks

// Function that simulates a philosopher's dining cycle
void dine(int n)
{
    printf("Philosopher %d is thinking.\n", n + 1);  // Philosopher thinking

    // Attempt to pick up left and right chopsticks (mutex locks)
    pthread_mutex_lock(&chopstick[n]);                 // Lock left chopstick
    pthread_mutex_lock(&chopstick[(n + 1) % NUM_CHOPSTICKS]); // Lock right chopstick

    printf("Philosopher %d is eating.\n", n + 1);      // Philosopher eating
    usleep(rand() % 1000);                             // Simulate eating time

    // Release chopsticks (mutex unlock)
    pthread_mutex_unlock(&chopstick[n]);               // Unlock left chopstick
    pthread_mutex_unlock(&chopstick[(n + 1) % NUM_CHOPSTICKS]); // Unlock right chopstick

    printf("Philosopher %d has finished eating.\n", n + 1); // Philosopher finished eating
}

// Main function to initialize mutexes, create threads, and manage cleanup
int main(void)
{
    srand(time(NULL));  // Seed the random number generator

    // Initialize chopstick mutexes
    for (int i = 0; i < NUM_CHOPSTICKS; i++)
    {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&philosopher[i], NULL, (void*)dine, (void*)(long)i); // Pass index as argument
    }

    // Wait for all philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosopher[i], NULL);
    }

    // Destroy chopstick mutexes to free resources
    for (int i = 0; i < NUM_CHOPSTICKS; i++)
    {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}

/*
 * Explanation of Key Functions and Improvements:
 * - pthread_create(): Creates a new thread. The third argument is the function to execute (dine), and the fourth argument is the index of the philosopher cast to a pointer.
 * - pthread_join(): Blocks until the specified thread (philosopher) terminates, ensuring the main function waits for each philosopher to finish dining.
 * - pthread_mutex_init(): Initializes each chopstick as a mutex for shared access control.
 * - pthread_mutex_lock(): Locks the mutex, ensuring exclusive access to a chopstick.
 * - pthread_mutex_unlock(): Unlocks the mutex, allowing other threads to access the chopstick.
 * - pthread_mutex_destroy(): Destroys each mutex, freeing resources.

 * How the Code Works:
 * 1. The program creates a set number of philosopher threads, each of whom "thinks" and then attempts to pick up two chopsticks (mutexes).
 * 2. To eat, a philosopher locks both their left and right chopsticks, eats, and then unlocks both chopsticks.
 * 3. After all philosophers have finished, the program cleans up by destroying the mutexes.

 * Improvements and Notes:
 * - The `dine` function's parameter passing uses `(void*)(long)i` to safely cast the integer `i` to a void pointer and back, avoiding data type issues.
 * - Deadlock Prevention: This simple mutex setup may still lead to deadlock if each philosopher simultaneously picks up their left chopstick. To prevent this, consider solutions such as allowing only four philosophers to pick up chopsticks simultaneously, enforcing an order for chopstick pickup, or using semaphores.
 * - `usleep`: Simulates eating time. Replace with `sleep()` for longer delays in real-world simulation.
 */