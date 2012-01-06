#include "data.h"

void *producerFn(void *arg);
void *serverFn(void *arg);
int parseCmdLn(int ,char **);
void InitRandom(long );

void setTime(struct timeval *tmp,long arg)
{
	tmp->tv_sec=arg;
	tmp->tv_usec=arg;
}

void printTime(struct timeval printTime)
{
	//pthread_mutex_lock(printLock);
	double time=0;
	time=time+(double)printTime.tv_sec*1000.00;
	time=time+(double)printTime.tv_usec/1000.00;
	printf("%012.3fms",time);
	//pthread_mutex_unlock(printLock);
}

double conv2ms(struct timeval time)
{
	double rTime=0;
	rTime=rTime+(double)time.tv_sec*1000.00;
	rTime=rTime+(double)time.tv_usec/1000.00;
	return rTime;
}

struct timeval subTime(struct timeval time1,struct timeval time2)
{
	struct timeval returnTime;
	returnTime.tv_sec=time2.tv_sec-time1.tv_sec;
	returnTime.tv_usec=time2.tv_usec-time1.tv_usec;
	if(returnTime.tv_usec<0)
	{
		returnTime.tv_sec=returnTime.tv_sec-1;
		returnTime.tv_usec=time2.tv_usec+1000000 - time1.tv_usec;
	}
	return returnTime;
}

struct timeval addTime(struct timeval time1,struct timeval time2)
{
	struct timeval returnTime;
	returnTime.tv_sec=time2.tv_sec+time1.tv_sec;
	returnTime.tv_usec=time2.tv_usec+time1.tv_usec;
	if(returnTime.tv_usec>=1000000)
	{
		returnTime.tv_sec=returnTime.tv_sec+1;
		returnTime.tv_usec=(returnTime.tv_usec-1000000);
	}
	return returnTime;
}

void printStat(void)
{
	//printf("%012.3fms: c%d arrives, inter-arrival time = %012.3f\n",totalTime,customer->ID,interArrivalTime);
	cout<<"Statistics : "<<endl;
	cout<<"\taverage inter-arrival time =";
	printf("%.6g", (conv2ms(sumInterArrTime)/(double)1000.0)  / (double)sumArrCust );
	cout<<endl;
	cout<<"\taverage service time =";
	printf("%.6g", (conv2ms(sumServTime)/(double)1000.0) / (double)totArrCust );
	//printf("  %.6g", ((conv2ms(sumServ1Time)+conv2ms(sumServ2Time))/(double)1000.0) / (double)(sumArrCust-dropCust) );
	cout<<endl;
	
	cout<<"\taverage number of customers in Q1 =";
	printf("%.6g",numCustInQ/( conv2ms(endSimTime)/(double)1000.0) );
	cout<<endl;
	
	cout<<"\taverage number of customers in S1 =";
	printf( "%.6g",((double)conv2ms(sumServ1Time)/double(1000.0))/((double)conv2ms(endSimTime)/double(1000.0)));
	cout<<endl;
	
	cout<<"\taverage number of customers in S2 =";
	printf("%.6g",((double)conv2ms(sumServ2Time)/double(1000.0))/((double)conv2ms(endSimTime)/double(1000.0)));
	cout<<endl;
	
	cout<<endl;
	cout<<"\taverage time spent in system =";
	printf("%.6g",((double)(conv2ms(sumTimeInSys)/(double)totArrCust)/(double)1000.0));
	cout<<endl;
	
	double var=(double) ( (squareOfTimeInSys/(double)(totArrCust) ) - ( ((conv2ms(sumTimeInSys)/(double)1000.0) /(double)(totArrCust))*((conv2ms(sumTimeInSys)/(double)1000.0) /(double)(totArrCust)) ))	 ;
	//cout<<" Variance is "<<var<<endl;
	cout<<"\tstandard deviation for time spent in system =";
	printf("%.6g",sqrt(fabs(var)));
	cout<<endl;
	
	cout<<endl;
	cout<<"\tcustomer drop probability =";
	printf("%.6g",(double)dropCust/(double)sumArrCust);
	cout<<endl;
	
	cout<<endl;
}

