#include "extern.h"

/*
FUNCTION TO PRINT THE COMMAND LINE ARGUMENTS
USED OLY FOR DEBUGGING
*/

void printCmdArgs()
{
	cout<<"Parameters:"<<endl;
	if(!tsFileSet)
	{
		printf("\tlambda = %f \n",lambda);
	
		printf("\tmu = %f \n",mu);
	}
	if(sSet)
	{
		cout<<"\tsystem = M/M/1"<<endl;
	}
	else
	{
		cout<<"\tsystem = M/M/2"<<endl;
	}
	//cout<<" -s option set : "<<sSet<<endl;
	if(!tsFileSet)
	{
		cout<<"\tseed = "<<seedVal<<endl;
	}
	cout<<"\tsize = "<<MaxqSize<<endl;
	cout<<"\tnumber = "<<numCust<<endl;
	if(!tsFileSet)
	{
		cout<<"\tdistribution = ";
		if(dOption==EXP)
		{
			cout<<"exp"<<endl;
		}
		else
		{
			cout<<"det"<<endl;
		}
	}
	if(tsFileSet==1)
	{
		//cout<<" TS file option is set "<<endl;
		cout<<"\ttsfile = "<<tsFileName<<endl;
	}

}

/*
THIS FUNCTION PRINT THE USAGE FORMAT OF THE COMMAND
*/
void printUsage()
{
	cout<<"Malformed Command"<<endl;
	cout<<"Usage: ";
	cout<<" ./mm2 [-lambda lambda] [-mu mu] [-s] [-seed seedval] [-size sz] [-n num] [-d {exp|det}] [-t tsfile]"<<endl;
}

/*

*/

int setNumCust()
{
	tsFilePtr=fopen(tsFileName,"r");
	if(tsFilePtr==NULL)
	{
		cout<<" Error : Cannot open ts file "<<endl;
		return -1;
	}
	else
	{
		fscanf(tsFilePtr,"%u",&numCust);
	}
	fclose(tsFilePtr);
	return 1;
}

/*
FUNCTION THAT PARSES THE TS FILE TO GET THE INPUTS
*/
int parseTsFile()
{
	//printCmdArgs();
	tsFilePtr=fopen(tsFileName,"r");
	if(tsFilePtr==NULL)
	{
		cout<<" Error : Cannot open ts file "<<endl;
		return -1;
	}
	else
	{
		//  THE FILE IS OPENED SUCCESSFULLY
		fscanf(tsFilePtr,"%u",&numCust);
		arrvTimes=new int[numCust];
		servTimes=new int[numCust];
		int k=0;
		while((fscanf(tsFilePtr,"%d",&arrvTimes[k])!=EOF)&&(fscanf(tsFilePtr,"%d",&servTimes[k])!=EOF))
		{
			cout<<" k is "<<k<<endl;
			k++;
		}
	}
	printCmdArgs();
	return 0;
	
}

/* 
THIS FUNCTION PARSES THE COMMAND LINE ARGUEMENTS 
IF ANY ERROR OCCURS IT RETURNS -1.
IF EVERY THING GOES FINE IT RETURNS 0
*/
int parseCmdLn(int argCount,char **args)
{
	//cout<<"The count of the command line argument are "<<argCount<<endl;
	//int ok=0;
	if(argCount>16)
	{
		cout<<"Error arg count"<<endl;
		printUsage();
		return -1;
	}
	else
	{
		for(int i=1;i<argCount;i++)
		{
			//cout<<" i is"<<i<<endl;

			if(!(strncmp(args[i],"-lambda",7)))
			{
				/* LAMBDA GIVEN */
				//cout<<"Lamda value given"<<endl;
				char *end;
				lambda=strtod(args[(i+1)], &end);
				i++;
				if(strcmp(end,""))
				{
					cout<<"Error lambda "<<endl;
					printUsage();
					return -1;
				}
				
			}
			else if(!(strncmp(args[i],"-mu",3)))
			{
				/* MU GIVEN */
				//cout<<"Mu value given"<<endl;
				char *end;
				mu=strtod(args[(i+1)], &end);
				i++;
				if(strcmp(end,""))
				{
					printUsage();
					return -1;
				}
			}
			else if(!(strncmp(args[i],"-seed",5)))
			{
				//cout<<" Seed value given "<<endl;
				/* SEED VALUE GIVEN */
				char *end;
				seedVal= strtol (args[(i+1)],&end,10);
				i++;
				if(strcmp(end,""))
				{
					cout<<"Error seed"<<endl;
					printUsage();
					return -1;
				}

			}


			else if(!(strncmp(args[i],"-size",5)))
			{
				/* SIZE OF THE QUEUE IS GIVEN */
				//cout<<"Queue size given "<<endl;
				char *end;
				MaxqSize= (unsigned int)strtol (args[(i+1)],&end,10);
				i++;
				if(strcmp(end,""))
				{
					cout<<"Erroe Q size "<<endl;
					printUsage();
					return -1;
				}

			}
			else if(!(strncmp(args[i],"-s",2)))
			{
				/* SERVER2 IS OFF */
				//cout<<"Error s "<<endl;
				//cout<<" Server 2 is switched off"<<endl;
				sSet=1;
			}
			else if(!(strncmp(args[i],"-n",2)))
			{
				/* TOTAL NUMBER OF CUSTOMERS GIVEN */
				//cout<<" Total number of customers given "<<endl;
				char *end;
				numCust= (unsigned int)strtol (args[(i+1)],&end,10);
				i++;
				if(strcmp(end,""))
				{
					cout<<"Error n"<<endl;
					printUsage();
					return -1;
				}

			}
			else if(!(strncmp(args[i],"-d",2)))
			{
				/* TYPE OF DISTRIBUTION IS GIVEN */
				if(!(strncmp(args[(i+1)],"exp",3)))
				{
					i++;
					//cout<<"Exponetial Dist is chosen"<<endl;
					dOption=EXP;
				}
				else if(!(strncmp(args[(i+1)],"det",3)))
				{
					i++;
					//cout<<"Derterministic Dist is chosen"<<endl;
					dOption=DET;
				}
				else
				{
					printUsage();
					return -1;
				}
			}
			else if(!(strncmp(args[i],"-t",2)))
			{	
				/* ALL THE INPUT ARE FROM THE T-FILE */
				tsFileSet=1;
				//cout<<"All information have to read from file"<<endl;
				cout<<args[(i+1)]<<endl;
				int len=strlen(args[(i+1)]);
				i++;
				tsFileName=new char[len+1];
				memset(tsFileName,'\0',len+1);
				strncpy(tsFileName,args[i],len);
				int rv=setNumCust();
				if (rv<0)
				{
					return -1;
				}
			}
			else
			{
				// MALFORMED COMMAND
				printUsage();
				return -1;
			}
				
		}
	}
	printCmdArgs();
	return 0;
}
