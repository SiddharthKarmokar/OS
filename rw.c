#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int rc = 0;
volatile int mutex = 1;
volatile int db = 1;

void down(volatile int *sema, char *thread){
    while(!__sync_bool_compare_and_swap(sema, 1, 0)){
        printf("%s is blocked\n", thread);
        usleep(1000000);
    }
}

void up(volatile int *sema, char *thread){
    *sema = 1;
}

void *reader(void *args){
    int id = *(int*)args;
    char buffer[100];
    sprintf(buffer, "Reader %d", id);
    while(1){
        down(&mutex, buffer);
        rc++;
        if(rc == 1){
            down(&db, buffer);
        }
        up(&mutex, buffer);
        FILE *file = fopen("criticalSection.txt", "r");
        char c;
        printf("%s is reading.....\n", buffer);
        while((fscanf(file, "%c", &c)) != EOF){
            printf("%c from %s\n", c, buffer);
        }
        fclose(file);
        printf("%s completed reading\n", buffer);
        down(&mutex, buffer);
        rc -= 1;
        if(rc == 0){
            up(&db, buffer);
        }
        up(&mutex, buffer);
        usleep(5000000);
    }
    return NULL;
}

void *writer(void *args){
    int id = *(int*)args;
    char buffer[100];
    char *w;
    if (id == 1){
        w = "Halley";
    }else{
        w = "Comet";
    }
    sprintf(buffer, "Writer %d", id);
    while(1){
        down(&db, buffer);
        
        FILE *file = fopen("criticalSection.txt", "w");
        printf("%s is writing.....\n", buffer);
        fprintf(file, "%s", w);
        fclose(file);
        printf("%s has completed writing.\n", buffer);

        up(&db, buffer);
        usleep(5000000);
    }
    return NULL;
}

int main(){
    pthread_t readers[3], writers[2];

    for(int i = 0; i < 3; i++){
        int *id = (int*)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader, id);
    }
    
    for(int i = 0; i < 2; i++){
        int *id = (int*)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer, id);
    }

    for(int i = 0; i < 3; i++){
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < 2; i++){
        pthread_join(writers[i], NULL);
    }

    return 0;
}