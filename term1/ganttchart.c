#include "main.h"

int arr[MaxProcess*(BurstMinus10+15)];

//gantt chart에 process pid정보 추가
void addGantt(Process *process, int time){
    arr[time] = process->pid;
}

void printGantt(int time){
    int norm = 20; //gantt chart를 한줄에 몇개씩 출력할지
    for(int i=0; i<=(time/norm); i++){
        for(int j=0; j<norm; j++){
            //gantt chart를 다 그렸으면 종료
            if (i*norm+j >= time){
                printf("|");
                break;
            }
            //idle인 경우 idle을 출력 
            if (arr[i*norm+j] == 0){
                if(i*norm+j == 0 || arr[i*norm+j-1] != 0)
                    printf("|idle ");
                else 
                    printf("      ");
                continue;
            }
            //process인 경우 process의 pid를 출력
            else{
                if(i*norm+j == 0 || arr[i*norm+j-1] != arr[i*norm+j])
                    printf("|P%-4d", arr[i*norm+j]);
                else
                    printf("      ");
            }
        }
        printf("\n");
        for(int j=0; j<norm; j++){
            //gantt chart를 다 그렸으면 종료
            if (i*norm+j >= time){
                printf("|");
                break;
            }
            //눈금을 그림
            printf("|-----");
        }
        printf("\n");
        for(int j=0; j<norm; j++){
            //gantt chart를 다 그렸으면 종료
            if (i*norm+j >= time){
                printf("%d", i*norm+j);
                break;
            }
            //시간을 출력
            printf("%-6d", i*norm+j);
        }
        printf("\n");
        printf("\n");
    }

    //정보 초기화
    for(int i=0;i<time;i++)
        arr[i]=0;
}