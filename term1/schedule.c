#include "main.h"

void FCFS(Process* pd[], int process_count);
void SJF(Process* pd[], int process_count);
void SJF_Preemptive(Process* pd[], int process_count);
void Priority(Process* pd[], int process_count);
void Priority_Preemptive(Process* pd[], int process_count);
void RR(Process* pd[], int process_count, int timequantum);
void MLQ(Process* pd[], int process_count, int timequantum);
void MLFQ(Process* pd[], int process_count);
void Lottery(Process* pd[], int process_count);
void HRRN(Process* pd[], int process_count);
void Stride(Process* pd[], int process_count);

//n에 따라 schedule할당
//n = FCFS : 1, SJF : 2, SJF_Preemptive : 3, Priority : 4, Priority_Preemptive : 5, Round Robin : 6, MLQ : 7, MLFQ : 8, Lottery : 9, HRRN : 10, Stride : 11
void Schedule(Process *pd[], int process_count, int n, int timequantum){
    switch (n){
        case 1:
            printf("FCFS\n");
            FCFS(pd, process_count);
            break;
        case 2:
            printf("SJF\n");
            SJF(pd, process_count);
            break;
        case 3:
            printf("SJF_Preemptive\n");
            SJF_Preemptive(pd, process_count);
            break;
        case 4:
            printf("Priority\n");
            Priority(pd, process_count);
            break;
        case 5:
            printf("Priority_Preemptive\n");
            Priority_Preemptive(pd, process_count);
            break;
        case 6:
            printf("Round Robin\n");
            RR(pd, process_count, timequantum);
            break;
        case 7:
            printf("MLQ\n");
            MLQ(pd, process_count, timequantum);
            break;
        case 8:
            printf("MLFQ\n");
            MLFQ(pd, process_count);
            break;
        case 9:
            printf("Lottery\n");
            Lottery(pd, process_count);
            break;
        case 10:
            printf("HRRN\n");
            HRRN(pd, process_count);
            break;
        case 11:
            printf("Stride\n");
            Stride(pd, process_count);
            break;
        default:
            printf("Wrong input\n");
            break;
    }

    return;
}

void FCFS(Process* pd[], int process_count){
    int terminated = 0; //terminate된 process 수
    int current_time = 0; //현재 시간
    int new = process_count; //new status인 process 수
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue로 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                //status가 new이고 arrival time이 current time보다 작거나 같은 경우 ready queue로 이동
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue, pd[i], &readyrear);
                    new--;
                }
            }
        }
 
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정
        
        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_Operation(readyqueue, waitqueue, &readyrear, &waitfront, &waitrear, current_time+1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동 
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

void SJF(Process* pd[], int process_count){
    int terminated = 0; //terminate된 process 수
    int current_time = 0; //현재 시간
    int new = process_count; //new status인 process 수
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 CPU_Burst time으로 정렬해 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    PushByBurst(readyqueue, pd[i], &readyfront, &readyrear);
                    new--;
                }
            }
        }

        
        
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정
        
        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_OperationByBurst(readyqueue, waitqueue, &readyfront, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);
    return;
}

void SJF_Preemptive(Process* pd[], int process_count){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 CPU_Burst time으로 정렬해 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    PushByBurst(readyqueue, pd[i], &readyfront, &readyrear);
                    new--;
                }
            }
        }

        
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }
        // 만약 running이 NULL이면, running중인 process의 remain_cpu_burst과 ready queue 맨 앞에 있는 process의 remain_cpu_burst를 비교 
        else if (readyfront != readyrear && running != NULL && readyqueue[readyfront]->remain_cpu_burst < running->remain_cpu_burst){
            // ready queue 맨 앞에 있는 process가 running process의 remain_cpu_burst보다 작으므로
            // running process를 ready queue로 보내고 ready queue 맨 앞에 있는 process가 running을 시작
            // running -> ready queue
            running->status = 1;
            running->waiting_start = current_time;
            PushByBurst(readyqueue, running, &readyfront, &readyrear);
            // ready queue -> running
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정

        // I/O operation 실행
        // 만약 I/O burst가 0이 되면, 해당 process를 ready queue에 remain_cpu_burst기준으로 정렬해 이동시킴
        Io_OperationByBurst(readyqueue, waitqueue, &readyfront, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);
    return;
}

