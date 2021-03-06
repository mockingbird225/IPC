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
#include <math.h>
#include <time.h>

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

int main(int argc, char *argv[])
{
	if(argc < 3)
		return 0;
	sem_t *my_semaphore;
	char* virt_addr;
	int md, status;
	struct timespec start;
	sleep(1);
	//long pg_size;
	int bytes = atoi(argv[3]);
	//my_semaphore = sem_open(SNAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
	my_semaphore = sem_open(argv[2], 0);
	md = shm_open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	//pg_size = sysconf(bytes);
	if(ftruncate(md, bytes) == -1) 
	{
		perror("ftruncate failure");
	}	

	virt_addr = (char*) mmap(0, bytes, PROT_WRITE, MAP_SHARED, md, 0);
	clock_gettime(CLOCK_MONOTONIC_RAW,&start);
	memset(virt_addr, '1',bytes); 	
	sem_post(my_semaphore);
 	unsigned long long starttime = start.tv_sec*pow(10,9) + start.tv_nsec;
	printf("START: %llu\n",starttime);
	//printf("%" PRIu64 "\n", RDTSC());
	fflush(stdout);
	status = munmap(virt_addr, bytes);
	status = close(md);
	status = shm_unlink("my_memory");
}
