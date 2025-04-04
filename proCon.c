#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    while (1) {
        sleep(1); // Simulate production delay
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            printf("Buffer full. Producer sleeping...\n");
            pthread_cond_wait(&cond_producer, &mutex);
        }

        buffer[count++] = rand() % 100; // Produce an item
        printf("Produced: %d, Buffer Count: %d\n", buffer[count-1], count);

        pthread_cond_signal(&cond_consumer); // Wake up consumer
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *arg) {
    while (1) {
        sleep(2); // Simulate consumption delay
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("Buffer empty. Consumer sleeping...\n");
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        int item = buffer[--count]; // Consume an item
        printf("Consumed: %d, Buffer Count: %d\n", item, count);

        pthread_cond_signal(&cond_producer); // Wake up producer
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