//Priority가 작을수록 높은 우선순위를 가짐
void Priority(Process* pd[], int process_count){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 Priority로 정렬해 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    PushByPriority(readyqueue, pd[i], &readyfront, &readyrear);
                    new--;
                }
            }
        }

        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정

        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue에 Priority로 정렬해  이동시킴
        Io_OperationByPriority(readyqueue, waitqueue, &readyfront, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);
    return;
}

//Priority가 작을수록 높은 우선순위를 가짐
void Priority_Preemptive(Process* pd[], int process_count){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 CPU_Burst time으로 정렬해 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    PushByPriority(readyqueue, pd[i], &readyfront, &readyrear);
                    new--;
                }
            }
        }

        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }
        // 만약 running이 NULL이면, running중인 process의 Priority와 ready queue 맨 앞에 있는 process의 Priority를 비교  
        else if (readyfront != readyrear && running != NULL && readyqueue[readyfront]->priority < running->priority){
            // ready queue 맨 앞에 있는 process가 running process의 priority보다 작으므로
            // running process를 ready queue로 보내고 ready queue 맨 앞에 있는 process가 running을 시작
            // running -> ready queue
            running->status = 1;
            running->waiting_start = current_time;
            PushByPriority(readyqueue, running, &readyfront, &readyrear);
            // ready queue -> running
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정

        // I/O operation 실행
        // 만약 I/O burst가 0이 되면, 해당 process를 ready queue에 priority기준으로 정렬해 이동시킴
        Io_OperationByPriority(readyqueue, waitqueue, &readyfront, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);
    return;
}

