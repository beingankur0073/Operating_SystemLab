// Write a program to generate random CPU-burst times (each in the range 10 – 40 milliseconds) for ten
// processes (say, P1, P2, ..., P10). Based on this information, your program should pick processes one by one
// to be scheduled at CPU, depending on each of the following CPU-scheduling algorithms. For each of these
// algorithms, your program should print the order of the scheduled processes and compute the average
// waiting time for these processes.

// FCFS

#include <stdio.h>      // For input/output functions (e.g., printf)
#include <stdlib.h>     // For general utilities (e.g., rand, srand)
#include <unistd.h>     // For POSIX operating system API (e.g., sleep, though not used here)
#include <time.h>       // For time functions (e.g., time, used to seed rand)

// Main function to calculate waiting and turnaround times
int main(void)
{
    srand(time(NULL));  // Seed the random number generator with the current time

    int n = 10;                      // Number of processes
    int bt[10];                      // Array for burst times of processes
    int wait_t[10];                  // Array for waiting times
    int turn_ar_t[10];               // Array for turnaround times
    float avg_wt_t = 0.0f;           // Variable to store average waiting time
    float avgturn_ar_t = 0.0f;       // Variable to store average turnaround time

    // Assign random burst times (between 10 and 40) to each process
    for (int i = 0; i < n; i++)
    {
        bt[i] = (rand() % 31) + 10;  // Generates a random burst time in the range [10, 40]
    }

    wait_t[0] = 0; // The first process has zero waiting time in FCFS scheduling

    // Calculate waiting times for each process
    for(int i = 1; i < n; i++)
    {
        wait_t[i] = wait_t[i - 1] + bt[i - 1]; // Waiting time = sum of all previous burst times
    }

    // Display table header
    printf("Process\t\tBurst time\tWaiting time\tTurnaround time\n");

    // Calculate turnaround times and accumulate total waiting and turnaround times
    for(int i = 0; i < n; i++)
    {
        turn_ar_t[i] = wait_t[i] + bt[i];     // Turnaround time = waiting time + burst time
        avg_wt_t += wait_t[i];                // Accumulate total waiting time
        avgturn_ar_t += turn_ar_t[i];         // Accumulate total turnaround time
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
 * - `wait_t[]`: Array storing the calculated waiting times for each process.
 * - `turn_ar_t[]`: Array storing the calculated turnaround times for each process.
 * - `avg_wt_t` and `avgturn_ar_t`: Variables to store the average waiting time and average turnaround time, respectively.

 * How the Code Works:
 * 1. The code generates random burst times for 10 processes.
 * 2. It calculates the waiting time for each process based on the cumulative burst times of previous processes.
 * 3. It calculates the turnaround time (waiting time + burst time) for each process.
 * 4. It prints a table of burst times, waiting times, and turnaround times for all processes.
 * 5. Finally, it calculates and displays the average waiting time and turnaround time across all processes.

 * Notes:
 * - The code uses the First-Come, First-Served (FCFS) scheduling algorithm, where each process is served in the order it arrives.
 * - The program generates random burst times, which could be changed by modifying the `rand` parameters.
 * - `printf` statements are used for displaying process information in a formatted table, followed by averages.
 */