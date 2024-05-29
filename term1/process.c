#include "main.h"

// process 생성
Process *Create_Process(int pid){
    int i, io_burst_remain;
    Process *process = (Process *)malloc(sizeof(Process));
    //난수 생성
    srand(time(NULL)+pid*7);
    
    //변하지 않는 값
    process->pid = pid;
    process->cpu_burst = rand()%BurstMinus10+10;
    process->io_timing_num = rand()%(Io_Num+1);
    process->io_burst = process->io_timing_num != 0 ? rand()%BurstMinus10+10 : 0; // Io_Num이 0이면 io가 발생하지 않으므로 burst도 0
    process->arrival_time = rand()%BurstMinus10;
    process->priority = rand()%BurstMinus10;
    
    process->current_io_timing = 0;
    
    io_burst_remain = process->io_burst - process->io_timing_num; //각각의 io는 1이상의 값을 가질 것이므로 미리 빼놓음

    for (i=0; i<process->io_timing_num; i++){
        if(i==0)
            process->io_timing[i] = rand()%(process->cpu_burst-(process->io_timing_num-i))+(process->io_timing_num-i); // first io_timing < cpu_burst + 남아있는 io 발생횟수보다 커야하므로 (io_timing_num-i)를 더함
        else
            process->io_timing[i] = rand()%(process->io_timing[i-1]-(process->io_timing_num-i))+(process->io_timing_num-i); //  last io_timing <... < second io_timing < first io_timing < cpu_burst
    }
    for (i=0; i<process->io_timing_num-1; i++){
        process->io_each_burst[i] = rand()%(io_burst_remain+1); // 남아있는 io burst값보다 작거나 같은값중 랜덤으로 설정 
        io_burst_remain -= process->io_each_burst[i];
        process->io_each_burst[i]++; // 1이상으로 설정
    }
    process->io_each_burst[i] = io_burst_remain+1;

    //변할 수 있는 값
    process->remain_cpu_burst = process->cpu_burst;
    for (i=0; i<process->io_timing_num; i++){
        process->remain_io_each_burst[i] = process->io_each_burst[i];
    }
    process->status = 0;
    process->waiting_start = process->arrival_time;    
    process->waiting_time = 0;
    process->turnaround_time = 0;
    process->current_io_timing = 0;
    
    return process;
}

//변할 수 있는 값들을 기본으로 초기화
void initialize(Process *process){
    process->remain_cpu_burst = process->cpu_burst;
    for (int i=0; i<process->io_timing_num; i++){
        process->remain_io_each_burst[i] = process->io_each_burst[i];
    }
    process->status = 0;
    process->waiting_start = process->arrival_time;    
    process->waiting_time = 0;
    process->turnaround_time = 0;
    process->current_io_timing = 0;
}

// Process 정보 출력
void Print_Process(Process *process){
    printf("PID: %d\n", process->pid);
    printf("CPU Burst: %d\n", process->cpu_burst);
    printf("IO Burst: %d\n", process->io_burst);
    printf("Arrival Time: %d\n", process->arrival_time);
    printf("Priority: %d\n", process->priority);
    printf("Waiting Time: %d\n", process->waiting_time);
    printf("Turnaround Time: %d\n", process->turnaround_time);
    printf("Status: %d\n", process->status);
    printf("Waiting Start: %d\n", process->waiting_start);
    printf("IO Timing Num: %d\n", process->io_timing_num);
    for (int i=0; i<process->io_timing_num; i++){
        printf("IO Timing %d: %d\n", i, process->io_timing[i]);
    }
    for (int i=0; i<process->io_timing_num; i++){
        printf("IO Each Burst %d: %d\n", i, process->io_each_burst[i]);
    }

    printf("\n");

    return;
}