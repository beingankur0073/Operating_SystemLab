// Write a program to generate random CPU-burst times (each in the range 10 – 40 milliseconds) for ten
// processes (say, P1, P2, ..., P10). Based on this information, your program should pick processes one by one
// to be scheduled at CPU, depending on each of the following CPU-scheduling algorithms. For each of these
// algorithms, your program should print the order of the scheduled processes and compute the average
// waiting time for these processes.

// RR


#include <stdio.h>      // For input/output functions (e.g., printf)
#include <stdlib.h>     // For general utilities (e.g., rand, srand)
#include <unistd.h>     // For POSIX operating system API (e.g., sleep, though not used here)
#include <stdbool.h>    // For boolean data type
#include <time.h>       // For time functions (e.g., time, used to seed rand)

// Main function to simulate Round Robin scheduling
int main(void)
{
    srand(time(NULL));  // Seed the random number generator with the current time

    int n = 10;             // Number of processes
    int tq = 5;             // Time quantum for Round Robin
    int bt[10];             // Array for burst times of processes
    int bt_copy[10];        // Array to hold remaining burst times
    int wait_t[10];         // Array for waiting times
    int turn_ar_t[10];      // Array for turnaround times
    float avg_wt_t = 0.0f;  // Variable to store average waiting time
    float avgturn_ar_t = 0.0f; // Variable to store average turnaround time
    int remain = n;         // Counter for remaining processes
    bool check[10];         // Array to check if a process is still running

    // Initialize burst times with random values and other arrays
    for (int i = 0; i < n; i++)
    {
        bt[i] = (rand() % 31) + 10;  // Random burst time in the range [10, 40]
        bt_copy[i] = bt[i];          // Copy burst times to track remaining times
        check[i] = true;             // Mark all processes as initially running
        turn_ar_t[i] = 0;            // Initialize turnaround times to 0
        wait_t[i] = 0;               // Initialize waiting times to 0
    }

    // Loop until all processes have completed
    while (remain != 0)
    {
        // Iterate through each process
        for (int i = 0; i < n; i++)
        {
            // Check if process `i` is still running and can complete within this time quantum
            if ((check[i]) && (bt_copy[i] <= tq))
            {
                int cp = bt_copy[i]; // Time taken by this process in this iteration
                turn_ar_t[i] += cp;  // Increment turnaround time for this process
                bt_copy[i] = 0;      // Mark remaining burst time as 0 (completed)
                check[i] = false;    // Mark process as completed
                remain--;            // Decrease remaining process count

                // Increment waiting and turnaround times for other processes still running
                for(int j = 0; j < n; j++)
                {
                    if (check[j])
                    {
                        wait_t[j] += cp;
                        turn_ar_t[j] += cp;
                    }
                }
            }
            // If process `i` cannot complete within this time quantum
            else if (check[i])
            {
                turn_ar_t[i] += tq;  // Add time quantum to turnaround time
                bt_copy[i] -= tq;    // Decrease remaining burst time by time quantum

                // Increment waiting and turnaround times for other processes still running
                for (int j = 0; j < n; j++)
                {
                    if ((check[j]) && (j != i))
                    {
                        wait_t[j] += tq;
                        turn_ar_t[j] += tq;
                    }
                }
            }
        }
    }

    // Display the burst times, waiting times, and turnaround times for each process
    printf("Process\t\tBurst time\tWaiting time\tTurnaround time\n");

    for (int i = 0; i < n; i++)
    {
        printf("P[%d]\t\t%d\t\t\t%d\t\t%d\n", i + 1, bt[i], wait_t[i], turn_ar_t[i]);
        avg_wt_t += wait_t[i];         // Accumulate total waiting time
        avgturn_ar_t += turn_ar_t[i];  // Accumulate total turnaround time
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
 * - `tq`: Time quantum for the Round Robin algorithm.
 * - `bt[]`: Array of burst times, randomly generated for each process.
 * - `bt_copy[]`: Array to hold the remaining burst times for each process.
 * - `wait_t[]`: Array storing the waiting times for each process.
 * - `turn_ar_t[]`: Array storing the turnaround times for each process.
 * - `check[]`: Boolean array to track which processes are still running.
 * - `remain`: Counter for the number of processes yet to complete.

 * How the Code Works:
 * 1. The code generates random burst times for 10 processes.
 * 2. It applies Round Robin scheduling, updating waiting and turnaround times based on the time quantum.
 * 3. If a process completes within the time quantum, it’s marked as finished; if not, it’s scheduled for another round.
 * 4. It prints a table of burst times, waiting times, and turnaround times for all processes.
 * 5. Finally, it calculates and displays the average waiting time and turnaround time across all processes.

 * Notes:
 * - The Round Robin scheduling is implemented using a while loop that iterates through processes until all are finished.
 * - The program ensures fairness by evenly distributing CPU time with the time quantum.
 * - `printf` statements are used for displaying process information in a formatted table, followed by averages.
 */