// Write a program for the following:

// a. Suppose we have a system with n = 10 processes (say, P1, P2, ..., P10) and m = 5 resource types
// (say, R1, R2, ..., R5), where each resource type has a single instance of that resource. For each pair
// (i, j), we have an edge between Pi and Rj with probability 0.5. Furthermore, if Pi and Rj have an
// edge between them, that edge is an assignment edge with probability 0.5, and it is a request edge
// otherwise. Build the resource-allocation graph for this system.

// b. Given the graph, check whether there is a deadlock present in the system.

// c. Extend your program such that it works for any values of n and m to be input by the user during
// runtime.
#include <stdio.h>      // For input/output functions (e.g., printf, scanf)
#include <stdlib.h>     // For memory allocation and random number generation (e.g., malloc, rand)
#include <time.h>       // For seeding random number generation
#include <stdbool.h>    // For boolean data type

int n, m, d = 0;        // Declare variables for number of processes (n), number of resources (m), and a count variable (d) 

int main(void)
{
    srand(time(NULL));  // Seed the random number generator for different values on each run

    printf("Enter the number of processes: ");
    scanf("%d", &n);    // Take user input for the number of processes
    printf("Enter the number of different types of resources: ");
    scanf("%d", &m);    // Take user input for the number of resources

    // Allocate memory for matrices and vectors
    int **adjacency_matrix = (int**)malloc(n * sizeof(int*));   // Adjacency matrix to represent requests and assignments
    int **req_resources = (int**)malloc(n * sizeof(int*));      // Matrix for requested resources
    int **allocated = (int**)malloc(n * sizeof(int*));          // Matrix for allocated resources
    int *available = (int*)malloc(m * sizeof(int));             // Array for available resources

    // Allocate memory for each process in the matrices
    for(int i = 0; i < n; i++) {
        adjacency_matrix[i] = (int*)malloc(m * sizeof(int));
        req_resources[i] = (int*)malloc(m * sizeof(int));
        allocated[i] = (int*)malloc(m * sizeof(int));
    }

    for (int i = 0; i < m; i++) {
        available[i] = 1;  // Initialize all resources in the available array to 1, assuming all are initially available
    }

    // Populate adjacency matrix with random values (0 or 1) indicating request or absence of an edge
    for (int i = 0; i < n; i++) {   
        for (int j = 0; j < m; j++) {
            adjacency_matrix[i][j] = rand() % 2;
        }
    }

    printf("\nUsing 1 for request edge and 2 for assignment edge.\n\n");

    // Randomly convert some requests (1s) to assignments (2s), updating the `available` array accordingly
    for (int j = 0; j < m; j++) {   
        bool flag = false;
        for (int i = 0; i < n; i++) {
            if (adjacency_matrix[i][j] == 1 && !flag) {
                int prob = rand() % 2;
                if (prob == 1) {
                    flag = true;
                    adjacency_matrix[i][j] = 2;
                    available[j] = 0;  // Mark the resource as unavailable
                }
            }
        }
    }

    // Initialize `req_resources` and `allocated` matrices based on `adjacency_matrix`
    for (int i = 0; i < n; i++) {      
        for (int j = 0; j < m; j++) {  
            // Set allocation and request based on adjacency matrix values
            if (adjacency_matrix[i][j] == 0) {
                allocated[i][j] = 0;
                req_resources[i][j] = 0;
            } else if (adjacency_matrix[i][j] == 1) {
                allocated[i][j] = 0;
                req_resources[i][j] = 1;
            } else if (adjacency_matrix[i][j] == 2) {
                allocated[i][j] = 1;
                req_resources[i][j] = 0;
            }
        }
    }

    // Display the adjacency matrix
    for (int i = 0; i < n; i++) {   
        for (int j = 0; j < m; j++) {
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }

    // Deadlock Detection Algorithm
    for (int k = 0; k < n; k++) {   
        for (int i = 0; i < n; i++) {
            int count = 0;  // Counter for resources that are available for the process
            for (int j = 0; j < m; j++) {
                if (req_resources[i][j] <= available[j]) {  // If the resources requested by the process are available, increase count
                    count++;
                }
            }

            if (count == m) {  // If the process can finish (all its requested resources are available)
                d++;  // Increase deadlock-free count
                for (int j = 0; j < m; j++) {    // Release resources held by the process and mark its request as fulfilled
                    available[j] += allocated[i][j];
                    req_resources[i][j] = 2;  // Mark the process as completed
                }
            }
        }
    }

    if (d == n) {    // Deadlock presence check: if all processes were able to finish, no deadlock
        printf("\nDeadlock not present.\n");
    } else {
        printf("\nDeadlock present.\n");
    }

    // Free dynamically allocated memory for matrices and vectors to avoid memory leaks
    for (int i = 0; i < n; i++) {
        free(adjacency_matrix[i]);
        free(req_resources[i]);
        free(allocated[i]);
    }
    free(adjacency_matrix);
    free(req_resources);
    free(allocated);
    free(available);

    return 0;
}

/*
 * Explanation of Key Variables:
 * - `n`: Number of processes entered by the user.
 * - `m`: Number of different types of resources entered by the user.
 * - `d`: Counter to keep track of the number of processes that can finish.
 * - `adjacency_matrix`: Represents the resource allocation graph where 1 indicates a request and 2 indicates an assignment.
 * - `req_resources[][]`: Matrix for resources requested by each process.
 * - `allocated[][]`: Matrix for resources allocated to each process.
 * - `available[]`: Array representing the number of available resources.

 * Explanation of the Algorithm:
 * 1. The program initializes matrices to represent processes and their resources.
 * 2. It populates the adjacency matrix with random values to indicate resource requests and assignments.
 * 3. The `available` array is updated based on the resources assigned.
 * 4. A deadlock detection algorithm is implemented to check if all processes can complete with the available resources.
 * 5. If all processes can finish, it concludes that there is no deadlock; otherwise, it indicates a deadlock.

 * Memory Management:
 * - Dynamically allocated memory for matrices and vectors is freed at the end of the program to avoid memory leaks.
 */