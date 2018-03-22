#ifndef Timer_H_
#define Timer_H_
#include <Windows.h>
//initialize timer structure
#define TIMER_INIT \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER t1,t2; \
    double elapsedTime; \
    QueryPerformanceFrequency(&frequency);


// Use to start the performance timer
#define TIMER_START QueryPerformanceCounter(&t1);

// Use to stop the performance timer and output the result
#define TIMER_STOP \
    QueryPerformanceCounter(&t2); \
    elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;

#endif
