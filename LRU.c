#include<stdio.h>
#include<limits.h>

int main(){
    int frames, pages, page_faults = 0;
    scanf("%d %d", &frames, &pages);
    int page_demand[pages], memory[frames], last_used[frames];
    for(int i = 0; i < pages; i++){
        scanf("%d", &page_demand[i]);
    }
    for(int i = 0; i < frames; i++){
        memory[i] = -1;
        last_used[i] = -1;
    }
    for(int i =0 ; i < pages; i++){
        int found = 0;
        for(int j = 0; j < frames; j++){
            if(memory[j] == page_demand[i]){
                last_used[j] = i;
                found = 1;
                break;
            }
        }
        if(!found){
            int lru_index = -1, min_last_used = INT_MAX;
            for(int j = 0; j < frames; j++){
                if(memory[j] == -1){
                    lru_index = j;
                    break;
                }
                
                if(last_used[j] < min_last_used){
                    min_last_used = last_used[j];
                    lru_index = j;
                }
            }
            memory[lru_index] = page_demand[i];
            last_used[lru_index] = i;
            page_faults++;
            
        }
    }
    printf("%d\n", page_faults);
    return 0;
}