void RR(Process* pd[], int process_count, int timequantum){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    int timequantum_count = 0;

    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue, pd[i], &readyrear);
                    new--;
                }
            }
        }
 
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        // 새로운 process가 들어왔으므로 timequantum_count 초기화
        if (readyfront != readyrear && running == NULL){
            running = deleteFront(readyqueue, &readyfront, &readyrear);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
            timequantum_count = 0;
        }

        // 1초 지났다고 가정

        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_Operation(readyqueue, waitqueue, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            timequantum_count++;
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
            //time quantum이 다 되었을 때 ready queue로 이동
            else if (timequantum_count == timequantum){
                running->status = 1;
                running->waiting_start = current_time;
                Push(readyqueue, running, &readyrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

// foreground queue(RR) 우선순위가 높은 process 배정 / 80%
// background queue(FCFS) 우선순위가 낮은 process 배정 / 20%
void MLQ(Process* pd[], int process_count, int timequantum){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    int timequantum_count = 0;
    int current_queue;

    int turn = 0;

    Process *readyqueue[2][MaxProcess+1];
    Process *waitqueue[2][MaxProcess+1];
    int readyfront[] = {0,0};
    int readyrear[] = {0,0};
    int waitfront[] = {0,0};
    int waitrear[] = {0,0};
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 priority에 따라 다른 ready queue에 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    if (pd[i]->priority > (Priority_max/2))
                        Push(readyqueue[1], pd[i], &readyrear[1]);
                    else
                        Push(readyqueue[0], pd[i], &readyrear[0]);
                    new--;
                }
            }
        }

        // foreground queue 80%, background queue 20%
        if(current_time % 10 >= 8)
            turn = 1;
        else
            turn = 0;
 
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        // 새로운 process가 들어왔으므로 timequantum_count 초기화
        for(int i=turn;i<2+turn;i++){
            if (readyfront[i%2] != readyrear[i%2] && running == NULL){
                running = deleteFront(readyqueue[i%2], &readyfront[i%2], &readyrear[i%2]);
                running->status = 2;
                running->waiting_time += current_time - running->waiting_start;
                timequantum_count = 0;
                current_queue = i%2;
            }
        }

        // turun이 아닌 process가 실행중이고 turn의 readyqueue가 비어있지 않다면, process switch 발생
        if(current_queue != turn && readyfront[turn] != readyrear[turn]){
            // running중인 process는 ready queue로 이동
            running->status = 1;
            running->waiting_start = current_time;
            Push(readyqueue[current_queue], running, &readyrear[current_queue]);
            // turn에 맞춰 process 추가
            running = deleteFront(readyqueue[turn], &readyfront[turn], &readyrear[turn]);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
            timequantum_count = 0;
            current_queue = turn;
        }

        // 1초 지났다고 가정
        for(int i = 0;i<2;i++){
            // I/O operation 실행
            // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
            Io_Operation(readyqueue[i], waitqueue[i], &readyrear[i], &waitfront[i], &waitrear[i], current_time + 1);
        }

        // running
        if(running != NULL){
            timequantum_count++;
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue[current_queue], running, &waitrear[current_queue]);
                running = NULL;
            }
            //time quantum이 다 되었을 때 ready queue로 이동
            else if (current_queue == 0 && timequantum_count == timequantum){
                running->status = 1;
                running->waiting_start = current_time;
                Push(readyqueue[current_queue], running, &readyrear[current_queue]);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

// RR 두 번 실행후 FCFS 실행
void MLFQ(Process* pd[], int process_count){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    int timequantum_count = 0;
    int current_queue;

    int timequantum[] = {1, 2};

    Process *readyqueue[3][MaxProcess+1];
    Process *waitqueue[3][MaxProcess+1];
    int readyfront[] = {0,0,0};
    int readyrear[] = {0,0,0};
    int waitfront[] = {0,0,0};
    int waitrear[] = {0,0,0};
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue[0], pd[i], &readyrear[0]);
                    new--;
                }
            }
        }
 
        // ready -> running
        // 만약 running이 NULL이면, ready queue 맨 앞에 있는 process가 running을 시작
        // 새로운 process가 들어왔으므로 timequantum_count 초기화
        for(int i=0;i<3;i++){
            if (readyfront[i] != readyrear[i] && running == NULL){
                running = deleteFront(readyqueue[i], &readyfront[i], &readyrear[i]);
                running->status = 2;
                running->waiting_time += current_time - running->waiting_start;
                timequantum_count = 0;
                current_queue = i;
            }
        
            // 1초 지났다고 가정

            // I/O operation 실행
            // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
            Io_Operation(readyqueue[i == 2 ? 2 : i + 1], waitqueue[i], &readyrear[i == 2 ? 2 : i + 1], &waitfront[i], &waitrear[i], current_time + 1);
        }
            
        // running
        if(running != NULL){
            timequantum_count++;
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue[current_queue], running, &waitrear[current_queue]);
                running = NULL;
            }
            //time quantum이 다 되었을 때 ready queue로 이동
            else if (current_queue != 2 && timequantum_count == timequantum[current_queue]){
                running->status = 1;
                running->waiting_start = current_time;
                Push(readyqueue[current_queue+1], running, &readyrear[current_queue+1]);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

// prioirty를 기준으로g lottery를 수행
void Lottery(Process* pd[], int process_count){
    int terminated = 0;
    int current_time = 0;
    int new = process_count;
    int winner;
    int lottery_sum = 0;
    int lottery[MaxProcess];
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;

    
    Process *running = NULL;

    for(int i = 0; i< process_count; i++){
        lottery[pd[i]->pid-1] = Priority_max - pd[i]->priority;
    }
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue에 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue, pd[i], &readyrear);
                    new--;
                }
            }
        }
        lottery_sum = 0;
        for(int i=readyfront; i!=readyrear; i=(i+1)%(MaxProcess+1)){
            lottery_sum += lottery[readyqueue[i]->pid-1];
        }
        // ready -> running
        if (readyfront != readyrear){
            winner = rand()%lottery_sum;
            for(int i=readyfront; i!=readyrear; i=(i+1)%(MaxProcess+1)){
                
                winner -= lottery[readyqueue[i]->pid-1];
                if(winner < 0){
                    running = delete(readyqueue, &readyfront, &readyrear, i);
                    running->status = 2;
                    running->waiting_time += current_time - running->waiting_start;      
                    break;
                }
            }
        }

        // 1초 지났다고 가정
  
        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_Operation(readyqueue, waitqueue, &readyrear, &waitfront, &waitrear, current_time + 1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
            //process를 ready queue로 이동 
            else{
                running->status = 1;
                running->waiting_start = current_time+1;
                Push(readyqueue, running, &readyrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

void HRRN(Process* pd[], int process_count){
    int terminated = 0; //terminate된 process 수
    int current_time = 0; //현재 시간
    int new = process_count; //new status인 process 수
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;

    int idx;
    
    Process *running = NULL;
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue로 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                //status가 new이고 arrival time이 current time보다 작거나 같은 경우 ready queue로 이동
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue, pd[i], &readyrear);
                    new--;
                }
            }
        }
 
        // ready -> running
        // 만약 running이 NULL이면, 응답률이 제일 높은 process가 running을 시작
        // 응답률 : (waiting time + cpu burst) / cpu burst
        if (readyfront != readyrear && running == NULL){
            idx = readyfront;
            // 가장 응답률이 높은 process의 index 찾기
            for(int i=(readyfront+1)%(MaxProcess+1); i!=readyrear; i = i+1%(MaxProcess+1)){
                if((double)(readyqueue[i]-> remain_cpu_burst + (current_time - readyqueue[i] -> waiting_start + readyqueue[i]->waiting_time))/ readyqueue[i]->remain_cpu_burst 
                > (double)(readyqueue[idx]-> remain_cpu_burst + (current_time - readyqueue[idx] -> waiting_start + readyqueue[idx]->waiting_time))/ readyqueue[idx]->remain_cpu_burst){
                    idx = i;
                }
            }
            // running으로 이동
            running = delete(readyqueue, &readyfront, &readyrear, idx);
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정
        
        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_Operation(readyqueue, waitqueue, &readyrear, &waitfront, &waitrear, current_time+1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동 
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}

void Stride(Process* pd[], int process_count){
    int terminated = 0; //terminate된 process 수
    int current_time = 0; //현재 시간
    int new = process_count; //new status인 process 수
    
    Process *readyqueue[MaxProcess+1];
    Process *waitqueue[MaxProcess+1];
    int readyfront = 0;
    int readyrear = 0;
    int waitfront = 0;
    int waitrear = 0;

    int stride[MaxProcess];
    int pass[MaxProcess] = {0,};
    int idx;
    
    Process *running = NULL;

    // Priority 기반으로 stride 설정
    for(int i = 0; i< process_count; i++){
        stride[pd[i]->pid-1] = pd[i]->priority+1; // 0이 나오지 않기 위해 1을 더함
    }
    
    while (terminated != process_count){
        // new -> ready
        // 만약 arrival time <= current time, process를 ready queue로 이동시킴
        if (new){
            for (int i=0; i<process_count; i++){
                //status가 new이고 arrival time이 current time보다 작거나 같은 경우 ready queue로 이동
                if(pd[i]->status == 0 && (pd[i]->arrival_time) <= current_time){
                    pd[i]->status = 1;
                    pd[i]->waiting_start = current_time;
                    Push(readyqueue, pd[i], &readyrear);
                    new--;
                }
            }
        }
 
        // ready -> running
        // 만약 running이 NULL이면, pass값이 가장 낮은 process가 running을 시작
        if (readyfront != readyrear && running == NULL){
            idx = readyfront;
            // 가장 pass값이 낮은 process의 index 찾기
            for(int i=(readyfront+1)%(MaxProcess+1); i!=readyrear; i = i+1%(MaxProcess+1)){
                if(pass[readyqueue[i]->pid-1] < pass[readyqueue[idx]->pid-1]){ 
                    idx = i;
                }
            }
            // running으로 이동
            running = delete(readyqueue, &readyfront, &readyrear, idx);
            pass[running->pid-1] += stride[running->pid-1];
            running->status = 2;
            running->waiting_time += current_time - running->waiting_start;
        }

        // 1초 지났다고 가정
        
        // I/O operation 실행
        // 만약 remain each I/O burst가 0이면, process를 ready queue로 이동시킴
        Io_Operation(readyqueue, waitqueue, &readyrear, &waitfront, &waitrear, current_time+1);

        // running
        if(running != NULL){
            running->remain_cpu_burst--;
            addGantt(running, current_time); //for draw Gantt chart
            //cpu burst가 0이면 process terminate 시킴
            if (running->remain_cpu_burst == 0){
                running->turnaround_time = (current_time+1) - running->arrival_time;
                running->status = 4;
                terminated++;
                running = NULL;
            }
            //process를 I/O queue로 이동 
            else if (running->current_io_timing != running->io_timing_num && running->io_timing[running->current_io_timing] == running->remain_cpu_burst){
                running->status = 3;
                Push(waitqueue, running, &waitrear);
                running = NULL;
            }
        }
        current_time++;
    }
    //Gantt chart 출력
    printGantt(current_time);

    return;
}



void Io_Operation(Process *readyqueue[], Process *waitqueue[], int *readyrear, int *waitfront, int *waitrear, int current_time){
    int waitnums = *waitrear >= *waitfront ? *waitrear - *waitfront : (*waitrear + MaxProcess + 1) - *waitfront;
    for (int i = 0; i < waitnums ; i++){
        int current_io = waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing;
        waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io]--;
        if(waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io] == 0) {
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing++;
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->waiting_start = current_time;
            Push(readyqueue, delete(waitqueue, waitfront, waitrear, i), readyrear);
            i--; waitnums--; //waitqueue의 크기가 줄었으므로 i와 waitnums, waitrear를 1씩 줄여줘야함
        }
    }
}

