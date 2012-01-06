#include "extern.h"

#define round(X) (((X)>= 0)?(int)((X)+0.5):(int)((X)-0.5))

double ExponentialInterval(double dval,double rate) 
{
    double temp = (-(1/rate)*log(dval));
    return (temp*1000);
}

void InitRandom(long l_seed)
{
	if (l_seed == 0L) 
	{
		time_t localtime=(time_t)0;
		time(&localtime);
		srand48((long)localtime);
	} 
	else 
	{
            srand48(l_seed);
    }
}
    
int GetInterval(int exponential, double rate)
{
	int intervalTime;
	if (exponential) 
	{
		//cout<<"Exp "<<endl;
		double dval=(double)drand48();
        intervalTime= round(ExponentialInterval(dval, rate));
    }
	else
	{
        double millisecond=((double)1000)/rate;
        intervalTime= round(millisecond);
    }
    if (intervalTime <= 1)
    {
    	return (1);
    }
    else if (intervalTime > 10000)
    {
    	return (10000);
    }
    else
    {
        return intervalTime;
    }
}


