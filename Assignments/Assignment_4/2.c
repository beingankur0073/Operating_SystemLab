// Write a program which creates a perfect binary tree of fifteen distinct positive integers (where each node
// contains an integer ∈ [10, 99]). Given an input integer i, the program searches whether i is present in the
// tree and prints the search result accordingly. Create two threads each taking care of searching in one of the

// two subtrees. See whether such a split improves the efficiency of the search operation compared to a single-
// threaded searching. See if this improvement increases if we take a larger tree.

#include <pthread.h>         // For using pthreads (POSIX threads)
#include <stdio.h>          // Standard I/O functions
#include <stdlib.h>         // General utilities, including malloc()

// Global variables
int search; // Value to search in the binary tree
int arr[] = {10, 11, 12, 13, 15, 16, 23, 18, 19, 20, 25, 30, 35, 40, 54}; // Array to initialize the tree
struct Node* root = NULL; // Root of the binary tree

// Definition of a binary tree node
struct Node
{
    int data;              // Node's data
    struct Node* left;     // Pointer to left child
    struct Node* right;    // Pointer to right child
};

// Function to check if a node with a specific key exists in the tree
int ifNodeExists(struct Node* node, int key)
{
    if (node == NULL) // Base case: If node is null, key does not exist
        return 0;

    if (node->data == key) // If current node's data matches the key
        return 1;

    // Recursively search in left and right subtrees
    int res1 = ifNodeExists(node->left, key);
    if (res1) return 1; // Key found in left subtree

    int res2 = ifNodeExists(node->right, key);
    return res2; // Return result from right subtree
}

// Function to create the binary tree from the predefined array
void create_tree(void)
{
    struct Node* head = (struct Node*)malloc(sizeof(struct Node)); // Allocate memory for root node
    head->data = 10; // Set root node data
    head->left = NULL; // Initialize left and right children to NULL
    head->right = NULL;
    root = head; // Assign head as the root of the tree

    struct Node* node_arr[15]; // Array to keep track of nodes for level-order insertion
    node_arr[0] = root; // First element is the root

    int index = 0; // Index for tracking the parent node
    int last_index = 1; // Index for tracking the next free node position

    // Insert nodes based on the input array
    for (int i = 1; i < 15; i++)
    {
        if (!node_arr[index]->left) // If left child is empty
        {
            node_arr[index]->left = malloc(sizeof(struct Node)); // Allocate memory for left child
            node_arr[last_index++] = node_arr[index]->left; // Update node array
            node_arr[index]->left->data = arr[i]; // Set the left child's data
            node_arr[index]->left->left = NULL; // Initialize children to NULL
            node_arr[index]->left->right = NULL;
        }
        else 
        {
            node_arr[index]->right = malloc(sizeof(struct Node)); // Allocate memory for right child
            node_arr[last_index++] = node_arr[index]->right; // Update node array
            node_arr[index]->right->data = arr[i]; // Set the right child's data
            node_arr[index]->right->left = NULL; // Initialize children to NULL
            node_arr[index]->right->right = NULL;
            index++; // Move to the next parent node
        }
    }
}

// Thread function to search in the left subtree
void* searchleft(void* args)
{
    int result = ifNodeExists(root->left, search); // Search in the left subtree
    if (result) 
    {
        printf("Key found in left subtree.\n");
    }
    else
    {
        printf("Key not found in left subtree.\n");
    }
    pthread_exit(NULL); // Terminate the thread
    return NULL; // Return NULL (not necessary since we call pthread_exit)
}

// Thread function to search in the right subtree
void* searchright(void* args)
{
    int result = ifNodeExists(root->right, search); // Search in the right subtree
    if (result) 
    {
        printf("Key found in right subtree.\n");
    }
    else
    {
        printf("Key not found in right subtree.\n");
    }
    pthread_exit(NULL); // Terminate the thread
    return NULL; // Return NULL (not necessary since we call pthread_exit)
}

int main(void)
{
    create_tree(); // Create the binary tree
    printf("Enter value to be searched: ");
    scanf("%d", &search); // Read the value to search for
    
    pthread_t thr[2]; // Array to hold thread identifiers
    
    // Create threads for searching in left and right subtrees
    pthread_create(&thr[0], NULL, (void *)searchleft, NULL);
    pthread_create(&thr[1], NULL, (void *)searchright, NULL);
    
    // Wait for both threads to finish
    pthread_join(thr[0], NULL);
    pthread_join(thr[1], NULL);
    
    // Check if the search value matches the root data
    if (search == root->data)
    {
        printf("Value present in root.\n");
    }
    else
    {
        printf("Value not present in root.\n");
    }

    return 0; // Exit the program
}

/*
 * Explanation of Key Functions:
 * - ifNodeExists(): Recursively checks if a node with the specified key exists in the binary tree.
 * - create_tree(): Initializes the binary tree from the predefined array using level-order insertion.
 * - pthread_create(): Creates a new thread, executing the specified function (searchleft or searchright).
 * - pthread_join(): Blocks the main thread until the specified thread terminates.
 * - pthread_exit(): Ends the calling thread.

 * How the Code Works:
 * 1. The program defines a binary tree using a structure with pointers to left and right children.
 * 2. The `create_tree` function initializes the binary tree using an array of integers.
 * 3. The user is prompted to enter a value to search in the tree.
 * 4. Two threads are created to search for the value in the left and right subtrees simultaneously.
 * 5. After both threads finish executing, the program checks if the search value is equal to the root node's data and prints the appropriate message.

 * Notes:
 * - This program demonstrates multithreading to perform concurrent searches in a binary tree, improving efficiency.
 * - It is assumed that the binary tree is complete and that it has exactly 15 nodes.
 * - Error handling for memory allocation and thread creation is omitted for simplicity. In a production environment, it’s essential to handle these errors.
 */