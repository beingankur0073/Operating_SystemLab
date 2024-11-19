// Write a program where two processes (a producer and a consumer) share a shared memory. The
// producer process writes the first ten positive integers in the shared memory. The consumer process reads
// those integers and prints their sum.

#include <stdio.h>            // Standard I/O functions for printing
#include <stdlib.h>           // General utilities
#include <unistd.h>           // For POSIX API, including fork()
#include <sys/mman.h>         // For shared memory functions
#include <sys/wait.h>         // For wait() function
#include <fcntl.h>            // For file control options

// Define the size of shared memory to hold ten integers
#define SHARED_MEM_SIZE (10 * sizeof(int))

int main() 
{
    int i;
    pid_t pid;

    // Open a shared memory object
    // O_CREAT: create the shared memory if it does not exist
    // O_RDWR: open for reading and writing
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    
    // Set the size of the shared memory object
    ftruncate(shm_fd, SHARED_MEM_SIZE);
   
    // Map the shared memory into the process's address space
    int *shared_memory = (int *)mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create a new process using fork()
    pid = fork();

    if (pid == 0) // This block is executed by the child process (consumer)
    {
        int sum = 0; // Variable to store the sum of integers
        // Read integers from shared memory and compute the sum
        for (i = 0; i < 10; i++) 
        {
            sum += shared_memory[i]; // Add each integer to sum
        }

        // Print the sum of the integers
        printf("Sum of first ten positive integers: %d\n", sum);

        // Clean up: unmap shared memory and unlink the shared memory object
        munmap(shared_memory, SHARED_MEM_SIZE);
        shm_unlink("/my_shared_memory");
       
        exit(0); // Exit the child process
    }
    else // This block is executed by the parent process (producer)
    {
        // Write the first ten positive integers into shared memory
        for (i = 1; i <= 10; i++) 
        {
            shared_memory[i-1] = i; // Store each integer in shared memory
        }

        wait(NULL); // Wait for the child process to finish

        // Clean up: unmap shared memory and close the file descriptor
        munmap(shared_memory, SHARED_MEM_SIZE);
        close(shm_fd);
        shm_unlink("/my_shared_memory"); // Remove the shared memory object
    }
    
    return 0; // End of the program
}

/*
 * Explanation of Key Functions:
 * - shm_open(): Opens a shared memory object for reading and writing.
 * - ftruncate(): Sets the size of the shared memory object to the specified size.
 * - mmap(): Maps the shared memory object into the process's address space.
 * - fork(): Creates a new process. The child process gets a PID of 0, and the parent gets the child's PID.
 * - wait(): The parent process waits for the child to complete its execution.
 * - munmap(): Unmaps the shared memory from the process's address space.
 * - close(): Closes the file descriptor associated with the shared memory object.
 * - shm_unlink(): Removes the shared memory object from the system.

 * How the Code Works:
 * 1. The program starts by including necessary headers for I/O and shared memory functions.
 * 2. A shared memory object is created or opened using `shm_open()`.
 * 3. The size of the shared memory is set with `ftruncate()`.
 * 4. The shared memory is mapped into the process's address space using `mmap()`.
 * 5. The program then forks into two processes: the producer and the consumer.
 * 6. The producer process writes the first ten positive integers (1 to 10) into shared memory.
 * 7. The consumer process reads these integers from shared memory and calculates their sum.
 * 8. The consumer prints the sum of the integers it read.
 * 9. Finally, both processes clean up by unmapping the shared memory, closing the file descriptor, and unlinking the shared memory object.

 * Notes:
 * - Inter-Process Communication (IPC): Shared memory allows the producer and consumer to communicate by sharing a common memory space.
 * - Synchronization: The use of `wait()` ensures that the parent process (producer) waits for the child process (consumer) to finish before cleaning up.
 * - Error Handling: In production code, it's important to check the return values of system calls for error handling, though this code does not include such checks for brevity.
 */