void Io_OperationByBurst(Process *readyqueue[], Process *waitqueue[], int *readyfront, int *readyrear, int *waitfront, int *waitrear, int current_time){
    int waitnums = *waitrear >= *waitfront ? *waitrear - *waitfront : (*waitrear + MaxProcess + 1) - *waitfront;
    for (int i = 0; i < waitnums ; i++){
        int current_io = waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing;
        waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io]--;
        if(waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io] == 0) {
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing++;
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->waiting_start = current_time;
            PushByBurst(readyqueue, delete(waitqueue, waitfront, waitrear, i), readyfront, readyrear);
            i--; waitnums--; //waitqueue의 크기가 줄었으므로 i와 waitnums, waitrear를 1씩 줄여줘야함
        }
    }
}

void Io_OperationByPriority(Process *readyqueue[], Process *waitqueue[], int *readyfront, int *readyrear, int *waitfront, int *waitrear, int current_time){
    int waitnums = *waitrear >= *waitfront ? *waitrear - *waitfront : (*waitrear + MaxProcess + 1) - *waitfront;
    for (int i = 0; i < waitnums ; i++){
        int current_io = waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing;
        waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io]--;
        if(waitqueue[(*waitfront+i)%(MaxProcess+1)]->remain_io_each_burst[current_io] == 0) {
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->current_io_timing++;
            waitqueue[(*waitfront+i)%(MaxProcess+1)]->waiting_start = current_time;
            PushByPriority(readyqueue, delete(waitqueue, waitfront, waitrear, i), readyfront, readyrear);
            i--; waitnums--; //waitqueue의 크기가 줄었으므로 i와 waitnums, waitrear를 1씩 줄여줘야함
        }
    }
}