#include <stdio.h>       // Standard I/O functions for printing
#include <unistd.h>      // POSIX API for process control, includes fork(), getpid(), getppid()
#include <stdlib.h>      // General utilities (not heavily used here)
#include <sys/wait.h>    // For wait() function to manage child processes

pid_t pid1, pid2;        // Process ID variables to hold the PIDs of parent and child processes

int main(void)
{
    // Create a new process (child) by calling fork()
    pid1 = fork();

    // Check if this is the parent or child process based on the return value of fork()
    if (pid1 > 0) // Parent process: fork() returns child's PID
    {
        // Parent process waits for the child process to finish executing
        // Using NULL with wait() is a practical choice when the parent process does not need the exit status of the child process
        pid2 = wait(NULL);   
        // Output the parent process ID and the child's PID
        printf("Parent process with pid %d and child pid is %d.\n", getpid(), pid1);
        // getpid() returns the PID of the current process (parent)
    }
    else if (pid1 == 0) // Child process: fork() returns 0
    {
        // Output the child process ID and its parent's PID
        printf("Child process with pid %d and parent has pid %d.\n", getpid(), getppid());
        // getpid() returns the PID of the current process (child)
        // getppid() returns the PID of the parent process
    }
    
    // End of the program: both processes (parent and child) will exit here
    return 0;
}

/*
 * Explanation of Key Functions:
 * - fork(): Creates a new process. It returns 0 to the child process and the child’s PID to the parent.
 * - getpid() and getppid(): 
 *   - getpid(): Retrieve the current process ID.
 *   - getppid(): Retrieve the parent process ID.
 * - wait(): The parent process calls wait() to pause execution until the child process completes,
 *           helping to prevent zombie processes (a process that has completed execution but still has an entry in the process table).
 *
 * How the Code Works:
 * 1. The program starts by including necessary headers for I/O and process control.
 * 2. It defines two variables to hold process IDs.
 * 3. The main function calls fork() to create a child process.
 * 4. Depending on the return value of fork():
 *    - The parent process waits for the child to finish using wait() and prints its PID along with the child's PID.
 *    - The child process prints its own PID and its parent's PID.
 * 5. The program terminates after both processes have executed their respective code blocks.
 *
 * Notes:
 * - Zombie Processes: Without wait(), if the parent process completes before the child, the child becomes a zombie,
 *                    which is a process that has finished executing but still has an entry in the process table.
 * - Platform Behavior: The output order can differ across systems because process scheduling is managed by the OS.
 */