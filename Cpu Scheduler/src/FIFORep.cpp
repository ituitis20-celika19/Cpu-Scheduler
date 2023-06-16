/* @Author
Student Name: Arda Deniz Çelik
Date: 29.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    
    mpHead = NULL;
    mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep* head)
{
    head = this->mpHead;
    head = this->mpTail;      //assigning pointer
}


FIFORep::~FIFORep()
{
    ProcessRep* p1 = NULL; //pointer 1
    ProcessRep* n1 = NULL; //next 1
    FIFORep* p2 = NULL;
    p2->setHead(p1);

    while(n1 != NULL)
    { 
        n1=p1->getNext();
        delete p1;
        p1=n1;

    }
    delete p2;
    
}

void FIFORep::setHead(ProcessRep* head)
{
    this->mpHead = head;
}

ProcessRep* FIFORep::getHead()
{
    return this->mpHead;
}

void FIFORep::setTail(ProcessRep* tail)
{
    this->mpTail = tail;
}

ProcessRep* FIFORep::getTail()
{
    return this->mpTail;
}

void FIFORep::queue(ProcessRep* p)
{
    /*
        The function add a process to the tail of the queue.
    */
    if(mpHead == NULL)
    {
        mpHead = p;
        mpTail = p;

    }
    else{
        mpTail->setNext(p);  //add to end
        mpTail = mpTail->getNext();

    }
}

ProcessRep* FIFORep::dequeue()
{   
    /*
        The function remove a process from the tail of the queue and returns it.
    */   
    if(mpHead == NULL)
        return NULL;
    else{
        ProcessRep* temp = mpHead;
        mpHead = mpHead->getNext();   // remove from beginning
        temp->setNext(NULL);
        return temp;
        
    }    
}

ProcessRep* FIFORep::searchID(int id)
{   
    /*
        The function search a process by id and returns it if it exist otherwise returns NULL.
    */ 
    ProcessRep * locater = mpHead;
    if ( mpHead == NULL)
        return NULL;

    else
    {
        while (locater->getNext() != NULL)
        {
            if(locater->getProcessID() == id)   //look for id return that id
                return locater;

            else
            {
                return NULL;
            }

            locater = locater->getNext();   
        }
        
    }
    return NULL;
    
}

void FIFORep::printFIFO()
{
    /*
        The function prints the proceses in the queue starting from Head to tail.
    */
    ProcessRep* temp = mpHead;
    if(temp == NULL)
    {
        cout<<flush;
        return;
    }
    else
    {
        while (temp != NULL)
        {
            cout<<temp->getProcessType()<<temp->getProcessID()<<" "<<temp->startTime<<" "<<temp->endTime<<" "<<flush;  // print type,id,process cpu start time, end time of proces
            temp = temp->getNext();
        }


    }    
    


}