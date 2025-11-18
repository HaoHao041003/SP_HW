#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_SIZE 4096

typedef struct{
    int pid;
    int value;
}Pair;

typedef struct{
    int top;
    //int capacity;
    sem_t Mutex, Empty_Slots, Full_Slots;
    Pair* datas;
}SharedStack;

SharedStack *stack;

void push(Pair pair, int id)
{
    sem_wait(&stack->Empty_Slots);
    
    sem_wait(&stack->Mutex);
    stack->datas[stack->top++] = pair;
    printf("P%d write (%d, %d)\n", id, pair.pid, pair.value);
    sem_post(&stack->Mutex);

    sem_post(&stack->Full_Slots);
}

Pair pop()
{
    //printf("Consumer (PID %d) waits\n", getpid());
    sem_wait(&stack->Full_Slots);

    sem_wait(&stack->Mutex);
    Pair temp = stack->datas[--stack->top];

    sem_post(&stack->Empty_Slots);

    sem_post(&stack->Mutex);
    return temp;
}


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s M N\n", argv[0]);
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    key_t key = ftok("etc/passwd", 'J');
    int total_shm_size = sizeof(SharedStack) + sizeof(Pair) * M;
    int shmid = shmget(key, total_shm_size, IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }
    
    stack = (SharedStack*)shmat(shmid, NULL, 0);
    if(stack == (SharedStack*)-1)
    {
        perror("shmat");
        exit(1);
    }
    //stack->capacity = M;
    stack->top = 0;
    stack->datas = (Pair*)((char*)stack + sizeof(SharedStack));

    if(sem_init(&stack->Mutex, 1, 1))
    {
        perror("sem_init Mutex");
        exit(1);
    }
    if(sem_init(&stack->Empty_Slots, 1, M))
    {
        perror("sem_init Empty Slots");
        exit(1);
    }
    if(sem_init(&stack->Full_Slots, 1, 0))
    {
        perror("sem_init Full Slots");
        exit(1);
    }

    for(int i=0; i<M; i++)
    {
        int pid = fork();
        if(pid == 0)
        {
            srand(time(NULL) ^ getpid());
            printf("Producer P%d start writing...\n", i+1);
            for(int j=0; j<N; j++)
            {
                int random_val = rand()%10+1;
                Pair temp = {getpid(), random_val};
                push(temp, i+1);
            }
            exit(0);
        }
    }

    int sum = 0;
    for(int i=0; i<M*N; i++)
    {
        Pair temp = pop();
        sum += temp.value;
        printf("Consumer reads (%d, %d). Sum = %d\n", temp.pid, temp.value, sum);
    }
    printf("Total Data Points Processed: %d\n", M*N);
    printf("Final Cumulative Sum: [%d]\n", sum);

    sem_destroy(&stack->Mutex); sem_destroy(&stack->Empty_Slots); sem_destroy(&stack->Full_Slots);
    if(shmdt(stack) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
    printf("END\n");
}