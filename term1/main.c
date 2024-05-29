#include "main.h"


int main(void){
    int process_count;
    int timequantum;

    Process* pd[MaxProcess];
    
    //process수를 입력받음
    printf("Process Number : ");
    scanf("%d",&process_count);
    if (process_count>MaxProcess){
        printf("Too many processes\n");
        return 0;
    }

    //RR에 사용될 time quantum을 입력받음
    printf("Time Quantum : ");
    scanf("%d",&timequantum);
    printf("\n");

    //process를 생성하고 출력
    for(int i=0;i<process_count;i++){
        pd[i] = Create_Process(i+1);
    }
    for(int i=0;i<process_count;i++){
        Print_Process(pd[i]);
    }

    //각 스케줄링 알고리즘을 실행
    for(int n = 1; n <= Schedule_num ; n++){
        //FCFS = 1, SJF = 2, SJF_Preemptive = 3, Priority = 4, Priority_Preemptive = 5, RR = 6
        for(int i=0;i<process_count;i++){
            initialize(pd[i]);
        }
        Schedule(pd, process_count, n, timequantum);
        
        //각 스케줄링 알고리즘의 average waiting time과 average turnaround time을 계산하고 저장
        add_wait_turnaround_time(pd, process_count, n);
    }
    //각 스케줄링 알고리즘의 waiting time과 turnaround time을 출력
    printf("--------------------------------------------\n\n");
    evaluation();
    return 0;
}