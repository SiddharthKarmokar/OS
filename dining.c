#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

#define PHILOSOPHERS 5
int n = PHILOSOPHERS;
pthread_mutex_t forks[PHILOSOPHERS];

void Thinking(int i){
    printf("Philosopher %d is thinking\n", i);
    usleep(500000);
}

void EAT(int i){
    printf("Philosopher %d is eating\n", i);
    usleep(500000);
}

void* philosopher(void *args){
    int i = *(int*)args;
    while(true){
        Thinking(i);
        if (i % 2 == 0) {
            pthread_mutex_lock(&forks[i]);
            printf("Philosopher %d has taken up left fork\n", i);
            pthread_mutex_lock(&forks[(i+1)%n]);
            printf("Philosopher %d has taken up right fork\n", i);
        } else {
            pthread_mutex_lock(&forks[(i+1)%n]);
            printf("Philosopher %d has taken up right fork\n", i);
            pthread_mutex_lock(&forks[i]);
            printf("Philosopher %d has taken up left fork\n", i);
        }
        
        EAT(i);
        
        pthread_mutex_unlock(&forks[i]);
        printf("Philosopher %d has put down left fork\n", i);
        pthread_mutex_unlock(&forks[(i+1)%n]);
        printf("Philosopher %d has put down right fork\n", i);
    }
    return NULL;
}

int main(){
    pthread_t phil[PHILOSOPHERS];
    for(int i = 0; i < n; i++){
        pthread_mutex_init(&forks[i], NULL);
    }
    for(int i = 0; i < n; i++){
        int *id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&phil[i], NULL, philosopher, id);
    }
    for(int i = 0; i < n; i++){
        pthread_join(phil[i], NULL);
    }
    for(int i = 0; i < n; i++){
        pthread_mutex_destroy(&forks[i]);
    }
    return 0;
}
