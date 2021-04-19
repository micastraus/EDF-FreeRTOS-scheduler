#include <Arduino_FreeRTOS.h>
#include "heder.h"

#define LEDS_LENGTH 5
#define JOBS_LENGTH 5

StackType_t task1Stack[STACK_SIZE];
StaticTask_t task1Handle;

StackType_t task2Stack[STACK_SIZE];
StaticTask_t task2Handle;

StackType_t task3Stack[STACK_SIZE];
StaticTask_t task3Handle;

StackType_t task4Stack[STACK_SIZE];
StaticTask_t task4Handle;

StackType_t task5Stack[STACK_SIZE];
StaticTask_t task5Handle;

volatile int leds[] = {2,4,7,8,12};

volatile char s[100];     
volatile int serSize = 0;
volatile int brojacPoslova = 0;
volatile boolean losRaspored = false;

MyTCB jobs[4];
typedef struct moja{
    char nejm[10];
} moja;

moja m[4];

void setup() {
  for (int i=0; i<LEDS_LENGTH; i++) {
    pinMode(leds[i], OUTPUT);
  }
  Serial.begin(9600); 

  // primer unosa na Serial
  // [0,60,150,t1];[0,120,210,t2]; rasporediv posao
  // [0,65,100,t1];[0,120,210,t2]; nerasporediv posao
//  parseInput();
  
    create2tasks();
//    create4tasks();
}

void task1(void* pvParameters) {
//  Serial.println(F("Pocetak 1"));
  while (true) {
      ledManeuver(leds[0]);
//    printCurrentTask();
  }
}

void task2(void* pvParameters) {
//   Serial.println(F("Pocetak 2"));
   while (true){
      ledManeuver(leds[1]);
//    printCurrentTask();
   }
}

void task3(void* pvParameters) {
   while (true){
      ledManeuver(leds[2]);
//    printCurrentTask();
   }
}

void task4(void* pvParameters) {
   while (true){
      ledManeuver(leds[3]);
//    printCurrentTask();
   }
}


void task5(void* pvParameters) {
   while (true){
//      ledManeuver(leds[4]);
//    printCurrentTask();
   }
}

void loop() {
  ledManeuver(200);
//  printCurrentTask();
}

void ledManeuver(int ledON) {
  for (int i=0; i<LEDS_LENGTH; i++) {
    ledON == leds[i] ? digitalWrite(leds[i], HIGH) : digitalWrite(leds[i], LOW);
  }
}

void printCurrentTask() {
    TaskHandle_t hend = xTaskGetCurrentTaskHandle();
    StaticTask_t* currTCB = (StaticTask_t*)hend;
    char *n = pcTaskGetName(hend);
    Serial.println(n);
    Serial.println(currTCB->uxCount);
}

void create2tasks() {
  xTaskCreateStatic(task1,"task1",STACK_SIZE,NULL,0,task1Stack,&task1Handle,0,60,150);
  xTaskCreateStatic(task2,"task2",STACK_SIZE,NULL,0,task2Stack,&task2Handle,0,120,210);
}

void create4tasks() {
  xTaskCreateStatic(task1,"task1",STACK_SIZE,NULL,0,task1Stack,&task1Handle,0,30,90);
  xTaskCreateStatic(task2,"task2",STACK_SIZE,NULL,0,task2Stack,&task2Handle,0,30,150);
  xTaskCreateStatic(task3,"task3",STACK_SIZE,NULL,0,task3Stack,&task3Handle,0,30,180);
  xTaskCreateStatic(task4,"task4",STACK_SIZE,NULL,0,task4Stack,&task4Handle,0,60,300);
}


