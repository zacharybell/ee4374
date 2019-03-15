// UT El Paso EE 4374 Assignment 3
// Multi-threaded Prime Number Search
// Author: Zachary Bell
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "zbell_prime.h"

#define BUFFER_SIZE	1000000

unsigned char buffer[BUFFER_SIZE+1];
unsigned char fileName[100];
FILE *primeFile;
FILE *primeThreadFile;


int main(int argc, char *argv[]) {
    int i, bytesRead, bytesWritten, rc;
    pthread_t tid[MAX_THREADS]; 
    pthread_t tidshell;
    pthread_attr_t attr;
    time_t before, after;

    /* Record time at start */
    before = time(NULL);

    /* Setup threads to find prime numbers */
    pthread_attr_init(&attr);
    numThreads = 2;
  
    /* Initialize thread data (i.e. number ranges */
    init_sprime(1, 5000000); // 5 million !!!


    /* Setup a mini shell thread to provide interactivity with the user */
    pthread_create(&tidshell,&attr,mini_shell,NULL);


    /* Setup worker threads */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i=0; i<numThreads; i++) {
        pthread_create(&tid[i], &attr, prime_search, (void*)&primeThreadData[i]);
    } 
    pthread_attr_destroy(&attr);


    /* Wait for threads to complete */
    for (i=0; i<numThreads; i++) {
        rc = pthread_join(tid[i], NULL);
        if (rc) exit(-1);
    }

    /* Concat both the thread files (prime*.txt) into primes.txt file. */
    // used for testing (normally, I wouldn't hardcode this)
    FILE* fp1 = fopen("prime1.txt", "r");
    FILE* fp2 = fopen("prime2.txt", "r");
    concat_files(fp1, fp2, "primes.txt"); 

    sleep(20);
  
        
    /* Lastly, kill the interaction thread */
    pthread_kill(tidshell, SIGKILL);

    return 0;
}

