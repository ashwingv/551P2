#include "extern.h"
int GetInterval(int , double );

void printTime(struct timeval );
struct timeval subTime(struct timeval ,struct timeval );
struct timeval addTime(struct timeval ,struct timeval );
void setTime(struct timeval *,long );
double conv2ms(struct timeval );
void *signalHandler(void *arg);


void *serverFn(void *arg)
{
	int *ref;
	ref=(int *)(arg);
	int id=*ref;
	sumOfCustInServ[(id-1)]=0;
	int qSize;
	customers *cust;
	struct timeval servTimeNow;
	struct timeval lastTime;
	struct timeval procTime;
	struct timeval asleep;
	struct timeval bsleep;
	setTime(&procTime,0);
	struct timeval gapTime;
	setTime(&gapTime,0);
	setTime(&sumServTime,0);
	setTime(&sumTimeInSys,0);
	setTime(&sumServ1Time,0);
	setTime(&sumServ2Time,0);
	while(1)
	{
		if(shutdown==1)
		{
			pthread_exit(NULL);
		}
		pthread_mutex_lock(qLock);	
		qSize=cQueue.size();
		while(qSize==0 && end!=1 && shutdown!=1)
		{
			// NO WAITING CUSTOMERS SO I MUST WAIT
			pthread_mutex_lock(servWaitingLock);
			servWaiting++;	// INCREMENTING THE COUNT OF WAITING SERVERS
			pthread_mutex_unlock(servWaitingLock);
			pthread_cond_wait(serverWaitingCond,qLock);
			qSize=cQueue.size();
		}
		if(qSize==0)
		{
			pthread_kill(*sigHandlerThread,SIGUSR1);
			pthread_mutex_unlock(qLock);
			pthread_exit(NULL);
		}
		cust=cQueue.front();	// GETTING THE REFERENCE OF THE 1ST CUSTOMER IN THE QUEUE
		pthread_mutex_lock(totArrCustLok);
		totArrCust++;
		pthread_mutex_unlock(totArrCustLok);
		cQueue.pop();			// REMOVING THAT CUSTOMER FROM THE QUEUE
		gettimeofday(&servTimeNow,NULL);
		//cust->qExitTime=addTisubTime(cust->qEnterTimeNw,servTimeNow);
		struct timeval timeInQ=subTime(cust->qEnterTimeNw,servTimeNow);
		cust->qExitTime=addTime(cust->qEnterTime,timeInQ);
		pthread_mutex_unlock(qLock);
		
		pthread_mutex_lock(printLock);
		printTime(cust->qExitTime);
		cout<<" : c"<<cust->id<<" leaves Q1, time in Q1 = ";
		printTime(timeInQ);
		cout<<endl;
		pthread_mutex_unlock(printLock);
		
		struct timeval bgTime;
		gettimeofday(&bgTime,NULL);
		cust->servBegTime=addTime(cust->qExitTime,subTime(servTimeNow,bgTime));

		
		pthread_mutex_lock(printLock);
		printTime(cust->servBegTime);
		cout<<" : c"<<cust->id<<" begins service"<<endl;
		pthread_mutex_unlock(printLock);
		
		int sleepTime;
		if(tsFileSet)
		{
			int temp;
			pthread_mutex_lock(fileLok);
			tsFilePtr=fopen(tsFileName,"r");
			for(int j=0;j<(cust->id+1);j++)
			{
				if(j==0)
				{
					fscanf(tsFilePtr,"%d",&temp);
				}
				else
				{
					fscanf(tsFilePtr,"%d %d",&temp,&sleepTime);
				}
			}
			fclose(tsFilePtr);
			pthread_mutex_unlock(fileLok);
		}
		else
		{
			sleepTime=GetInterval(dOption,mu);  // CALCULATING THE TIME TO SLEEP
			//cout<<"sleep time is "<<sleepTime<<endl;
		}
		//cout<<"\n Server sleep time is "<<sleepTime<<"cust id is "<<cust->id<<endl;
		gettimeofday(&bsleep,NULL);
		usleep(sleepTime*1000-conv2ms(procTime));							
		gettimeofday(&asleep,NULL);
		gapTime=addTime(subTime(bsleep,asleep),procTime);
		
		/*pthread_mutex_lock(qLock);
		qSize=cQueue.size();
		pthread_mutex_unlock(qLock);
		
		pthread_mutex_lock(numCustInQLok);
		numCustInQ=numCustInQ+((conv2ms(gapTime)/(double)1000.0)*(double)qSize);
		pthread_mutex_unlock(numCustInQLok);
		*/
		
		pthread_mutex_lock(sumServTimeLok);
		sumServTime=addTime(sumServTime,gapTime);
		pthread_mutex_unlock(sumServTimeLok);
		if(id==1)
		{
			sumServ1Time=addTime(sumServ1Time,gapTime);
		}
		else if(id==2)
		{
			sumServ2Time=addTime(sumServ2Time,gapTime);
		}
		else{}
		cust->sysExitTime=addTime(cust->servBegTime,gapTime);
		pthread_mutex_lock(endSimTimeLok);
		endSimTime=cust->sysExitTime;
		pthread_mutex_unlock(endSimTimeLok);
		cust->timeInSys=subTime(cust->arrvTime,cust->sysExitTime);
		pthread_mutex_lock(squareOfTimeInSysLok);
		squareOfTimeInSys=squareOfTimeInSys+((conv2ms(cust->timeInSys)/(double)1000.0)*(conv2ms(cust->timeInSys)/(double)1000.0));
		pthread_mutex_unlock(squareOfTimeInSysLok);
		pthread_mutex_lock(sumTimeInSysLok);
		sumTimeInSys=addTime(sumTimeInSys,cust->timeInSys); // 	CALCULATING THE SUM OF TOTAL TIME IN SYSTEM
		pthread_mutex_unlock(sumTimeInSysLok);
		
		pthread_mutex_lock(printLock);
		printTime(cust->sysExitTime);
		cout<<" : c"<<cust->id<<" departs, service time =";
		printTime(gapTime);
		cout<<endl;
		cout<<"\t\t time in system = ";
		printTime(cust->timeInSys);
		cout<<endl;
		pthread_mutex_unlock(printLock);
		
		gettimeofday(&lastTime,NULL);
		procTime=addTime(subTime(asleep,lastTime),subTime(servTimeNow,bsleep));
		sumOfCustInServ[(id-1)]++;
	}
	pthread_exit(NULL);
}
