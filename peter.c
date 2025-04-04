#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define Iterations 5

int turn;
int flag[2] = {0, 0};

void enter_critical_section(int self){
    int other = 1 - self;
    flag[self] = 1;
    turn = other;
    while(flag[other] && turn == other);
}

void exit_critical_section(int self){
    flag[self] = 0;
}

void *process(void *args){
    int self = *(int*)args;
    for(int i = 0; i < Iterations; i++){
        enter_critical_section(self);
        printf("Process %d is in the critical section (iteration %d)\n", self, i + 1);
        sleep(1); 
        exit_critical_section(self);
        printf("Process %d is in the remainder section (iteration %d)\n", self, i + 1);
        sleep(1);
    }
    
    return NULL;
}

int main(){
    pthread_t p1, p2;
    int id1 = 0, id2 = 1;
    pthread_create(&p1, NULL, process, &id1);
    pthread_create(&p2, NULL, process, &id2);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    return 0;
}