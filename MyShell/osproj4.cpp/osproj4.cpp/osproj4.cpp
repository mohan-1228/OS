//
//  main.cpp
//  osproj4.cpp
//
//  Created by Mohan Thapa on 9/22/23.
//********************************************************************
//
// Mohan Thapa
// CSC 360:Operating Systems
// Programming Project #4:  Process Synchronization Using Pthreads:
//                          The Producer / Consumer Problem With Prime Number Detector
// September 27, 2023
// Instructor: Dr. Siming Liu
//
//********************************************************************


#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _BUFFER_H_DEFINED_
#define _BUFFER_H_DEFINED_
#ifndef STATS_H
#define STATS_H

using namespace std;
int counting=0;
int buf_emp=0;
int buf_ful = 0;

typedef int buffer_item;

#define BUFFER_SIZE 5
buffer_item global_buffer[BUFFER_SIZE];
 int bufferCount=0; //keeps track of number of items in buffer
 int buffer_In_index=0; //index position for putting stuff in buffer
 int buffer_Out_index=0; //index position for pulling stuff out of buffer



class Stats
{
    
private:
    //vars
    
    

public:
    //vars
    
    int *totalThreadNumProduced; //holds # of items produced in array for each thread
    int *totalThreadNumConsumed; //holds # of items consumed in array for each thread
    int numTimesBufferFull; //total # of times the buffer was full
    int numTimesBufferEmpty; //total # of times the buffer was empty
    int numItemsRemaining;
    int totalNumConsumed; //gets # of items consumed
    int totalNumProduced; //gets # of items produced
    int simulationTime; //holds simulation time
    int maxThreadSleepTime; //holds max sleep time for threads
    int numProducerThreads; //holds # of producer threads
    int numConsumerThreads; //holds # of consumer threads
    int sizeOfBuffer; //holds size of the buffer
    


//functions
void print(); //does the output at the end
    Stats(int, int, int, int, int); //Constructor to set all the stuff we get from cli
    ~Stats(); //destructor that gets rid of dynamic elements
};

/********************************************************************
// Stats Constructor Function

*******************************************************************/

Stats::Stats(int _simTime, int _maxSleep, int _numProducerThreads, int _numConsumerThreads, int _bufferSize)
{

simulationTime = _simTime;
maxThreadSleepTime = _maxSleep;
numProducerThreads = _numProducerThreads;
numConsumerThreads = _numConsumerThreads;
sizeOfBuffer = _bufferSize;


totalThreadNumProduced = new int[_numProducerThreads];
totalThreadNumConsumed = new int[_numConsumerThreads];

}

/********************************************************************
// Stats Function

*******************************************************************/

Stats::~Stats()
{
delete [] totalThreadNumProduced;
delete [] totalThreadNumConsumed;
}

/********************************************************************
// Print Function

*******************************************************************/
void Stats::print()
{

cout << "PRODUCER / CONSUMER SIMULATION COMPLETE\n";
cout << "=======================================\n";
cout << "Simulation Time:" << setw (25) << simulationTime << endl;
cout << "Maximum Thread Sleep Time:"  << setw(15) << maxThreadSleepTime << endl;
cout << "Number of Producer Threads:" << setw(14) << numProducerThreads << endl;
cout << "Number of Consumer Threads:" << setw(14) << numConsumerThreads << endl;
cout << "Size of Buffer:" << setw(26) << sizeOfBuffer << "\n\n";

//Producer threads
int totalProd = 0;

//-----sum up the amount of items produced by all threads
for(int i=0; i < numProducerThreads; i++)
totalProd += totalThreadNumProduced[i];

//-----display total found above
cout << "Total Number of Items Produced:" << setw(10) << totalProd << endl;

//-----for each producer thread show # of items produced
for(int i=0; i < numProducerThreads; i++)
{
cout << " Thread " << setw(2);
cout << (i+1) << ":" << setw(30);
cout << totalThreadNumProduced[i] << endl;
}

cout << endl;

//Consumer threads
int totalCons = 0;

//------sum up the amount of items consumed by all threads
for(int i=0; i < numConsumerThreads; i++)
totalCons += totalThreadNumConsumed[i];

//------display total # of items consumed
cout << "Total Number of Items Consumed:" << setw(10) << totalCons << endl;

//-----for each consumer thread show # of items produced
for(int i=0; i < numConsumerThreads; i++)
{
cout << " Thread " << setw(2);
cout << (i+numProducerThreads+1) << ":" << setw(30);
cout << totalThreadNumConsumed[i] << endl;
}

cout << endl;

cout << "Number Of Items Remaining in Buffer:" << setw(6) << numItemsRemaining << endl;
cout << "Number Of Times Buffer Was Full:" << setw(10) << buf_ful << endl;
cout << "Number Of Times Buffer Was Empty:" << setw(9) << buf_emp << endl;

}

