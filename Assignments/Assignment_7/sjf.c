// Write a program to generate random CPU-burst times (each in the range 10 – 40 milliseconds) for ten
// processes (say, P1, P2, ..., P10). Based on this information, your program should pick processes one by one
// to be scheduled at CPU, depending on each of the following CPU-scheduling algorithms. For each of these
// algorithms, your program should print the order of the scheduled processes and compute the average
// waiting time for these processes.

// SJF
#include <stdio.h>      // For input/output functions (e.g., printf)
#include <stdlib.h>     // For general utilities (e.g., rand, srand)
#include <unistd.h>     // For POSIX operating system API (e.g., sleep, though not used here)
#include <stdbool.h>    // For boolean data type
#include <time.h>       // For time functions (e.g., time, used to seed rand)

// Main function for SJF scheduling
int main(void)
{
    srand(time(NULL));  // Seed the random number generator with the current time

    int n = 10;                // Number of processes
    int bt[10];                // Array for burst times of processes
    int wait_t[10];            // Array for waiting times
    int turn_ar_t[10];         // Array for turnaround times
    float avg_wt_t = 0.0f;     // Variable to store average waiting time
    float avgturn_ar_t = 0.0f; // Variable to store average turnaround time
    bool check[10];            // Array to check if a process is already scheduled
    int last = -1;             // Tracks the last executed process
    int curr;                  // Current process with shortest burst time

    // Generate random burst times and initialize check array
    for (int i = 0; i < n; i++)
    {
        bt[i] = (rand() % 31) + 10; // Random burst time in the range [10, 40]
        check[i] = true;            // Mark all processes as initially unscheduled
    }

    printf("Process\t\tBurst time\tWaiting time\tTurnaround time\n");

    // Loop to schedule each process based on SJF (Shortest Job First)
    for (int i = 0; i < n; i++)
    {
        int min = 41; // Initialize to a value greater than the max burst time (40)
        for (int j = 0; j < n; j++)
        {
            // Find the process with the smallest burst time that hasn't been scheduled yet
            if ((bt[j] < min) && (check[j]))
            {
                min = bt[j];
                curr = j; // Update current process to the one with the shortest burst time
            }
        }

        check[curr] = false; // Mark the chosen process as scheduled

        // Calculate waiting time for the current process
        if (last != -1)
        {
            wait_t[curr] = wait_t[last] + bt[last];
        }
        else
        {
            wait_t[curr] = 0; // First process has a waiting time of 0
        }

        // Calculate turnaround time and accumulate waiting and turnaround times
        turn_ar_t[curr] = bt[curr] + wait_t[curr];
        avg_wt_t += wait_t[curr];
        avgturn_ar_t += turn_ar_t[curr];

        last = curr; // Update last to the current process
    }

    // Display burst times, waiting times, and turnaround times for each process
    for (int i = 0; i < n; i++)
    {
        printf("P[%d]\t\t%d\t\t\t%d\t\t%d\n", i + 1, bt[i], wait_t[i], turn_ar_t[i]);
    }

    // Calculate and display average waiting and turnaround times
    avg_wt_t /= n;
    avgturn_ar_t /= n;
    printf("Average Waiting Time: %f\n", avg_wt_t);
    printf("Average Turnaround Time: %f\n", avgturn_ar_t);

    return 0;
}

/*
 * Explanation of Key Variables:
 * - `n`: Number of processes, set to 10 for this example.
 * - `bt[]`: Array of burst times, randomly generated for each process.
 * - `check[]`: Boolean array to track which processes have been scheduled.
 * - `wait_t[]`: Array for the waiting times of each process.
 * - `turn_ar_t[]`: Array for the turnaround times of each process.
 * - `last`: Index of the last process executed, used to compute waiting time for the current process.
 * - `curr`: Index of the current process with the shortest burst time.
 *
 * Explanation of the Algorithm:
 * 1. The code generates random burst times for 10 processes.
 * 2. It finds the process with the shortest burst time that has not been scheduled and executes it.
 * 3. The waiting time for each process is the cumulative burst time of all previous processes.
 * 4. The turnaround time is the waiting time plus the burst time for each process.
 * 5. The program calculates and prints the average waiting and turnaround times.
 *
 * How the SJF Algorithm Minimizes Waiting Time:
 * - The SJF algorithm minimizes waiting time by prioritizing processes with shorter burst times, ensuring that shorter processes are completed quickly.
 */