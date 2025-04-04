#include<stdio.h>
typedef struct {
    int id;
    int at;
    int bt;
    int tat;
    int cm;
    float rr;
    int exe;
} Process;

float getRR(Process p, int ct){
    return (float)(ct - p.at + p.bt) / p.bt;
}

int findHRR(Process p[], int n, int ct){
    int index = -1;
    float maxRR = -1.0;
    for(int i = 0; i < n; i++){
        if(!p[i].exe && p[i].at <= ct){
            p[i].rr = getRR(p[i], ct);
            if(p[i].rr > maxRR){
                maxRR = p[i].rr;
                index = i;
            }
        }
    }
    return index;
}

void hrrn(Process proc[], int n){
    Process p[n];
    for(int i = 0; i < n; i++){
        p[i].at = proc[i].at;
        p[i].bt = proc[i].bt;
        p[i].exe = 0;
    }
    int c = 0, ct = 0;
    float wat = 0.0, tat = 0.0;
    while(c < n){
        int index = findHRR(p, n, ct);
        if(index == -1){
            ct++;
            continue;
        }
        p[index].exe = 1;
        ct += p[index].bt;
        wat += ct - p[index].at - p[index].bt;
        c++;
    }
    printf("%.2f\n", (wat/n));
    return;
}


void sjf(Process proc[], int n){
    Process p[n];
    for(int i = 0; i < n; i++){
        p[i].at = proc[i].at;
        p[i].bt = proc[i].bt;
        p[i].exe = 0;
    }
    int ct = 0;
    float wat = 0;
    int c = 0;
    while (c < n){
        int mbt = 100000;
        int index = -1;
        for(int i = 0; i < n; i++){
            if(!p[i].exe && p[i].at <= ct && p[i].bt < mbt){
                mbt = p[i].bt;
                index = i;
            }
        }
        if(index == -1){
            ct += 1;
            continue;
        }
        ct += p[index].bt;
        wat += ct - p[index].bt - p[index].at;
        p[index].exe = 1;
        c++;
    }
    printf("%.2f\n", (wat/n) );
    return;
}
void ljf(Process proc[], int n){
    Process p[n];
    for(int i = 0; i < n; i++){
        p[i].at = proc[i].at;
        p[i].bt = proc[i].bt;
        p[i].exe = 0;
    }
    int ct = 0;
    float wat = 0;
    int c = 0;
    while (c < n){
        int mbt = -1;
        int index = -1;
        for(int i = 0; i < n; i++){
            if(!p[i].exe && p[i].at <= ct && p[i].bt > mbt){
                mbt = p[i].bt;
                index = i;
            }
        }
        if(index == -1){
            ct += 1;
            continue;
        }
        ct += p[index].bt;
        wat += ct - p[index].bt - p[index].at;
        p[index].exe = 1;
        c++;
    }
    printf("%.2f\n", (wat/n) );
    return;
}

void fcfs(Process proc[], int n){
    Process p[n];
    for(int i = 0; i < n; i++){
        p[i].at = proc[i].at;
        p[i].bt = proc[i].bt;
        p[i].exe = 0;
    }
    int ct = 0;
    float wat = 0;
    int c = 0;
    while (c < n){
        int mat = 100000;
        int index = -1;
        for(int i = 0; i < n; i++){
            if(!p[i].exe && p[i].at <= ct && p[i].bt < mat){
                mat = p[i].at;
                index = i;
            }
        }
        if(index == -1){
            ct += 1;
            continue;
        }
        // printf("%d\n", index);
        ct += p[index].bt;
        wat += ct - p[index].bt - p[index].at;
        p[index].exe = 1;
        c++;
    }
    printf("%.2f\n", (wat/n) );
    return;
}

int main(){
    int n;
    scanf("%d", &n);
    Process proc[n];
    for(int i = 0; i < n; i++){
        proc[i].id = i + 1;
        scanf("%d", &proc[i].at);
    }
    for(int i =0; i < n; i++){
        scanf("%d", &proc[i].bt);
        proc[i].exe = 0;
    }
    fcfs(proc, n);
    sjf(proc, n);
    ljf(proc, n);
    hrrn(proc, n);
    return 0;
}