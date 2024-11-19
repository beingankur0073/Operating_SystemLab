// Write a program where two processes (a producer and a consumer) have a shared memory to store a
// buffer containing hundred integer elements. Consider a variable "counter" that keeps track of the number
// of filled buffer-locations. Initially, counter is set to 0. It is incremented when the producer produces and
// puts a new element, and it is decremented by the consumer consumes an element. Implement the Peterson’s
// solution in this scenario to provide synchronization between these two processes.



#include <stdio.h>          // Standard I/O functions (e.g., printf, scanf)
#include <stdlib.h>         // General utilities, including memory allocation (e.g., malloc, free) and process control (e.g., exit)
#include <unistd.h>         // POSIX operating system API (e.g., fork, close, read, write)
#include <time.h>           // Time functions (e.g., time, srand)
#include <string.h>         // String handling functions (e.g., memset, memcpy)
#include <fcntl.h>          // File control options (e.g., O_CREAT, O_RDWR for opening shared memory)
#include <stdbool.h>        // Support for boolean data type (true/false)
#include <math.h>           // Mathematical functions (not used in this specific program, but included for potential future use)
#include <sys/types.h>      // Definitions for data types used in system calls
#include <sys/mman.h>       // Memory management declarations (e.g., mmap for memory mapping)
#include <sys/ipc.h>        // Definitions for inter-process communication (IPC)
#include <sys/shm.h>        // Shared memory definitions (e.g., shm_open, shm_unlink)
#include <sys/wait.h>       // Declarations for waiting for process termination (e.g., wait)

#define buffer_size 100 // Define the buffer size for the shared memory

// Global pointers for shared variables
bool* flag;  // Array of flags for mutual exclusion
int *n;      // Pointer for the number of filled spaces
int *arr;    // Pointer for the shared array (buffer)
int *turn;   // Pointer for turn variable
int *count;  // Pointer for the current count of items in the buffer

// Function for the producer process
void producer(void)
{
    int num = rand() % 1001; // Generate a random number between 0 and 1000
    flag[0] = true;           // Set the flag for producer
    *turn = 1;                // Set turn to consumer

    // Wait if consumer is working or buffer is full
    while(flag[1] && *turn == 1 || *count >= buffer_size);

    arr[*count] = num; // Place the produced number into the buffer
    printf("Producer produces %d at index %d.\n", num, *count);
    *count = (*count + 1); // Increment the count of items
    *n = (*n + 1);         // Increment the number of filled spaces

    flag[0] = false;      // Clear the producer flag
}

// Function for the consumer process
void consumer(void)
{
    flag[1] = true;       // Set the flag for consumer
    *turn = 0;            // Set turn to producer

    // Wait if producer is working or buffer is empty
    while(flag[0] && *turn == 0 || *n == 0);

    int num = arr[*count - *n]; // Retrieve the number from the buffer
    printf("Consumer consumes %d at index %d.\n", num, *count - *n);
    *n = (*n - 1);              // Decrement the number of filled spaces

    flag[1] = false;           // Clear the consumer flag
}

int main(void)
{
    srand(time(NULL)); // Seed the random number generator

    // Create shared memory for the buffer
    int fd1 = shm_open("shared memory", O_CREAT | O_RDWR, 0666);
    ftruncate(fd1, buffer_size * sizeof(int)); // Set the size of shared memory
    arr = mmap(0, buffer_size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    // Create shared memory for turn variable
    int fd2 = shm_open("turn", O_CREAT | O_RDWR, 0666);
    ftruncate(fd2, 1 * sizeof(int));
    turn = mmap(0, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    *turn = 0; // Initialize turn variable

    // Create shared memory for count variable
    int fd3 = shm_open("count", O_CREAT | O_RDWR, 0666);
    ftruncate(fd3, 1 * sizeof(int));
    count = mmap(0, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd3, 0);
    *count = 0; // Initialize count

    // Create shared memory for the number of filled spaces
    int fd4 = shm_open("number of filled space", O_CREAT | O_RDWR, 0666);
    ftruncate(fd4, 1 * sizeof(int));
    n = mmap(0, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd4, 0);
    *n = 0; // Initialize number of filled spaces

    // Create shared memory for flags
    int fd5 = shm_open("flag", O_CREAT | O_RDWR, 0666);
    ftruncate(fd5, 2 * sizeof(bool));
    flag = mmap(0, 2 * sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED, fd5, 0);
    flag[0] = false; // Initialize producer flag
    flag[1] = false; // Initialize consumer flag

    // Fork the process to create producer and consumer
    pid_t id = fork();
    if (id != 0) // Parent process (producer)
    {
        for (int i = 0; i < 100; i++)
        {
            producer(); // Call producer function
        }
    }
    else // Child process (consumer)
    {
        for (int i = 0; i < 100; i++)
        {
            consumer(); // Call consumer function
        }
    }

    wait(NULL); // Wait for the child process to finish
    // Unlink shared memory
    shm_unlink("shared memory");
    shm_unlink("turn");
    shm_unlink("count");
    shm_unlink("number of filled space");
    shm_unlink("flag");
    
    return 0; // Exit the program
}

/*
 * Explanation of Key Functions:
 * - shm_open(): Creates a new shared memory object or opens an existing one.
 * - ftruncate(): Sets the size of the shared memory object.
 * - mmap(): Maps the shared memory object into the address space of the process.
 * - fork(): Creates a new process by duplicating the calling process.
 * - wait(): Suspends execution of the calling process until one of its children terminates.

 * How the Code Works:
 * 1. The program sets up shared memory for the producer-consumer problem.
 * 2. Global pointers are declared to manage shared data.
 * 3. The producer generates random numbers and adds them to the buffer.
 * 4. The consumer retrieves numbers from the buffer.
 * 5. Flags are used to manage synchronization between producer and consumer.
 * 6. After producing and consuming, shared memory is unlinked to free resources.

 * Notes:
 * - The program relies on busy-waiting (spin-lock) for synchronization, which is simple but may not be efficient.
 * - Error checking for system calls is omitted for clarity; in production code, it's important to handle errors properly.
 * - The shared memory segments are cleaned up at the end to avoid memory leaks.
 */