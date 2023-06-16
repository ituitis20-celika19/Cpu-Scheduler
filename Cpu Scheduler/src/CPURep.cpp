/* @Author
Student Name: Arda Deniz Çelik
Date: 29.10.2022
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
    mFinishedProcess = new FIFORep(NULL);
}

CPURep::~CPURep()
{
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   
    if(p->remainingTime == 1)   //if process finished add to finished process fiforep
    {
        p->endTime = time+1;
        mFinishedProcess->queue(p);
        return NULL;
    }
    else{
        p->remainingTime = (p->remainingTime - 1); //if it is not finished decrease remaining time
        return p;
    }

    

}

FIFORep* CPURep::getFinishedProcess()
{
    return this->mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    this->mFinishedProcess = finishedProcess; 
}