#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>

#define EXP 1
#define DET 0
#define MAX_SERVERS 2

using namespace std;


struct customers
{
	struct timeval arrvTime;
	struct timeval interArrTime;
	struct timeval qEnterTime;
	struct timeval qEnterTimeNw;
	struct timeval qExitTime;
	struct timeval servBegTime;
	struct timeval sysExitTime;
	struct timeval timeInSys;
	int id;
};


