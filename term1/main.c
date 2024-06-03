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
/*
    //테스트용 process
    pd[0] -> cpu_burst = 10;
    pd[0] -> io_burst = 10;
    pd[0] -> arrival_time = 3;
    pd[0] -> priority = 2;
    pd[0] -> io_timing_num = 2;
    pd[0] -> io_timing[0] = 3;
    pd[0] -> io_timing[1] = 2;
    pd[0] -> io_each_burst[0] = 7;
    pd[0] -> io_each_burst[1] = 3;
    pd[0] -> remain_cpu_burst = 10;
    pd[0] -> remain_io_each_burst[0] = 7;
    pd[0] -> remain_io_each_burst[1] = 3;

    pd[1] -> cpu_burst = 10;
    pd[1] -> io_burst = 0;
    pd[1] -> arrival_time = 4;
    pd[1] -> priority = 4;
    pd[1] -> io_timing_num = 0;
    pd[1] -> remain_cpu_burst = 10;

    pd[2] -> cpu_burst = 10;
    pd[2] -> io_burst = 11;
    pd[2] -> arrival_time = 0;
    pd[2] -> priority = 3;
    pd[2] -> io_timing_num = 3;
    pd[2] -> io_timing[0] = 9;
    pd[2] -> io_timing[1] = 8;
    pd[2] -> io_timing[2] = 4;
    pd[2] -> io_each_burst[0] = 5;
    pd[2] -> io_each_burst[1] = 3;
    pd[2] -> io_each_burst[2] = 3;
    pd[2] -> remain_cpu_burst = 10;
    pd[2] -> remain_io_each_burst[0] = 9;
    pd[2] -> remain_io_each_burst[1] = 8;
    pd[2] -> remain_io_each_burst[2] = 4;

    pd[3] -> cpu_burst = 13;
    pd[3] -> io_burst = 14;
    pd[3] -> arrival_time = 2;
    pd[3] -> priority = 2;
    pd[3] -> io_timing_num = 1;
    pd[3] -> io_timing[0] = 4;
    pd[3] -> io_each_burst[0] = 14;
    pd[3] -> remain_cpu_burst = 13;
    pd[3] -> remain_io_each_burst[0] = 14;

    pd[4] -> cpu_burst = 13;
    pd[4] -> io_burst = 12;
    pd[4] -> arrival_time = 1;
    pd[4] -> priority = 3;
    pd[4] -> io_timing_num = 3;
    pd[4] -> io_timing[0] = 4;
    pd[4] -> io_timing[1] = 2;
    pd[4] -> io_timing[2] = 1;
    pd[4] -> io_each_burst[0] = 4;
    pd[4] -> io_each_burst[1] = 4;
    pd[4] -> io_each_burst[2] = 4;
    pd[4] -> remain_cpu_burst = 13;
    pd[4] -> remain_io_each_burst[0] = 12;
*/

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