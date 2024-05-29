#include "main.h"

double average_waittime[Schedule_num];
double average_turnaroundtime[Schedule_num];

char *schedule[Schedule_num] = {"FCFS", "SJF", "SJF_Preemptive", "Priority", "Priority_Preemptive", "RR", "MLQ", "MLFQ"};

void add_wait_turnaround_time(Process *process[], int process_count, int n){
    //n -> FCFS = 1, SJF = 2, SJF_Preemptive = 3, Priority = 4, Priority_Preemptive = 5, RR = 6
    n--;

    //process별 waiting time, turnaround time을 각각 더함
    for(int i=0;i<process_count;i++){
        average_waittime[n] += process[i]->waiting_time;
        average_turnaroundtime[n] += process[i]->turnaround_time;
    }

    //process수로 나누어 평균을 구함
    average_waittime[n] /= process_count;
    average_turnaroundtime[n] /= process_count;

    printf(" Average Waiting Time of Schedule %s : %.2lf\n", schedule[n], average_waittime[n]);
    printf(" Average Turnaround Time of Schedule %s : %.2lf\n", schedule[n], average_turnaroundtime[n]);
    printf("\n\n");
}

void evaluation(){
    

    //average waiting time과 average turnaround time을 출력
    for(int i=0;i<Schedule_num;i++){
        printf(" Average Waiting Time of Schedule %s : %.2lf\n", schedule[i], average_waittime[i]);
        printf(" Average Turnaround Time of Schedule %s : %.2lf\n", schedule[i], average_turnaroundtime[i]);
        printf("\n");
    }

    
    int index;

    //average waiting time이 작은 순으로 출력
    printf("Schedule in order of average waiting time\n");
    for(int i=0; i<Schedule_num; i++){
        index = 0;
        for(int j=1; j<Schedule_num; j++){
            if (average_waittime[index] == -1 && average_waittime[j] != -1)
                index = j;
            else if(average_waittime[index] > average_waittime[j] && average_waittime[j] != -1)
                index = j;        
        }
        average_waittime[index] = -1;
        if (i == 0)
            printf("%s", schedule[index]);
        else
            printf(" < %s", schedule[index]);
    }
    printf("\n\n");

    //average turnaround time이 작은 순으로 출력
    printf("Schedule in order of average turnaround time\n");
    for(int i=0; i<Schedule_num; i++){
        index = 0;
        for(int j=1; j<Schedule_num; j++){
            if (average_turnaroundtime[index] == -1 && average_turnaroundtime[j] != -1)
                index = j;
            else if(average_turnaroundtime[index] > average_turnaroundtime[j] && average_waittime[j] != -1)
                index = j;        
        }
        average_turnaroundtime[index] = -1;
        if (i == 0)
            printf("%s", schedule[index]);
        else
            printf(" < %s", schedule[index]);
    }
    printf("\n\n");
}