#include "extern.h"

int GetInterval(int , double );

void printTime(struct timeval );
struct timeval subTime(struct timeval ,struct timeval );
struct timeval addTime(struct timeval ,struct timeval );
void setTime(struct timeval *,long );
double conv2ms(struct timeval );
void *signalHandler(void *arg);

/* 
PRODUCER FUNCTION WHICH PRODUCES THE CUSTOMERS AND INSERTS THEM 
INTO THE QUEUE AT CALCULATED PROBABILITY SIDTRIBUTION
*/

void *producerFn(void *arg)
{
	customers *cust;
	int qSize;
	gettimeofday(&startTime,NULL); 			// SIMULATION STARTING TIME IS GOT HERE
	currTime=subTime(startTime,startTime);	// CURRENT TIME IS ZERO AS THE EMULATION BEGINS
	printTime(currTime);
	
	pthread_mutex_lock(printLock);
	cout<<": Emulation begins"<<endl;
	pthread_mutex_unlock(printLock);
	
	struct timeval timeNow=currTime;		// INTIALIZING ALL THE TIMES TO ZERO AT THE START 
	struct timeval gapTime=currTime;        // INTIALIZING ALL THE TIMES TO ZERO AT THE START 
	struct timeval bsleep;
	struct timeval asleep;
	struct timeval processTime=currTime;
	struct timeval lastArrTime=currTime;
	startSimTime=currTime;
	int sleepTime;
	for(int i=0;i<numCust;i++)
	{
		pthread_mutex_lock(qLock);
		if(shutdown==1)
		{
			while(cQueue.size()>0)
			{
				cQueue.pop();
			}
			pthread_cond_broadcast(serverWaitingCond);
			pthread_mutex_unlock(qLock);
			//cout<<"Producer exiting ctrl+c press"<<endl;
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(qLock);
		if(tsFileSet)
		{
			int temp;
			pthread_mutex_lock(fileLok);
			tsFilePtr=fopen(tsFileName,"r");
			for(int k=0;k<(i+2);k++)
			{
				if(k==0)
				{
					fscanf(tsFilePtr,"%d",&temp);
				}
				else
				{
					fscanf(tsFilePtr,"%d %d",&sleepTime,&temp);
					//cout<<"\n sleep time is "<<sleepTime<<endl;
				}
			}
			fclose(tsFilePtr);
			pthread_mutex_unlock(fileLok);
		}
		else
		{
			sleepTime=GetInterval(dOption,lambda);  // CALCULATING THE TIME TO SLEEP
			//cout<<"sleep time is "<<sleepTime<<endl;
		}
		cust=new customers();						// CUSTOMER IS CREATED
		cust->id=i+1;
		//cout<<"\n sleep time is "<<sleepTime<<" cust id is "<<cust->id<<endl;
		gettimeofday(&bsleep,NULL);
		usleep(sleepTime*1000-conv2ms(processTime));	// SIMULATING THE INTER ARRIVAL TIME						
		gettimeofday(&asleep,NULL);				// CALCULATING THE INTER ARRIVAL TIME
		sumArrCust++;
		pthread_mutex_lock(qLock);// LOCKING THE QUEUE
		qSize=cQueue.size();
		pthread_mutex_unlock(qLock);
		gapTime=addTime(subTime(bsleep,asleep),processTime);
		////////////
		pthread_mutex_lock(numCustInQLok);
		numCustInQ=numCustInQ+((conv2ms(gapTime)/(double)1000.0)*(double)qSize);
		pthread_mutex_unlock(numCustInQLok);
		//////////
		cust->interArrTime=gapTime;
		sumInterArrTime=addTime(sumInterArrTime,cust->interArrTime);
		gettimeofday(&timeNow,NULL);
		currTime=addTime(currTime,gapTime);
		
		/* EMULATION OUTPUT IS PRINTED - BEGIN */
		//printf("%012.3fms: c%d arrives, inter-arrival time = %012.3f\n",totalTime,customer->ID,interArrivalTime);

		cust->arrvTime=currTime;
		
		pthread_mutex_lock(printLock);
		printTime(cust->arrvTime);
		cout<<" : c"<<cust->id<<" arrives, inter-arrival time = ";
		printTime(cust->interArrTime);
		cout<<endl;
		pthread_mutex_unlock(printLock);
		
		/* EMULATION OUTPUT IS PRINTED - END */
		
		//gettimeofday(&timeNow,NULL);
		//  IF I = 0 THEN ITS THE 1ST CUSTOMER 
		//  SO DECREMENTING THE TME FROM THE START TIME
		pthread_mutex_lock(qLock);// LOCKING THE QUEUE
		qSize=cQueue.size();
		gettimeofday(&timeNow,NULL);
		cust->qEnterTimeNw=timeNow;
		lastArrTime=addTime(currTime,subTime(asleep,timeNow));
		cust->qEnterTime=lastArrTime;
		if(qSize<MaxqSize)
		{	
		
				pthread_mutex_lock(printLock);
				printTime(cust->qEnterTime);
				cout<<" : c"<<cust->id<<" enters Q1"<<endl;
				pthread_mutex_unlock(printLock);

				cQueue.push(cust);					// PUSHING THE CUSTOMERS
				pthread_mutex_lock(servWaitingLock);
				if(servWaiting>0)
				{
					servWaiting=0;
				}	
				pthread_mutex_unlock(servWaitingLock);
				pthread_cond_broadcast(serverWaitingCond);
			
			pthread_mutex_unlock(qLock);
			gettimeofday(&timeNow,NULL);
			processTime=addTime(processTime,subTime(asleep,timeNow));

		}
		else
		{
			// QUEUE IS FULL SO SHOULD DROP CUSTOMERS
			pthread_mutex_unlock(qLock);
			dropCust++; // INCREMENTING THE DROPPED CUSTOMERS
			
			pthread_mutex_lock(printLock);
			printTime(cust->qEnterTime);
			cout<<" : c"<<cust->id<<" dropped"<<endl;
			pthread_mutex_unlock(printLock);
			
			/* PROCESS TIME IS CALCULATED IF THE CUSTOMER IS DROPPED ALSO */
			gettimeofday(&timeNow,NULL);
			processTime=addTime(processTime,subTime(asleep,timeNow));
		}

	}
	end=1;
	pthread_mutex_lock(qLock);
	pthread_cond_broadcast(serverWaitingCond);
	pthread_mutex_unlock(qLock);
	
	pthread_exit(NULL);
}