#endif


//function prototypes
bool buffer_insert_item( buffer_item item );
bool buffer_remove_item( buffer_item *item );
void buffer_print();
void buffer_initialize();

//Semphores and mutexs
sem_t bufferEmpty;
sem_t bufferFull;
pthread_mutex_t mutex;

/********************************************************************
// Buffer Initialize
*******************************************************************/
void buffer_initialize()
{
    sem_init(&bufferEmpty, 0, BUFFER_SIZE );
    sem_init( &bufferFull, 0, 0 );
    pthread_mutex_init( &::mutex, NULL );
    for(int i=0 ; i < BUFFER_SIZE; i++)
    {
        global_buffer[i] = -1;  //sets all buffer positions to -1
    }
}

/********************************************************************
// Buffer Insert Item Function

*******************************************************************/


bool buffer_insert_item( buffer_item item )
{
    cout << "Item: " << (int) item << endl; //debug print statement

    //put item into buffer
    global_buffer[buffer_In_index] = item;
    counting++;
    

    
    //move buffer in index plus one in circular fashion
    buffer_In_index = (buffer_In_index + 1) % BUFFER_SIZE;
    //cout << bufferCount << endl; //debug output buffercount

    //increase buffer count since item was inserted
    bufferCount++;

    return true;
}

/********************************************************************
// Buffer Remove Item Function

*******************************************************************/

bool buffer_remove_item( buffer_item *item )
{
    //Grab item from buffer
    *item = global_buffer[buffer_Out_index];
    counting--;
    
        
        //Move out to next index position in circular fashion
        buffer_Out_index = ( buffer_Out_index + 1) % BUFFER_SIZE;
        
        //Decrease number of items in buffer
        bufferCount--;
    

    return true;
}

/********************************************************************
// Linear Search Function

*******************************************************************/


void buffer_print()
{
    //line 1
    cout << "(buffers occupied: " << (int) bufferCount << ")\n";

    //line 2
    cout << "buffers: ";

       
    for(int i=0; i < BUFFER_SIZE; i++)
        cout << " " << (int) global_buffer[i] << "  "; //print #'s in buffer
    cout << endl; //start a new line

    // Third line of output. puts dashes under numbers
    cout << "        ";
    for(int i=0; i < BUFFER_SIZE; i++)
        cout << "  ---";
    cout << endl;

    // Fourth line of output. Shows position of in & out indexes
    cout << "         ";
    for(int i=0; i < BUFFER_SIZE; i++)
    {
        cout << " ";

        if(buffer_In_index == i || buffer_Out_index == i)
        {
            if( bufferCount == BUFFER_SIZE )
                cout << "WR";
            else if( bufferCount == 0 )
                cout << "RW";
            else if( buffer_In_index == i )
                cout << "W ";
            else
                cout << "R ";

            cout << "  ";
        }
        else
            cout << "    ";
    }
    cout << endl << endl;


}

#endif // _BUFFER_H_DEFINED_

