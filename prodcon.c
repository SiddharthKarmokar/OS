#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#define BUFFER_SIZE 10

typedef struct{
    int n;
} item;

item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Semaphore structure
typedef struct {
    int val;
    int waiting;
} semaphore;

semaphore s = {1, 0};
semaphore full = {0, 0};
semaphore empty = {BUFFER_SIZE, 0};

void block() {
    printf("Thread blocked on semaphore\n");
    while (true) {
        usleep(100000);
        if(empty.waiting == 0 && full.waiting == 0) break;
    }
}

void wakeup() {
    printf("Thread waking up\n");
}

void down(semaphore *sema) {
    sema->val--;
    if (sema->val < 0) {
        sema->waiting++;
        block();
    }
}

void up(semaphore *sema) {
    sema->val++;
    if (sema->waiting > 0) {
        sema->waiting--;
        wakeup();
    }
}

void producer(){
    item nextProduced;
    while(true){
        down(&empty);
        down(&s);
        buffer[in] = nextProduced;
        in = (in + 1) % BUFFER_SIZE;
        printf("Item Produced in: %d out: %d\n", in, out);
        // usleep(5000000);
        usleep(1000);
        up(&s);
        up(&full);
    }
}

void consumer(){
    item nextConsumed;
    while(true){
        down(&full);
        down(&s);
        nextConsumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Item Consumed in: %d out: %d\n", in, out);
        usleep(500000);
        up(&s);
        up(&empty);
    }
}

void *producer_thread(void *args){
    producer();
    return NULL;
}

void *consumer_thread(void *args){
    consumer();
    return NULL;
}

int main(){    
    pthread_t prod1, prod2, con;
    pthread_create(&prod1, NULL, producer_thread, NULL);
    pthread_create(&prod2, NULL, producer_thread, NULL);
    pthread_create(&con, NULL, consumer_thread, NULL);      
    pthread_join(prod1, NULL);   
    pthread_join(prod2, NULL);   
    pthread_join(con, NULL);   
    return 0;
}