void parseInput() {
  while (true) {

    serSize = 0;

    brojacPoslova = 0;

    losRaspored = false;

    while (! Serial.available()) { }

    while (true) {
      if ( Serial.available()  ) {
        s[serSize] = Serial.read();
        if (s[serSize] == '\n') break;
        serSize++;
      }
    }

    int i = 0;
    for (i = 0; i < serSize; i++) {
      Serial.print(s[i]);
    }
    Serial.println();

    // Parsiranje Seriala i provera tacnosti parametara

    i = 0;
    while (i < serSize) {
      if (brojacPoslova >= 5) {
        Serial.println(F("Broj poslova ne sme biti veci od 5!"));
        losRaspored = true;
        break;
      }

      if (s[i] != '[') {
        Serial.println(F("Greska kod '[' !"));
        losRaspored = true;
        break;
      }
      i++;

      int startTick = 0, durationTick = 0, deadlineTick = 0;
      
      for (int j = 0; j < 3; j++) {
        int tmpTick = 0;

        if (s[i] < '0' || s[i] > '9') {
          Serial.println(F("Greska kod vrednosti tick-a!"));
          losRaspored = true;
          break;
        }

        while (s[i] >= '0' && s[i] <= '9' && tmpTick <= 1000) {
          tmpTick = tmpTick * 10 + (s[i] - '0');
          i++;
        }
        if (s[i] != ',' || tmpTick > 1000) {
          Serial.println(F("Greska kod zapete ili vrednosti tick-ova!"));
          losRaspored = true;
          break;
        }
        i++;
        if (j == 0) startTick = tmpTick;
        else if (j == 1) durationTick = tmpTick;
        else if (j == 2) deadlineTick = tmpTick;
      }

      if (losRaspored) break;

      if (s[i]==']' || s[i+1]==']') {
        Serial.println(F("Task mora imati naziv duzine 2!"));
        losRaspored = true;
        break;
      }
      
      m[brojacPoslova].nejm[0] = s[i++];
      m[brojacPoslova].nejm[1] = s[i++];
      m[brojacPoslova].nejm[2] = '\0';
      
      if (s[i] != ']') {
        Serial.println(F("Greska kod ']' !"));
        losRaspored = true;
        break;
      }
      i++;

      if (s[i] != ';') {
        Serial.println(F("Greska kod ';' !"));
        losRaspored = true;
        break;
      }
      i++;

      jobs[brojacPoslova].a = startTick;
      jobs[brojacPoslova].c = durationTick;
      jobs[brojacPoslova].d = deadlineTick;
      switch(brojacPoslova) {
        case 0: 
          jobs[brojacPoslova].kod = task1;
          jobs[brojacPoslova].stek = task1Stack;
          jobs[brojacPoslova].hendl = &task1Handle;
          break;
        case 1:
          jobs[brojacPoslova].kod = task2;
          jobs[brojacPoslova].stek = task2Stack;
          jobs[brojacPoslova].hendl = &task2Handle;
          break;
        case 2:
          jobs[brojacPoslova].kod = task3;
          jobs[brojacPoslova].stek = task3Stack;
          jobs[brojacPoslova].hendl = &task3Handle;
          break;
        case 3:
          jobs[brojacPoslova].kod = task4;
          jobs[brojacPoslova].stek = task4Stack;
          jobs[brojacPoslova].hendl = &task4Handle;
          break;
        case 4:
          jobs[brojacPoslova].kod = task5;
          jobs[brojacPoslova].stek = task5Stack;
          jobs[brojacPoslova].hendl = &task5Handle;
          break;

        // Dovde nikad ne bi trebalo da dodje
        default: 
          Serial.println(F("Greska kod default"));
          break;
      }
      brojacPoslova++;
    }

    if (losRaspored) continue;

    float cpuUsage = 0;
    for (int i=0; i<brojacPoslova; i++) {
      cpuUsage += (1.0 * jobs[i].c/jobs[i].d);
    }
    
    if (cpuUsage > 1) {
      Serial.print(F("cpuUsage = ~"));
      Serial.print(cpuUsage);
      Serial.println(F(". Poslovi nisu rasporedivi!"));
      continue;
    }
    
    for (int i=0; i<brojacPoslova; i++) {
      xTaskCreateStatic(jobs[i].kod, m[i].nejm, STACK_SIZE, NULL, 0, jobs[i].stek, jobs[i].hendl, jobs[i].a, jobs[i].c, jobs[i].d);
    }
    return;

    }
  }
