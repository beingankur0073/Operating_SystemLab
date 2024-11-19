// Write another program extending the program (in Item #2) to a scenario where we have three processes
// (one producer and two consumers).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <math.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int* arr;
int* n;
int* count;
bool* lock;

bool testandset(bool* target)
{
    bool rv = *target;
    *target = true;
    return rv;
}

void producer(void)
{
    *lock = false;
    int num = rand() % 1001;
    while(*count >= 100 || testandset(lock));

    arr[*count] = num;
    printf("Producer produces %d at index %d.\n", num, *count);
    *count = (*count + 1);
    *n = (*n + 1);

    *lock = false;
}

void consumer1(void)
{
    *lock = false;
    while(*n == 0 || testandset(lock));

    printf("Consumer1 consumes %d at index %d.\n", arr[*count - *n], *count - *n);
    *n = (*n - 1);

    *lock = false;
}

void consumer2(void)
{
    *lock = false;
    while(*n == 0 || testandset(lock));

    printf("Consumer2 consumes %d at index %d.\n", arr[*count - *n], *count - *n);
    *n = (*n - 1);

    *lock = false;
}

int main(void)
{
    srand(time(NULL));

    int fd1 = shm_open("shared memory", O_CREAT | O_RDWR, 0666);
    ftruncate(fd1, 100 * sizeof(int));
    arr = mmap(0, 100 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    int fd2 = shm_open("number of filled space", O_CREAT | O_RDWR, 0666);
    ftruncate(fd2, 1 * sizeof(int));
    n = mmap(0, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    int fd3 = shm_open("count", O_CREAT | O_RDWR, 0666);
    ftruncate(fd3, 1 * sizeof(int));
    count = mmap(0, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd3, 0);

    int fd4 = shm_open("lock", O_CREAT | O_RDWR, 0666);
    ftruncate(fd4, 1 * sizeof(bool));
    lock = mmap(0, 1 * sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED, fd4, 0);

    pid_t pid1 = fork();
    if (pid1 != 0)
    {
        for (int i = 0; i < 100; i++)
        {
            producer();
        }
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 != 0)
        {
            while(*count < 100 || *n > 0)
            {
                consumer1();
            }
        }
        else
        {
            while(*count < 100 || *n > 0)
            {
                consumer2();
            }
        }
    }

    wait(NULL);
    wait(NULL);
    shm_unlink("shared memory");
    shm_unlink("number of filled space");
    shm_unlink("count");
    shm_unlink("lock");

    return 0;
}