#include "common.h"

extern double lambda;
extern double mu;
extern int seedVal;
extern int MaxqSize;
extern int numCust; 
extern int dOption;
extern int sSet;
extern int serv1;
extern int serv2;
extern char *tsFileName;
extern int tsFileSet;


extern FILE *tsFilePtr;
extern pthread_mutex_t *fileLok;
extern int dropCust;
extern int sumArrCust;
extern int  *arrvTimes;		// ARRAY HOLDING THE ARRVIAL TIMES OF ALL THE CUSTOMERS 
extern int  *servTimes;		// ARRAY HOLDING THE SERVICE TIMES OF ALL THE CUSTOMERS 

extern struct timeval startTime;
extern struct timeval EndTime;
extern struct timeval currTime;
extern struct timeval sumInterArrTime;

extern struct timeval sumServTime;
extern struct timeval sumServ1Time;
extern struct timeval sumServ2Time;
extern pthread_mutex_t *sumServTimeLok;

extern struct timeval sumTimeInSys;
extern pthread_mutex_t *sumTimeInSysLok;

extern sigset_t signals;
extern pthread_mutex_t *threadRefLok;
extern vector<pthread_t> threadRefs;
extern pthread_t *sigHandlerThread;
extern int shutdown;
extern int end;

extern int totArrCust;
extern pthread_mutex_t *totArrCustLok;

extern pthread_mutex_t *numCustInQLok;

extern int sumOfCustInServ[MAX_SERVERS];

extern double squareOfTimeInSys;
extern pthread_mutex_t *squareOfTimeInSysLok;

extern double numCustInQ;
extern struct timeval startSimTime;
extern struct timeval endSimTime;
extern pthread_mutex_t *endSimTimeLok;

extern queue<customers *> cQueue; 
extern pthread_mutex_t *qLock; // A lOCK TO CONTROL ACCESS TO IT

extern int servWaiting;	// VARIABLE INDICATING THE DEFAULT SERVER ID
extern pthread_mutex_t *servWaitingLock;
extern pthread_cond_t *serverWaitingCond;
extern pthread_mutex_t *printLock;

extern pthread_t *prodThread;	// THREAD FOR THE PRODUCER WHICH PRODUCES THE CUSTOMERS 
extern pthread_t *servThread1; // THREAD FOR THE 1ST SERVER
extern pthread_t *servThread2; // THREAD FOR THE 2ND SERVER
