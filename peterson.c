#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITERATIONS 5

int turn;
int flag[2] = {0, 0}; // Shared flag array

void enter_critical_section(int self) {
    int other = 1 - self;
    flag[self] = 1;   // Set flag to indicate interest
    turn = other;     // Give turn to the other process
    while (flag[other] && turn == other); // Wait while the other has turn
}

void exit_critical_section(int self) {
    flag[self] = 0; // Indicate exit
}

void *process(void *arg) {
    int self = *(int *)arg; // Get process ID

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        enter_critical_section(self);
        
        printf("Process %d is in the critical section (iteration %d)\n", self, i + 1);
        sleep(1); // Simulating work in critical section
        
        exit_critical_section(self);
        
        printf("Process %d is in the remainder section (iteration %d)\n", self, i + 1);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t p1, p2;
    int id1 = 0, id2 = 1;

    pthread_create(&p1, NULL, process, &id1);
    pthread_create(&p2, NULL, process, &id2);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
