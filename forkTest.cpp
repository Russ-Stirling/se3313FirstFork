#include <iostream>
#include <sstream>
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "sched.h"  
#include <string>
#include <fstream>


void writeFile(std::string userData, int count, pid_t parentID);
void parallelProcess(int count);
int checkFilesExist(int count);
std::string getUserData();

int main ()
{
	parallelProcess(0);
	return 0;
}

std::string getUserData()
{
	std::string userData;
	std::cout<<"Enter 'done' to quit or enter text to be written to a new file: ";
	std::getline(std::cin, userData);
	std::cout<<std::endl<<std::endl;
	return userData;
}

void parallelProcess(int count)
{
	count++;
	count=checkFilesExist(count);
	std::string userData = getUserData();
	if(userData!="done")
	{
		pid_t pid = fork();
		if (pid == 0) //child
		{
			writeFile(userData, count, getppid());
		}
	
		else if (pid >0) //parent
		{
			parallelProcess(count);
		}

		else //failed fork
		{
			std::cout<<"Fork failed";
		}
	}
}

int checkFilesExist(int count)
{
	std::stringstream index;
	index<<"fileOutput"<<(count)<<".txt";
	std::string tempstr=index.str();
	const char *j=tempstr.c_str();
	
	std::ifstream fileTest(j);
	if(fileTest)
	{
		count++;
		fileTest.close();
		count=checkFilesExist(count);
	}
	return count;
}

void writeFile(std::string userData, int count, pid_t parentID) 
{
	std::stringstream index;
	index<<"fileOutput"<<(count)<<".txt";
	std::string tempstr=index.str();
	const char *j=tempstr.c_str();	
	
	std::ofstream theFile;
	theFile.open(j);
	while(getppid()==parentID)
	{
		theFile<<userData;
		theFile<<"\n";
		sleep(1); //wait for 1 second
	}
	theFile.close();
	//check to ensure all processes terminate, commented out for final code
	//std::cout<<std::endl<<"parent dead terminating child: "<<count; 
}


