#include "main.h"

//queue에 process를 맨 뒤에 push
void Push(Process *process[], Process *addprocess, int *rear){
    process[*rear] = addprocess;
    *rear = (*rear+1)%(MaxProcess+1);
}

//남은 cpu burst가 작은 순서대로 정렬하며 push, 정렬된 queue에 사용 (SJF)
void PushByBurst(Process *process[], Process*addprocess, int *front, int *rear){
    int now = *front;
    // 앞에서 부터 차례대로 비교
    // addprocess의 remain_cpu_burst가 기존보다 작은 경우 break
    while (now != *rear){
        if (process[now]->remain_cpu_burst > addprocess->remain_cpu_burst){
            break;
        }
        now = (now+1)%(MaxProcess+1);
    }
    // 기존 process들을 한 칸씩 뒤로 이동시킴
    for (int i = *rear; i != now; i = (i-1)%(MaxProcess+1)){
        process[i] = process[(i-1)%(MaxProcess+1)];
    }
    // now위치에 addprocess를 삽입 후 rear를 한 칸 뒤로 이동(process 1개 증가)
    process[now] = addprocess;
    *rear = (*rear+1)%(MaxProcess+1);
}

//priority가 작은 순서대로 정렬하며 push, 정렬된 queue에 사용 (Priority)
//낮은 prioirty가 높은 우선순위를 가짐
void PushByPriority(Process *process[], Process*addprocess, int *front, int *rear){
    //앞에서 부터 차례대로 비교
    //addprocess의 prioirty가 기존보다 작은 경우 break
    int now = *front;
    while (now != *rear){
        if (process[now]->priority > addprocess->priority){
            break;
        }
        now = (now+1)%(MaxProcess+1);
    }
    //기존 process들을 한 칸씩 뒤로 이동시킴
    for (int i = *rear; i != now; i = (i-1)%(MaxProcess+1)){
        process[i] = process[(i-1)%(MaxProcess+1)];
    }
    //now위치에 addprocess를 삽입 후 rear를 한 칸 뒤로 이동(process 1개 증가)
    process[now] = addprocess;
    *rear = (*rear+1)%(MaxProcess+1);
}

//queue의 맨 앞에 있는 process를 pop
Process *deleteFront(Process *process[], int *front, int *rear){
    //queue가 비어있는 경우 NULL 반환
    if (*front == *rear){
        return NULL;
    }
    Process *temp = process[*front];
    process[*front] = NULL;
    *front = (*front+1)%(MaxProcess+1);
    return temp;
}

//index를 입력받아 해당 index에 있는 process를 pop
Process *delete(Process *process[], int *front, int *rear, int index){
    //queue가 비어있는 경우 NULL 반환
    if(*front==*rear){
        return NULL;
    }
    //index에 있는 process를 temp에 저장
    Process *temp = process[index];
    
    //index 뒤에 있는 process들을 한 칸씩 앞으로 이동시킴
    while(index != *rear){
        process[index] = process[(index+1)%(MaxProcess+1)];
        index = (index+1)%(MaxProcess+1);
    }
    *rear = (*rear-1)%(MaxProcess+1);
    return temp;
}