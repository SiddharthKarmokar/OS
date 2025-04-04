#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<stdatomic.h>
#include<pthread.h>

#define PRODUCERS 1
#define CONSUMERS 3
#define BUFFER_SIZE 20

int n = PRODUCERS, m = CONSUMERS;
atomic_int s = 1, full = 0, empty = BUFFER_SIZE;
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

void down(atomic_int *sema){
    while(atomic_fetch_sub(sema, 1) <= 0){
        atomic_fetch_add(sema, 1);
        usleep(1000);
    }
}

void up(atomic_int *sema){
    atomic_fetch_add(sema, 1);
}

void *producer(void *args){
    int id = *(int*)args;
    int nextP = 0;
    while(nextP < 10){
        down(&empty);
        down(&s);
        buffer[in] = nextP;
        printf("Producer %d produced %d at %d\n", id, nextP, in);
        in = (in + 1)%BUFFER_SIZE;
        up(&s);
        up(&full);
        nextP++;
        usleep(500000);
    }
    
    return NULL;    
}
void *consumer(void *args){
    int id = *(int*)args;
    int sum = 0;
    int i = 0;
    int nextC = -1;
    while(nextC < 10){
        down(&full);
        down(&s);
        nextC = buffer[out];
        out = (out + 1)%BUFFER_SIZE;
        sum += nextC;
        printf("Consumer %d consumed %d at %d\n", id, nextC, out);
        printf("Consumer %d sum: %d\n", id, sum);
        up(&s);
        up(&empty);
        usleep(500000);
    }
    return NULL;
}

int main(){
    pthread_t producers[PRODUCERS], consumers[CONSUMERS];
    for(int i = 0; i < n; i++){
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producers[i], NULL, producer, id);
    }
    for(int i = 0; i < m; i++){
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumers[i], NULL, consumer, id);
    }
    for(int i = 0; i < n; i++){
        pthread_join(producers[i], NULL);
    }
    for(int i = 0; i < m; i++){
        pthread_join(consumers[i], NULL);
    }
    return 0;
}


