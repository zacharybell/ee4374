#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "zbell_prime.h"

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int numThreads;


/**
 * Initializes the sPRIME_THREAD array with the correct number ranges. For example, if 
 * the range was from 1 to 6 and there were 2 threads, then thread one would get the 
 * range of 1 to 3 and thread 2 would get the range from 4 to 6.
 *
 * NOTE: This function modifies global state (primeThreadData array).
 *
 * Args:
 *      low - the lower bound of the search area for primes
 *      high - the upper bound of the search area for primes
 */
void init_sprime(unsigned int low, unsigned int high) {
    
    if (high < low) pthread_exit(NULL);         // defensive programming 

    /* Range is partitioned by the number of threads, remainder will be distributed. */
    unsigned int diff = high - low + 1; 
   
    unsigned int remainder = diff % numThreads;

    diff = diff - remainder;
    unsigned int range = diff / numThreads;     // interval of work for each thread

    if (range == 0) {                           // more defensive programming
        printf("\nToo many threads specified! More threads than numbers.\n");
    }

    // setup ranges in sPRIME_THREAD array
    int i;
    for (i=0; i<numThreads; i++) {

        primeThreadData[i].num = i;
        primeThreadData[i].current = low;
        primeThreadData[i].low = low;
        
        // distribute the remainder to the first threads one at a time
        if (remainder > 0) {
            primeThreadData[i].high = low+range;
            low = low+range+1;
            remainder--;
        } else {
            primeThreadData[i].high = low+range-1;
            low = low+range;
        }
    }
}


/**
 * Takes two files and concats the contents into one new file.
 *
 * Args:
 *      fp1 - pointer to first file
 *      fp2 - pointer to second file
 *      target - file name of new file
 */
void concat_files(FILE* fp1, FILE* fp2, char* target) {
    
    char tmp;
    FILE* fp = fopen(target, "w");

    // add contents from first file
    for (tmp = fgetc(fp1); tmp != EOF; tmp = fgetc(fp1)) {
        fputc(tmp, fp);
    } fclose(fp1);
    
    // add contents from second file
    for (tmp = fgetc(fp2); tmp != EOF; tmp = fgetc(fp2)) {
        fputc(tmp, fp);
    } fclose(fp2);

    fclose(fp);
}


/**
 * Searches a range of numbers for a set of prime numbers and prints the 
 * numbers to a file. The file will be called "prime#.txt" where # is the 
 * thread number(num) specifiec in the sPRIME_THREAD argument.
 *
 * Args:
 *      args - the thread data (thread number, current number, low, and high)
 *              must be castable to type sPRIME_THREAD
 */
void *prime_search(void* args) {

    sPRIME_THREAD* td = (sPRIME_THREAD*)args;

    if (td->current < td->low) pthread_exit(NULL);
    
    char file_name[100];
    sprintf(file_name, "prime%u.txt", td->num+1);       // 0 index thread is thread #1, ect.
    FILE* fp = fopen(file_name, "w");

    while (td->current <= td->high) {
        if (test_prime(td->current)) fprintf(fp, "%u\n", td->current);
        td->current++;
    }

    if (td->current-1 == td->high) td->current--;       // not really necessary but looks good in shell

    fclose(fp);
}


/**
 * Mini shell allows users to query the progress of prime searching by 
 * outputing the current number from the given thread.
 *
 * Args:
 *      param - not implemented
 */
void *mini_shell(void *param) {
    
    int inputSize, i;
    int threadNum;
    char buff[128];

    while(1) {
	// Request a line of input
	write(1, " -> ", 4);
	// Read the line
	inputSize = read(0, buff, 128);
	// Strip off carriage return
	buff[inputSize-1] = '\0';
	
	if((buff[0] >= '1') && (buff[0] <= '9')) {
	    // An integer was entered
            threadNum = buff[0] - '0';
	    if(threadNum <= numThreads) {
		printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum-1].current);
	    } else {
		printf("Thread %d does not exist\n", threadNum);
	    }
			
	} else {		
	    if(buff[0] == 'a') {
		// Print status for all threads
		for(i = 0; i < numThreads; i++) {
		    printf("Thread %d progress: %d\n", i+1, primeThreadData[i].current);
		}
	    } else {
		printf("Unrecognized input\n");
	    }
	}

	printf("\n");
	fflush(NULL);

    }

    pthread_exit(0);
}


/**
 * Determine if a given number is prime.
 *
 * Args:
 *      n - the number
 * Returns:
 *      1 if prime, 0 otherwise
 */
int test_prime(int n) {
    if (n <= 1) return 0;
    int k = floor(sqrt(n));

    int i;
    for (i=2; i<=k; i++)
        if (n % i == 0) return 0;

    return 1;
}
