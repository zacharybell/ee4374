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
int	numThreads;


void concat_files(FILE* fp1, FILE* fp2, char* target) {
    
    char tmp;
    FILE* fp = fopen(target, "w");

    // add contents from first file
    for (tmp = fgetc(fp1); tmp != EOF; tmp = fgetc(fp1)) {
        fputc(tmp, fp);
    } 
    fclose(fp1);
    
    // add contents from second file
    for (tmp = fgetc(fp2); tmp != EOF; tmp = fgetc(fp2)) {
        fputc(tmp, fp);
    } 
    fclose(fp2);

    fclose(fp);
}


/**
 * Searches a range of numbers for a set of prime numbers and prints the 
 * numbers to a file. The file will be called "prime#.txt" where # is the 
 * thread number(num) specifiec in the sPRIME_THREAD argument.
 *
 * Args:
 *      td - the thread data (thread number, current number, low, and high)
 */
void *prime_search(sPRIME_THREAD* td)
{
    if (td->current < td->low) pthread_exit(NULL);
    
    char file_name[100];
    sprintf(file_name, "prime%u.txt", td->num);
    FILE* fp = fopen(file_name, "w");

    while (td->current <= td->high) {
       if (test_prime(td->current)) fprintf(fp, "%u\n", td->current);
       td->current++;
    }
    
    fclose(fp);
}

void *mini_shell(void *param)
{
	int inputSize, i;
	int threadNum;
	char buff[128];		// Character buffer

	while(1)
	{
		// Request a line of input
		write(1, " -> ", 4);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize-1] = '\0';
		
		if((buff[0] >= '1') && (buff[0] <= '9'))
		{
			// An integer was entered
			threadNum = buff[0] - '0';
			if(threadNum <= numThreads)
			{
				printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum-1].current);
			}
			else
			{
				printf("Thread %d does not exist\n", threadNum);
			}
			
		}
		else
		{		
			if(buff[0] == 'a')
			{
				// Print status for all threads
				for(i = 0; i < numThreads; i++)
				{
					printf("Thread %d progress: %d\n", i+1, primeThreadData[i].current);
				}
			}
			else
			{
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
int test_prime(int n)
{
    if (n <= 1) return 0;
    int k = floor(sqrt(n));

    int i;
    for (i=2; i<=k; i++)
        if (n % i == 0) return 0;

    return 1;
}
