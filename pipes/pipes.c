#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/time.h>

int main(int argc,char** argv){
	if(argc<2){
		printf("Usage: ./a.out <numberof ints>\n");
		return -1;
	}

	struct timeval start,end;
	int fd1[2],fd2[2];
	
	pipe(fd1);
	pipe(fd2);

	int *array;
	array=(int *)malloc(sizeof(int)*atoi(argv[1]));

	int ret=fork();
	int status;

	if(ret!=0){
		int i;
		for(i=0;i<atoi(argv[1]);i++){
			array[i]=i;
		}

		printf("Size = %lu\n",sizeof(int)*atoi(argv[1]));
		
		gettimeofday(&start,NULL);

		close(fd1[0]);
		//printf("Parent writing\n");
		write(fd1[1],array,sizeof(int)*atoi(argv[1]));	
		//close(fd1[1]);

		waitpid(ret,&status,0);
		gettimeofday(&end,NULL);
	
	  	printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));
	}
	else{
		close(fd1[1]);
		//printf("Child reading\n");
		read(fd1[0],array,sizeof(int)*atoi(argv[1]));
		//close(fd1[0]);
		
		int i;
		for(i=0;i<atoi(argv[1]);i++){
			printf("%d\n",array[i]);
		}
	}	

	return 0;
}
