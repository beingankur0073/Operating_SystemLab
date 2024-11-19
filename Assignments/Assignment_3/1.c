//  Write a program which creates a shared memory and writes “Department of IT” in that shared memory.

#include <stdio.h>            // Standard I/O functions for printing
#include <stdlib.h>           // General utilities
#include <unistd.h>           // For POSIX operating system API, including fork()
#include <sys/mman.h>         // For shared memory functions
#include <sys/wait.h>         // For wait() function (not used in this example)
#include <fcntl.h>            // For file control options
#include <string.h>           // For memory operations like memcpy()

// Define the size of shared memory to be allocated
#define SHARED_MEM_SIZE (16 * sizeof(char))

int main()
{
    // Variable declaration
    int i;

    // Open a shared memory object
    // O_CREAT: create the shared memory if it does not exist
    // O_RDWR: open for reading and writing
    char shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    
    // Set the size of the shared memory object
    ftruncate(shm_fd, SHARED_MEM_SIZE);

    // Map the shared memory into the address space of the process
    // mmap() returns a pointer to the shared memory
    char *shared_memory = (char *)mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Message to be stored in shared memory
    char message[] = "Department of IT";
    
    // Copy the message into the shared memory
    memcpy(shared_memory, message, sizeof(message));
    
    // Print the address of the file descriptor (not the shared memory address)
    printf("Message stored in address: %x\n", &shm_fd);
    // Print the message stored in the shared memory
    printf("Message stored in shared memory is: %s\n", shared_memory);

    // Unmap the shared memory from the address space
    munmap(shared_memory, SHARED_MEM_SIZE);
    
    // Close the file descriptor for the shared memory
    close(shm_fd);
    
    // Unlink the shared memory object, removing it from the system
    shm_unlink("/my_shared_memory");
    
    return 0; // End of the program
}

/*
 * Explanation of Key Functions:
 * - shm_open(): Opens a shared memory object. It creates the object if it does not exist and opens it for read/write access.
 * - ftruncate(): Sets the size of the shared memory object to the specified size.
 * - mmap(): Maps the shared memory object into the process's address space, allowing it to be accessed like regular memory.
 * - memcpy(): Copies a specified number of bytes from one memory location to another.
 * - munmap(): Unmaps the shared memory from the process's address space.
 * - close(): Closes the file descriptor associated with the shared memory object.
 * - shm_unlink(): Removes the shared memory object from the system, so it can no longer be accessed.

 * How the Code Works:
 * 1. The program starts by including necessary headers for I/O, shared memory, and other utilities.
 * 2. A shared memory object is created or opened using `shm_open()`.
 * 3. The size of the shared memory is set with `ftruncate()`.
 * 4. The shared memory is mapped into the process's address space using `mmap()`.
 * 5. A predefined message ("Department of IT") is copied into the shared memory.
 * 6. The address of the file descriptor and the contents of the shared memory are printed.
 * 7. The shared memory is unmapped using `munmap()`, and the file descriptor is closed.
 * 8. Finally, the shared memory object is unlinked with `shm_unlink()`, ensuring it is cleaned up.

 * Notes:
 * - Shared Memory: A method for processes to communicate with each other by sharing a common memory space.
 * - File Descriptor: The variable `shm_fd` holds a file descriptor for the shared memory object, which is used for subsequent operations.
 * - Error Handling: In production code, it is essential to check the return values of system calls (e.g., `shm_open`, `mmap`, `ftruncate`, etc.) for error handling.
 */