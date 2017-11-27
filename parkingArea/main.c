#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h> // forking
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h> //mssget, msgsnd, msgrcv
#include <stdlib.h> // exit

#include <signal.h> //signal functions


#include <sys/signal.h> //signal functions
#include <pthread.h> //thread functions


#include <sys/wait.h> // wait function and macros

#define WRITEEND 1
#define READEND 0

int msqid = 0;

typedef struct msgbuf {
	unsigned long msgtype;
	char msgdata[];
} message;

void omni_sig_handler(int signum) {

	psignal(signum," __FUNC__ Some exotic signal was received ! ");

}

enum {
	FATHER_THREAD_1 = 0,
	FATHER_THREAD_2,
	MAX_THREADS
};

void signal_handler_onkill(int signum, siginfo_t *sigi, void * context) {
	const char msg[200] = {'\0'};
	strncpy(msg,__PRETTY_FUNCTION__,200);
	psiginfo(sigi, msg);
	msgctl(msqid, IPC_RMID, NULL);
	exit(1);

}

void signal_handler_chld(int signum, siginfo_t *sigi, void * context) {

	psiginfo(sigi, __PRETTY_FUNCTION__ );

	int status = -1;
	waitpid(-1, &status, WUNTRACED | WCONTINUED );

	if (WIFSIGNALED(status)) {
		fprintf(stderr,"children are dead over here by external signal\n");
		signal_handler_onkill(signum, sigi, context);
	} 
	else if (WIFSTOPPED(status)) {

		fprintf(stderr,"children is stopped\n");
	}
	else if (WIFCONTINUED(status)) {

		fprintf(stderr,"children is continued\n");
	}

}



pthread_t brotherThreadsIDs[MAX_THREADS] = {0};

void *sig_thread_start_father( void *threadsToKill) {


	int *threadsToKillParsed = (int *)threadsToKill;


	printf("New signal thread is up ... \n");


	//block and ignore all signals
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_SETMASK, &set , NULL );

	//apart from those....
	sigemptyset(&set);
	sigaddset(&set,SIGTSTP);
	sigaddset(&set,SIGCONT);
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGTERM);
	sigaddset(&set,SIGCHLD);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL );
	//sigprocmask

	//this thread will change the default behaviour (i.e. provide signal handler) of INT and CHLD signals
	struct sigaction sigstruct;
	sigstruct.sa_sigaction = signal_handler_onkill;
	sigstruct.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sigstruct, NULL);	
	sigaction(SIGTERM, &sigstruct, NULL);	
	sigstruct.sa_sigaction = signal_handler_chld;
	sigaction(SIGCHLD, &sigstruct, NULL);	
	
	//this loops iterates over all signals instead of other threads in this program.
	//it prints each invoked signal details for debug purposes.
	//but when a SIGKILL comes... a great shadow will loom above us
	while(1) {} 
	while(0) {
		int signum = 0;
		if (sigwait(&set, &signum) == 0 ) {
			printf("signal was received ... "); fflush(stdout);
			psignal(signum," Some exotic signal was received:");
			if (signum == SIGTERM) {

				printf("signal SIGKILL was received ... ");
				//msgkey isn't known in this scope
				msgctl(msqid, IPC_RMID, NULL);

				//loop all over sibiling threads and KILL
				for (int i = 0; i < MAX_THREADS && brotherThreadsIDs[i] != 0 ; i++)
					pthread_kill( threadsToKillParsed[i], SIGKILL );
			}
		}
		
	}

}


int main(int argc, char **argv) {
	
	int fds[2];
	int result = pipe(fds);
	char msg[100];
	
	long msqkey = ftok("/bin/rm",'K');
	msqid = msgget(msqkey, IPC_CREAT | 0666);
	perror(strerror(errno));

	assert(result==0);
	assert(msqid>=0);
	assert(msqkey>=0);

	sigset_t set;
	sigemptyset(&set);
	pthread_sigmask(SIG_SETMASK, &set, NULL); 

	if (result = fork()) //PARENT
	{
		//close(1);
		//close(2);

		sigfillset(&set);
		pthread_sigmask(SIG_SETMASK, &set, NULL); 

		pthread_t sigthreadfather;
		pthread_attr_t attr;
		pthread_create(&sigthreadfather, &attr, sig_thread_start_father , NULL);
		
		while (1) {
			fgets(msg,100,stdin);
			if (write(fds[WRITEEND],msg,sizeof(msg)) == -1)
			{
				//assuming ERRNO is EREAD and the children are DEAD
				//exit(1);	
			}
			//message callback;
			//callback.msgtype = 1;
			//	printf ("from father!: %s\n", callback.msgdata);
		}

	}
	else //CHILD
	{
		assert(result!=-1); //if fork failed then we will be here
		close(0);
		while (1) {
			if (read(fds[READEND],msg,sizeof(msg)) != -1){
				printf("%s\n",msg);
			}
			else{
				exit(1);
			}
			/*message element ;
			element.msgtype = 1;a */
			//strncpy(element.msgdata,"HELLO ME IM SO PRETTY",100);
			//if (msgsnd(msqid,&element,strlen(element.msgdata),0) != 0)
			//{
				//perror(strerror(errno));
				//exit(1);
			//}
		}


	}

}