//Structs
struct ARGS //struct to hold args from command line
{
    int simLength; //simulation length
    int maxThreadSleep; //maximum sleep time for thread
    int numProducers; //number of producers
    int numConsumers; //number of consumers
    bool outputBufferSnapshots; //true/false output buffer snapeshots
};

//Global args
ARGS global_args; //create an instance of arguments struct
bool runSim = true; //flag to indicate that the simluation should stop when false

//function prototype
void arguments(int, char  **);
bool checkPrime(buffer_item);
void *producer(void * param);
void *consumer(void * param);

int main(int argc, char *argv[]) {
    // Get command line arguments
    arguments(argc, argv);

    // Store simulation statistics
    Stats simulationStats(global_args.simLength, global_args.maxThreadSleep,
                          global_args.numProducers, global_args.numConsumers,
                          BUFFER_SIZE);

    // Initialize the buffer
    buffer_initialize();

    // Threads
    pthread_t tidP[global_args.numProducers]; // Producer thread identifier array
    pthread_t tidC[global_args.numConsumers]; // Consumer thread identifier array
    pthread_attr_t attr;                      // Set of thread attributes
    pthread_attr_init(&attr);                 // Get the default attributes

    cout << "Starting Threads...\n";

    // Create producer thread(s)
    for (int i = 0; i < global_args.numProducers; i++) {
        pthread_create(&tidP[i], &attr, producer, &simulationStats);
    }

    // Create consumer thread(s)
    for (int i = 0; i < global_args.numConsumers; i++) {
        pthread_create(&tidC[i], &attr, consumer, &simulationStats);
    }

    // Sleep for simulation duration
    sleep(global_args.simLength);

    // Signal threads to quit
    cout << "Stopping simulation...\n";
    runSim = false;

    // Join threads and collect statistics
    for (int i = 0; i < global_args.numProducers; i++) {
        void *status;
        
        pthread_join(tidP[i], &status);
        int *producerStats = (int *)status;
        simulationStats.totalThreadNumProduced[i] = producerStats[0];
        delete[] producerStats; // Free memory
    }

    for (int i = 0; i < global_args.numConsumers; i++) {
        void *status;
        pthread_join(tidC[i], &status);
        int *consumerStats = (int *)status;
        simulationStats.totalThreadNumConsumed[i] = consumerStats[0];
        delete[] consumerStats; // Free memory
    }

    // Calculate total produced and consumed items
    int totalProduced = 0;
    for (int i = 0; i < global_args.numProducers; i++) {
        totalProduced += simulationStats.totalThreadNumProduced[i];
    }

    int totalConsumed = 0;
    for (int i = 0; i < global_args.numConsumers; i++) {
        totalConsumed += simulationStats.totalThreadNumConsumed[i];
    }

    // Update statistics
    simulationStats.numItemsRemaining = bufferCount;
    //simulationStats.numTimesBufferFull = 0; // Update this if needed
    //simulationStats.numTimesBufferEmpty = 0; // Update this if needed
    simulationStats.totalNumProduced = totalProduced;
    simulationStats.totalNumConsumed = totalConsumed;

    // Display statistics
    simulationStats.print();

    // Exit
    return 0;
}



/********************************************************************
// Arguments function

*******************************************************************/



void arguments(int argc, char  *argv[])
{
    if(argc < 6)
    {
        cerr << "5 arguments are needed. The arguments should be ";
        cerr << "in the following order:\n  simulation length\n  ";
        cerr << "max thread sleep\n  number of producers\n  number ";
        cerr << "of consumers\n yes or no for outputting buffer snapshots\n";
        exit(-1);
    }

    global_args.simLength = atoi(argv[1]);
    global_args.maxThreadSleep = atoi(argv[2]);
    global_args.numProducers = atoi(argv[3]);
    global_args.numConsumers = atoi(argv[4]);
    
    if(strcmp(argv[5],"yes") == 0)
        global_args.outputBufferSnapshots = 1;
    else if(strcmp(argv[5],"no") == 0)
        global_args.outputBufferSnapshots = 0;
    else
    {
        cerr << "The last argument must be 'yes' or 'no'\n";
        exit(-1);
    }


}

