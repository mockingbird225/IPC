#include<stdio.h>
#include<semaphore.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<unistd.h>
#include <time.h>
#include <math.h>

#define SNAME "/mysem"

/* assembly code to read the TSC */
static inline uint64_t RDTSC()
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

/* Call once before using RDTSC, has side effect of binding process to CPU1 */
void InitRdtsc()
{
  unsigned long cpuMask;
  cpuMask = 2; // bind to cpu 1
  sched_setaffinity(0, sizeof(cpuMask), &cpuMask);
}

int main(int argc, char* argv[])
{
	if(argc < 4)
		return;
  	sem_t *my_semaphore;
  	char* virt_add;
        int md, status;
	struct timespec start;
	char * temp = malloc(atoi(argv[3]));
	
	//long pg_size;
	int bytes = atoi(argv[3]);
	int x;
  	my_semaphore = sem_open(argv[2], O_CREAT, S_IRUSR | S_IWUSR, 0);
        md = shm_open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        //pg_size = sysconf(_SC_PAGE_SIZE);
        if(ftruncate(md, bytes) == -1)
        {
                perror("ftruncate failure");
        }
        virt_add = (char*) mmap(0, bytes, PROT_WRITE, MAP_SHARED, md, 0);    
        sem_wait(my_semaphore);
	memcpy(temp,virt_add,bytes);
	clock_gettime(CLOCK_MONOTONIC_RAW,&start);
 	unsigned long long starttime = start.tv_sec*pow(10,9) + start.tv_nsec;
	printf("  END: %llu\n",starttime);
	//printf("END: %" PRIu64 "\n", RDTSC());
	//printf("%s\n", virt_add); 
	status = munmap(virt_add, bytes);
        status = close(md);
        status = shm_unlink("my_memory");
}
