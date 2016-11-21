#include <unistd.h>		//for fork()
#include <stdio.h>
#include <stdlib.h>	

// fork man page: http://linux.die.net/man/2/fork
/*
fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process, referred to as the parent, except for the following points:
*/
int main()
{
    	int pid = 0;
    	pid = fork();
	if( pid < 0 )		//Fork Failed
    	{
		fprintf( stderr, "Fork Failed");        
		exit(-1);
    	}
    	else if (pid == 0)	//Child's Process 
    	{
		// getpid() returns the process ID of the calling process. http://linux.die.net/man/2/getpid 
		// getppid()  returns the process ID of the parent of the calling process.  http://linux.die.net/man/2/getpid 
		printf("PPID: %d\n",getppid());
		sleep(1.5);
		printf("PPID: %d\n", getppid());	
    	}
    	else			//Parent's Process
    	{
		sleep(1);	//Sleep until the child prints out the parent pid
    	}    
    	return 1;
}
