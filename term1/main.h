#define MaxProcess 100
#define Io_Num 3
#define BurstMinus10 5
#define Schedule_num 11
#define Priority_max 5

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

typedef struct Process{
    // 변하지 않는 값
    int pid; // porcess id
    int cpu_burst; // cpu burst time
    int io_burst; // io burst time
    int arrival_time; // arrival time
    int priority; // priority (낮을 수록 우선순위가 높음)
    int io_timing_num; // io 발생 횟수
    int io_timing[Io_Num]; // 각 io 발생 시간
    int io_each_burst[Io_Num]; // 각 io burst time

    // 변할 수 있는 값
    int remain_cpu_burst; // 남은 cpu burst time
    int remain_io_each_burst[Io_Num]; // 남은 각 io burst time
    int waiting_start; // waiting 시작 시간 (ready queue에 들어간 시간)
    int status; // 0: new, 1: ready, 2: running, 3: waiting, 4: terminated
    int waiting_time; // waiting time
    int turnaround_time; // turnaround time
    int current_io_timing; // 현재 io_timing, io_each_burst의 index
} Process;

// scheduling에 사용되는 함수들 나열
Process *Create_Process(int pid);
void Print_Process(Process *process);
void initialize(Process *process);
void Schedule(Process *pd[], int process_count, int n, int timequantum);

void Push(Process *process[], Process *addprocess, int *rear);
void PushByBurst(Process *process[], Process*addprocess, int *front, int *rear);
Process *deleteFront(Process *process[], int *front, int *rear);
Process *delete(Process *process[], int *front, int *rear, int index);
void PushByPriority(Process *process[], Process*addprocess, int *front, int *rear);

void Io_Operation(Process *readyqueue[], Process *waitqueue[], int *readyrear, int *waitfront, int *waitrear, int current_time);
void Io_OperationByBurst(Process *readyqueue[], Process *waitqueue[], int *readyfront, int *readyrear, int *waitfront, int *waitrear, int current_time);
void Io_OperationByPriority(Process *readyqueue[], Process *waitqueue[], int *readyfront, int *readyrear, int *waitfront, int *waitrear, int current_time);


void addGantt(Process *process, int time);
void printGantt(int time);

void add_wait_turnaround_time(Process *process[], int process_count, int n);
void evaluation();