/********************************************************************
// Check Prime Function
// This function determines if a number is prime
*******************************************************************/

bool checkPrime(buffer_item in)
{
    int lim = in/2;
    int result;

    for(int i=2; i<=lim; i++)
    {
        result = in % i;
        if(result == 0)
            return false;
    }

    return true;

}

/********************************************************************
// Producer function
****************************************************************/

void *producer(void * param)
{
    //Variables
    int * stats = new int[2]; //used to return stats
    int time; //holds random number for time to sleep
    stats[0] = 0; //holds # of items produced
    stats[1] = 0; //holds # of times buffer is full
    buffer_item bItem; // this is my item
    


    while(runSim)
    {
        //Sleep for a random amount of time.
        time = rand();
        time = time % global_args.maxThreadSleep;

        sleep(time);

        //Generate a random number
        bItem = rand()%100;
       
       
        cout << "Producer item: " << bItem << endl << endl;

        //Check to see if buffer is full
        if(bufferCount >= BUFFER_SIZE)
        {
            cout << "All buffers full.  Producer " << pthread_self();
            cout << " waits.\n\n";
            stats[1]=(stats[1] + 1);
            buf_ful++;
        }else{
            
            //Wait
            sem_wait( &bufferEmpty );
            
            //Lock
            pthread_mutex_lock( &::mutex );
            
            //Produce - CRITICAL SECTION
            if(runSim && buffer_insert_item(bItem))
            {
                cout << "Producer " << pthread_self() << " writes ";
                cout << bItem << endl;
                
                stats[0]= (stats[0] + 1);
                
                
                if(global_args.outputBufferSnapshots) //if snapshots enabled
                    buffer_print(); //print snapshots
            }
            
            //unlock
            pthread_mutex_unlock( &::mutex );
            
            //signal
            sem_post( &bufferFull );
        }
        
        
        if (bufferCount == BUFFER_SIZE) {
                    stats[1] = (stats[1] + 1); // Increment the buffer full counter
                }

    }

    pthread_exit( (void*)stats ); //return stats array
}

/********************************************************************
// Consumer function

*******************************************************************/

void *consumer(void * param)
{
    //Variables
    buffer_item bItem; //bItem to store received item
    int time; //int to hold random time to sleep
    int * stats = new int[2]; //array explain on next two lines
    stats[0] = 0; //holds # of items consumed
    stats[1] =0; //holds # of times buffer was empty
    int consumer_id = *((int *)param); // Extract the producer ID
    pthread_t id = pthread_self();


    while(runSim)
    {
        //Sleep for a random amount of time
        time = rand();
        time = time % global_args.maxThreadSleep;
        sleep(time);

        //Check if buffers are empty
        if(bufferCount == 0)
        {
            cout << "All buffers empty.  Consumer ";
            cout << id;
            cout << " waits.\n\n";
            buf_emp++;
        

            stats[1]= (stats[1] + 1); //add one time buffer was empty
        }else{
            
            //Check semaphore
            sem_wait( &bufferFull );
            
            //Lock
            pthread_mutex_lock( &::mutex );
            
            //Consume
            if(runSim && buffer_remove_item( &bItem)) //consumes in if statement
            {
                //line 1
                cout << "Consumer " << pthread_self() << " reads ";
                cout << bItem;
                
                stats[0] = (stats[0]+1);
                
                //increase # of items consumed
                
                if(checkPrime(bItem)) //find if # was prime
                    cout << "   * * * PRIME * * *";
                cout << endl;
                
                //if snapshots are enabled
                if(global_args.outputBufferSnapshots)
                    buffer_print();
            }
            
            //Unlock
            pthread_mutex_unlock( &::mutex );
            
            //Signal
            sem_post( &bufferEmpty );
        }
        
        if (bufferCount == 0) {
            stats[1] = (stats[1] + 1);
        }

    }

    pthread_exit( (void*)stats ); //return stats for this thread

}
