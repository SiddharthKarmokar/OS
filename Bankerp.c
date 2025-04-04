#include<stdio.h>
#include<stdbool.h>

#define P 4
#define R 3

bool is_safe_state(int alloc[P][R], int max[P][R], int available[R]){
    int need[P][R], work[R];
    for(int i = 0; i < P; i++){
        for(int j = 0; j < R; j++){
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    for(int i = 0; i < R; i++){
        work[i] = available[i];
    }
    int count = 0;
    int finish[R] = {false};
    while(count < P){
        bool found = false;
        for(int i = 0; i < P; i++){
            if(!finish[i]){
                bool can_alloc = true;
                for(int j = 0; j < R; j++){
                    if(need[i][j] > work[i]){
                        can_alloc = false;
                    }
                }   
                if(can_alloc){
                    for(int j = 0; j < R; j++){
                        work[j] = work[j] + alloc[i][j];
                    }
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        if(!found){
            return false;
        }
    }
    return true;
}   


int main() {
    int allocation[P][R], maximum[P][R], available[R];
    FILE *file = fopen("in.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }
    
    // Read Allocation matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            fscanf(file, "%d", &allocation[i][j]);
        }
    }
    
    // Read Maximum matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            fscanf(file, "%d", &maximum[i][j]);
        }
    }
    
    // Read Available resources
    for (int i = 0; i < R; i++) {
        fscanf(file, "%d", &available[i]);
    }
    
    fclose(file);
    
    if (!is_safe_state(allocation, maximum, available)) {
        printf("System is not in a safe state.\n");
    }else{
        printf("safe\n");
    }
    
    return 0;
}
