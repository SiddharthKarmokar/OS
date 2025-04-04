#include<stdio.h>
#include <limits.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct {
    int at;
    int bt;
    int rt;
    int wt;
    int tat;
    bool complete;
} Process;

void hrrn(Process *P, int n){
    int completed = 0, time = 0;
    // printf("Sorted:\n");
    for(int  i = 0; i < n; i++){
        // printf("%d ", P[i].at);
        P[i].wt = 0;
        P[i].complete = false;
    }
    // printf("\n");
    while(completed < n){
        Process *p = NULL;
        float Hrrn = 0;
        for(int i = 0; i < n; i++){
            float ratio = (P[i].wt + P[i].bt) / (float)(P[i].bt);
            // printf("ratio of %d at time %d is %.2f\n", P[i].at, time, ratio);
            if( (p == NULL && !P[i].complete && P[i].at <= time) || (!P[i].complete) && (P[i].at <= time) && (Hrrn < ratio)){
                p = &P[i];
                Hrrn = ratio;
            }
        }
        if(p == NULL){
            time++;
            continue;
        }
        p->complete = true;
        p->tat = time - p->at;
        // printf("%d ", p->at);
        completed++;
        time += p->bt;
        for(int i = 0; i < n; i++){
            if(!P[i].complete && time >= P[i].at){
                P[i].wt = time - P[i].at;
            }
        }
    }
    float waiting = 0;
    for(int i = 0; i < n; i++){
        waiting += P[i].wt;
    }
    printf("HRRN: %.2f", (waiting/(float)n));
}

int compare(const void*a, const void*b){
    Process* A = (Process*)a;
    Process* B = (Process*)b;
    return A->at - B->at;
}

int main(){
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    int n;
    scanf("%d", &n);
    Process P[n];
    for(int i = 0; i < n; i++){
        scanf("%d", &P[i].at);
    }
    for(int i = 0; i < n; i++){
        scanf("%d", &P[i].bt);
        P[i].rt = P[i].bt;
    }
    qsort(P, n, sizeof(Process), compare);
    hrrn(P, n);
    return 0;
}