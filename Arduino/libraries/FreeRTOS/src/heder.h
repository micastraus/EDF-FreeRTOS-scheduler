#ifndef HEDER_H
#define HEDER_H


// EDF
#define STACK_SIZE 128
typedef struct MyTCB {
	char* naziv;
	TickType_t a;
	TickType_t c;
	TickType_t d;
	TickType_t p;
	TaskFunction_t kod;
	StackType_t* stek;
	StaticTask_t* hendl;
} MyTCB;

MyTCB myTCBs[4];

void task1(void* p);
void task2(void* p);
void task3(void* p);
void task4(void* p);
void task5(void* p);
void saveTCB(TaskFunction_t pxTaskCode,
                                    const char * const pcName,
                                    StackType_t * const puxStackBuffer,
                                    StaticTask_t * const pxTaskBuffer ,
									TickType_t uxArrival, 
									TickType_t uxCount,	
									TickType_t uxDeadline,
									TickType_t uxPeriod);
// EDF

#endif