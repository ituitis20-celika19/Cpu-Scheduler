/* @Author
Student Name: Arda Deniz Çelik
Date: 29.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    mpProcessFIFO[0] = new FIFORep(NULL); 
    mpProcessFIFO[1] = new FIFORep(NULL);
    mpProcessFIFO[2] = new FIFORep(NULL);
    mpRunningProcess = NULL;
    pCpuObj = new CPURep();
    timeSliceCount = 0;
    totalTime = 0;
}
SchedulerRep::~SchedulerRep()
{
    delete pCpuObj;
    delete mpRunningProcess;
    delete mpProcessFIFO[3];

}

FIFORep* SchedulerRep::getProcessFIFO(int index)
{
    return this->mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep* fifo, int index)
{
    this->mpProcessFIFO[index] = fifo; 
}

void SchedulerRep::setRunningProcess(ProcessRep* p)
{
    this->mpRunningProcess = p;
}

ProcessRep* SchedulerRep::getRunningProcess()
{
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep* p)
{
    if((p->getProcessType()) == "A")  //if type is A  add process to L0
    {
       setProcessFIFO(mpProcessFIFO[0],0);
       getProcessFIFO(0)->queue(p);
       
    }

    else if((p->getProcessType()) == "B") //if type is B  add process to L1
    {
        setProcessFIFO(mpProcessFIFO[1],1);
        getProcessFIFO(1)->queue(p);
    }

    else if((p->getProcessType()) == "C") //if type is C add process to L2
    {
        setProcessFIFO(mpProcessFIFO[2],2);
        getProcessFIFO(2)->queue(p);
    }
}

ProcessRep* SchedulerRep::popProcess()
{
    if(mpProcessFIFO[0]->getHead() != NULL )  // if queue is not empty dequeue A process type process
    {
        return mpProcessFIFO[0]->dequeue();
    }

    if(mpProcessFIFO[0]->getHead() == NULL && mpProcessFIFO[1]->getHead() != NULL) // if queue is not empty dequeue B process type process
    {
        return mpProcessFIFO[1]->dequeue();
    }

    if(mpProcessFIFO[0]->getHead() == NULL && mpProcessFIFO[1]->getHead() == NULL && mpProcessFIFO[2]->getHead() != NULL) // if queue is not empty dequeue C process type process
    {
        return mpProcessFIFO[2]->dequeue();
    }
    else
    {
        return NULL;
    } 
}


bool SchedulerRep::checkTimeSlice()
{  
    if(mpRunningProcess == NULL)
    {
        return false;
    } 
    else if( mpRunningProcess->getProcessType() == "C" && timeSliceCount == 8)
        return true;

    else if( mpRunningProcess->getProcessType() == "B" && timeSliceCount == 4)
        return true;
        
    else if( mpRunningProcess->getProcessType() == "A" && timeSliceCount == 2)
        return true;

    else
    {
        return false;
    }        
}
ProcessRep* SchedulerRep::sendProcessToCPU(ProcessRep* p){

    if (p == NULL) //If process is become as a null return nothing
        return NULL;

    if(p->remainingTime == p->getProcessTime()) //if it is the first time  process will go to cpu, initialize start time
        p->startTime = totalTime;
    return pCpuObj->runCPU(p,totalTime);
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
    ProcessRep* new_job = new ProcessRep(type,id,arrivalTime,processTime); // make new upcoming process as a node
    new_job->remainingTime = processTime; //initialize remaining time

   if(checkTimeSlice() == 1) 
    {
        if(mpRunningProcess->getProcessType() == new_job->getProcessType()) //If upcoming process and running are same type
        {
            pushProcess(new_job);
            if (mpRunningProcess->remainingTime == 1) // if finished send cpu and finish process
                sendProcessToCPU(mpRunningProcess);
            else if(mpRunningProcess->remainingTime > 0) // is process still have time to end?
                pushProcess(mpRunningProcess);           // it has so push into queue
            
           setRunningProcess(sendProcessToCPU(popProcess()));
            timeSliceCount = 0;
            timeSliceCount++;    

        }
        
    }

   else
    {
        if(mpRunningProcess == NULL && mpProcessFIFO[0]->getHead() == NULL && mpProcessFIFO[1]->getHead() == NULL && mpProcessFIFO[2]->getHead() == NULL ) //first process which come from txt to  cpu
        {
            setRunningProcess(sendProcessToCPU(new_job)); //send new process to cpu and set as mprunningprocess
            timeSliceCount++;
        }

        else if(new_job->getProcessType() == "A" && mpRunningProcess->getProcessType() == "B") //upcoming A type ,running B type  case
        {
           if ( mpRunningProcess->remainingTime == 1) // if finished send cpu and finish process
            {
                sendProcessToCPU(mpRunningProcess);

                setRunningProcess(new_job); //set running process as new process
                timeSliceCount = 0;
            }
            else if(mpRunningProcess->remainingTime > 0) //if running process is not finished yet
            {
                pushProcess(mpRunningProcess);
                setRunningProcess(sendProcessToCPU(new_job));
                timeSliceCount = 0;
                timeSliceCount++;

            }
            
        }

        else if(new_job->getProcessType() == "C" && mpRunningProcess->getProcessType() == "B") //upcoming C type, running B type case
        {
            if (mpRunningProcess->remainingTime == 1)
            {
                sendProcessToCPU(mpRunningProcess);
                setRunningProcess(sendProcessToCPU(new_job));
                timeSliceCount = 0;
            }

            else
            {
                setRunningProcess(sendProcessToCPU(mpRunningProcess));
                timeSliceCount++;    

            }
        }


        else if( ((new_job->getProcessType() == "B") || (new_job->getProcessType() == "A")) && mpRunningProcess->getProcessType() == "C" ) //Running C and higher priority came into CPU
        {
            if ( mpRunningProcess->remainingTime == 1)
            {
                sendProcessToCPU(mpRunningProcess);

                setRunningProcess(new_job); 
                timeSliceCount = 0;
            }
            else if(mpRunningProcess->remainingTime > 0)
            {
                pushProcess(mpRunningProcess);
                setRunningProcess(sendProcessToCPU(new_job));
                timeSliceCount = 0;
                timeSliceCount++;

            }
        }

        else if((new_job->getProcessType() == "B" || new_job->getProcessType() == "C") && mpRunningProcess->getProcessType() == "A") //Running A and upcoming process is lower priority
        {
            if (mpRunningProcess->remainingTime == 1)
            {
                sendProcessToCPU(mpRunningProcess);
                setRunningProcess(sendProcessToCPU(new_job));
                timeSliceCount = 0;
            }
            
            else
            {
                setRunningProcess(sendProcessToCPU(mpRunningProcess));  
                timeSliceCount++;

            }
        }

        else if (new_job->getProcessType() == mpRunningProcess->getProcessType()) //upcoming and running process type is same
        {
            pushProcess(new_job);
            if (mpRunningProcess->remainingTime == 1)
            {
                sendProcessToCPU(mpRunningProcess);
                setRunningProcess(popProcess());
                timeSliceCount = 0; 
            }
            else
            {
                setRunningProcess(sendProcessToCPU(mpRunningProcess));
                timeSliceCount++;

            }
        }
    }

    
    totalTime++;  
}
void SchedulerRep::schedule(ProcessRep* p)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
if(mpRunningProcess != NULL) 
{
    if(checkTimeSlice() == 1)
        {

            if(mpRunningProcess->remainingTime > 0) //is it still have time to process?
                pushProcess(mpRunningProcess);   //yes, push back to queue
            setRunningProcess(sendProcessToCPU(popProcess()));
            timeSliceCount = 0;
            timeSliceCount++;
        }

    else
        {
            if ( mpRunningProcess->remainingTime == 1)
            {
                sendProcessToCPU(mpRunningProcess);

                setRunningProcess(popProcess()); 
                timeSliceCount = 0;
            }
            else
            {
                sendProcessToCPU(mpRunningProcess);
                timeSliceCount++; 
            }
        }
    totalTime++; 
}
else    //runningprocess is null so return nothing to finish the program
    return;

        

    }


