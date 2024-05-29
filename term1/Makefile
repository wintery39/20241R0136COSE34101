cpuscheduler : main.o process.o schedule.o queue.o ganttchart.o evaluator.o
	gcc main.o process.o schedule.o queue.o ganttchart.o evaluator.o -o cpu_scheduler

main.o : main.c
	gcc -c main.c -o main.o

process.o : process.c
	gcc -c process.c -o process.o

schedule.o : schedule.c
	gcc -c schedule.c -o schedule.o

linkedqueue.o : queue.c
	gcc -c queue.c -o queue.o

ganttchar.o : ganttchart.c
	gcc -c ganttchart.c -o ganttchart.o

evaluator.o : evaluator.c
	gcc -c evaluator.c -o evaluator.o

clean:
	rm -rf *o COSE341	