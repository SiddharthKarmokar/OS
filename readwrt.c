#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int rc = 0;
volatile int mutex = 1;
volatile int db = 1;

void down(volatile int *sema, char *thread) {
    while (!__sync_bool_compare_and_swap(sema, 1, 0)) {
        printf("%s is blocked\n", thread);
        usleep(100000); // Avoid busy-waiting
    }
}

void up(volatile int *sema, char *thread) {
    *sema = 1;
}

void reader(int id) {
    printf("Enter Reader %d\n", id);
    char buffer[100];
    sprintf(buffer, "Reader %d", id);
    
    while (1) {
        down(&mutex, buffer);
        rc++;
        if (rc == 1) {
            down(&db, buffer);
        }
        up(&mutex, buffer);

        FILE *file = fopen("cs.txt", "r");
        if (file == NULL) {
            printf("Reader %d: Error opening file!\n", id);
        } else {
            char c;
            printf("Reader %d is Reading....\n", id);
            while ((fscanf(file, "%c", &c)) != EOF) {
                printf("%c from %s\n", c, buffer);
            }
            fclose(file);
            printf("Reader %d completed reading.\n", id);
        }

        down(&mutex, buffer);
        rc--;
        if (rc == 0) {
            up(&db, buffer);
        }
        up(&mutex, buffer);

        usleep(5000000);  // Sleep 5 seconds
    }
}

void writer(int id) {
    printf("Enter Writer %d\n", id);
    char buffer[100];
    sprintf(buffer, "Writer %d", id);

    while (1) {
        down(&db, buffer);

        FILE *file = fopen("cs.txt", "w");
        if (file == NULL) {
            printf("Writer %d: Error opening file!\n", id);
        } else {
            printf("Writer %d is Writing....\n", id);
            fprintf(file, "from %d\n", id);
            fclose(file);
            printf("Writer %d completed writing.\n", id);
        }

        up(&db, buffer);
        usleep(5000000);
    }
}

void *reader_thread(void *args) {
    int id = *(int *)args;
    free(args);
    reader(id);
    return NULL;
}

void *writer_thread(void *args) {
    int id = *(int *)args;
    free(args);
    writer(id);
    return NULL;
}

int main() {
    pthread_t readers[3], writers[2];

    for (int i = 0; i < 3; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader_thread, id);
    }

    for (int i = 0; i < 2; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer_thread, id);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
