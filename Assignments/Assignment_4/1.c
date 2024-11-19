// Write a program which creates two threads: one for reading a character from the user and another for
// printing that character.

#include <pthread.h>         // For using pthreads (POSIX threads)
#include <stdio.h>          // Standard I/O functions
#include <stdlib.h>         // General utilities, including exit()

// Global variable to hold the character entered by the user
char c;

// Function for the input thread
void* input(void* args)
{
    // Prompt the user to enter a character
    printf("Enter a character: ");
    scanf(" %c", &c); // Read a character from standard input. The space before %c is to ignore any whitespace.
    pthread_exit(NULL); // Terminate the thread
    return NULL;        // Return NULL (not necessary since we call pthread_exit)
}

// Function for the output thread
void* output(void* args)
{
    // Print the character entered by the user
    printf("Entered character is %c\n", c);
    pthread_exit(NULL); // Terminate the thread
    return NULL;        // Return NULL (not necessary since we call pthread_exit)
}

int main(void)
{
    pthread_t thr[2];   // Array to hold thread identifiers

    // Create the input thread
    pthread_create(&thr[0], NULL, (void *)input, NULL);
    pthread_join(thr[0], NULL); // Wait for the input thread to finish

    // Create the output thread
    pthread_create(&thr[1], NULL, (void *)output, NULL);
    pthread_join(thr[1], NULL); // Wait for the output thread to finish

    return 0; // Exit the program
}

/*
 * Explanation of Key Functions:
 * - pthread_create(): Creates a new thread. The first argument is the thread identifier, 
 *   the second is thread attributes (NULL for default), the third is the function to run, 
 *   and the fourth is arguments for the function (NULL here).
 * - pthread_join(): Blocks the calling thread (main thread) until the specified thread (input/output) terminates.
 * - pthread_exit(): Ends the calling thread.
 * - scanf(): Reads formatted input from standard input.

 * How the Code Works:
 * 1. The program includes necessary headers for threading and input/output.
 * 2. A global variable `c` is declared to store the character input by the user.
 * 3. Two thread functions are defined: `input` for reading a character and `output` for printing it.
 * 4. In `main`, two threads are created: the first thread executes the `input` function, and the second executes the `output` function.
 * 5. The `pthread_join()` function is called after the input thread to ensure the character is read before it is printed.
 * 6. Finally, the program returns 0 to indicate successful execution.

 * Notes:
 * - Synchronization: The program relies on the order of thread execution, where the output thread runs after the input thread has completed. 
 * - The space in the `scanf(" %c", &c);` ensures that any leftover newline character in the input buffer is ignored, allowing the program to correctly read the character.
 * - Error checking is omitted for simplicity. In production code, it’s important to check the return values of `pthread_create()` and other system calls for error handling.
 */