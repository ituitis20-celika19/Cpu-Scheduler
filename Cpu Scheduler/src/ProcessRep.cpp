/* @Author
Student Name: Arda Deniz Çelik
Date: 29.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string>
#include "ProcessRep.h"


using namespace std;

ProcessRep::ProcessRep()  //Default constructor
{
    mpNext = NULL;
    mProcessType = "";
    mProcessID = 0;
    mDeliverTime = 0;
    mProcessTime = 0;
    remainingTime = 0;
    startTime = 0;
    endTime = 0;


}

ProcessRep::ProcessRep(string processType, int processID, int deliverTime, int processTime)
{   
    this->mProcessType = processType;
    this->mProcessID = processID;
    this->mDeliverTime = deliverTime;
    this->mProcessTime = processTime;
    remainingTime = 0;
    startTime = -1;
    endTime = -1;
}

ProcessRep::~ProcessRep()
{
    delete mpNext;
}

ProcessRep* ProcessRep::getNext()
{
    return this->mpNext;
}

void ProcessRep::setNext(ProcessRep *next)
{
    this->mpNext = next;
}

string ProcessRep::getProcessType()
{
    return this->mProcessType;
}

void ProcessRep::setProcessType(string processType)
{
    this->mProcessType = processType;
}

int ProcessRep::getDeliverTime()
{
    return this->mDeliverTime;
}

void ProcessRep::setDeliverTime(int deliverTime)
{
    this->mDeliverTime = deliverTime;
}

int ProcessRep::getProcessID()
{
    return this->mProcessID;
}

void ProcessRep::setProcessID(int processID)
{
    this->mProcessID = processID;
}

int ProcessRep::getProcessTime()
{
    return this->mProcessTime;
}

void ProcessRep::setProcessTime(int ProcessTime)
{
    this->mProcessTime = ProcessTime;
    
}