void *signalHandler(void *arg)
{
    int recvSignal;
    for(;;)
     {
        sigwait(&signals,&recvSignal);
        if( recvSignal == SIGINT)
        {
		 //cout<<" Ctrl+c pressed by the user "<<endl;
         shutdown=1;
         //cout<<"Shutdown value is "<<shutdown<<endl;
		 pthread_exit(NULL);
        }
        if(recvSignal==SIGUSR1)
        {
        	pthread_exit(NULL);
        }
      }
}


int main(int argc,char *argv[])
{
	int rv;
	rv=parseCmdLn(argc,argv);	
	if(rv<0)
	{
		exit(0);
	}
	sigHandlerThread=new pthread_t();
	threadRefLok=new pthread_mutex_t();
	pthread_mutex_init(threadRefLok,NULL);
	sigemptyset (&signals);
    sigaddset (&signals, SIGINT);
    sigaddset (&signals, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &signals, NULL);
    pthread_create(sigHandlerThread,NULL,signalHandler,NULL);
    pthread_mutex_lock(threadRefLok);
    threadRefs.push_back(*sigHandlerThread);
    pthread_mutex_unlock(threadRefLok);
    sumServTimeLok=new pthread_mutex_t();
    pthread_mutex_init(sumServTimeLok,NULL);
    sumTimeInSysLok=new pthread_mutex_t();
    pthread_mutex_init(sumTimeInSysLok,NULL);
    endSimTimeLok=new pthread_mutex_t();
    pthread_mutex_init(endSimTimeLok,NULL);
	squareOfTimeInSysLok=new pthread_mutex_t();
	pthread_mutex_init(squareOfTimeInSysLok,NULL);
	numCustInQLok=new pthread_mutex_t();
	pthread_mutex_init(numCustInQLok,NULL);
	totArrCustLok=new pthread_mutex_t();
	pthread_mutex_init(totArrCustLok,NULL);
	
	fileLok=new pthread_mutex_t();
	pthread_mutex_init(fileLok,NULL);
	//pthread_exit(NULL);
	InitRandom(seedVal);
	/* CREATING AND INTIALIZING THE QUEUE LOCK */
	qLock=new pthread_mutex_t();
	pthread_mutex_init(qLock,NULL);

	/* CREATING AND INTIALIZING THE WAITING SERVER LOCK */
	servWaitingLock=new pthread_mutex_t();
	pthread_mutex_init(servWaitingLock,NULL);

	/* CREATING AND INTIALIZING THE COUT LOCK */
	printLock=new pthread_mutex_t();
	pthread_mutex_init(printLock,NULL);

	/* CREATING AND INTIALIZING THE CV WHERE THE SERVERS WILL WAIT IF THERE ARE NO CUSTOMERS IN THE WAIT QUEUE */
	serverWaitingCond=new pthread_cond_t();
	pthread_cond_init(serverWaitingCond,NULL);

	/* CREATIING THE PRODUCER THREAD */
	prodThread=new pthread_t();
	pthread_create(prodThread,NULL,producerFn,NULL);
	pthread_mutex_lock(threadRefLok);
    threadRefs.push_back(*prodThread);
    pthread_mutex_unlock(threadRefLok);

	/* CREATIING THE 1ST SERVER THREAD */
	servThread1=new pthread_t();
	pthread_create(servThread1,NULL,serverFn,(void *)&serv1);
	pthread_mutex_lock(threadRefLok);
    threadRefs.push_back(*servThread1);
    pthread_mutex_unlock(threadRefLok);

	/* CREATIING THE 2ND SERVER THREAD */
	if(!sSet)
	{
		servThread2=new pthread_t();
		pthread_create(servThread2,NULL,serverFn,(void *)&serv2);
		pthread_mutex_lock(threadRefLok);
    	threadRefs.push_back(*servThread2);
    	pthread_mutex_unlock(threadRefLok);
    }
	
	//cout<<"Main thread exiting "<<endl;
	for(int i = 0; i < (int)threadRefs.size() ; i++)
	{
		//cout<<"Main server thread is waiting for its child thread to complete"<<endl;
		
		pthread_join(threadRefs[i], NULL);
		
	}
	printStat();
	pthread_exit(NULL);
}
 
