#include "common.h"

/*
THIS FILE CONTAINS THE GLOBAL VARIABLE THAT ARE USED IN THE SIMULATION
*/
/* 
GLOBAL SHARED VARIABLES 
THREAD SAFE READ ONLY 
*/ 
double lambda=0.5;			// ARRIVAL RATE FOR CUSTOMERS
double mu=0.35;				// SERVICE RATE FOR SERVERS
unsigned long int seedVal=0;// RECORDS THE SEED VALUE THAT IS GIVEN AS INPUT TO THE RANDOM NUMBER GEN FN
unsigned int MaxqSize=5;	// MAX SIZE OF THE QUEUE FOR WAITING CUSTOMERS
unsigned int numCust=20; 	// RECORDS THE TOTAL NUMBER OF CUSTOMERS
unsigned int dOption=EXP;	// VARIABLE INDICATING THE TYPE THE DISTRIBUTION
int numServ=MAX_SERVERS;	// VARIABLE INDICATIING THE NUMBER OF SERVERS
int sSet=0;
char *tsFileName;
int tsFileSet=0;

FILE *tsFilePtr;
pthread_mutex_t *fileLok;
int dropCust=0;		// VARIABLE TO KEEP TRACK OF THE DROPPED CUSTOMERS
int sumArrCust=0;

/*
THE BELOW TWO POINTERS ARE USED ONLY IF THE DATA IS READ FROM THE TS FILE
THREAD SAFE READ ONLY 
*/
int *arrvTimes;		// ARRAY HOLDING THE ARRVIAL TIMES OF ALL THE CUSTOMERS READ FROM THE TS FILE
int *servTimes;		// ARRAY HOLDING THE SERVICE TIMES OF ALL THE CUSTOMERS READ FROM THE TS FILE

struct timeval startTime;
struct timeval EndTime;
struct timeval currTime;	

struct timeval sumInterArrTime;
double squareOfTimeInSys=0.0;
pthread_mutex_t *squareOfTimeInSysLok;

struct timeval sumServ1Time;
struct timeval sumServ2Time;
struct timeval sumServTime;
pthread_mutex_t *sumServTimeLok;

struct timeval sumTimeInSys;
pthread_mutex_t *sumTimeInSysLok;

sigset_t signals;
pthread_mutex_t *threadRefLok;
vector<pthread_t> threadRefs;
pthread_t *sigHandlerThread;
int shutdown=0;
int end=0;

int totArrCust=0;
pthread_mutex_t *totArrCustLok;

pthread_mutex_t *numCustInQLok;

/* 
IDS OF THER SERVERS 
THREAD SAFE 
*/
int serv1=1;
int serv2=2;

double numCustInQ=0.0;
int sumOfCustInServ[MAX_SERVERS];

struct timeval startSimTime;
struct timeval endSimTime;
pthread_mutex_t *endSimTimeLok;
/* THE SERVER ID OF S1 IS 0 AND S2 IS 1 */
int servWaiting=0;	// VARIABLE INDICATING THE NUMBER OF WAITING SEVERS NOT THREAD SAFE
pthread_mutex_t *servWaitingLock; // LOCK TO THE ABOVE SHARED VARIABLE
pthread_cond_t *serverWaitingCond; // CONDITION VARIABLE ON WHICH THE SERVERS WAIT


queue<customers *> cQueue; // QUEUE TO HOLD THE CUSTOMERS NOT THREAD SAFE
pthread_mutex_t *qLock;    // A lOCK TO CONTROL ACCESS TO IT

pthread_mutex_t *printLock;


pthread_t *prodThread;	// THREAD FOR THE PRODUCER WHICH PRODUCES THE CUSTOMERS 
pthread_t *servThread1; // THREAD FOR THE 1ST SERVER
pthread_t *servThread2; // THREAD FOR THE 2ND SERVER
