#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>


#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <poll.h>

#define SERVER
#include "main.h"
#include <stdlib.h>

extern char **environ;

struct client_info_struct_t {
	int source_socket;
	int mqid;
};


static volatile int msg_id = 0;
pthread_mutex_t m;

void *handle_client_thread_function(void *args) {
	int new_sock = 0;
	pid_t tid = syscall(SYS_gettid);
	pid_t tid2 = pthread_self();
	struct client_info_struct_t *input = (struct client_info_struct_t *)args;
	
	if (!input) {
		fprintf(stderr,"%s received invalid input\n", __func__);
		return NULL;
	}
	
	struct sockaddr receving_end;
	socklen_t addrlen = 0;
	while ((new_sock = accept(input->source_socket,&receving_end,&addrlen))>0)  {
		printf("successfull connection\n" );
		char buf[50] = {'\0'};
		ssize_t len = recv(new_sock,buf,50,0);
		
		pthread_mutex_lock(&m);
		my_msg msg = {.msgid=msg_id++, .parent=getppid()}; 
		strncpy(msg.msg_data, buf, 50);
		int res = mq_send(input->mqid, (const char *)&msg, sizeof(msg), 1);
		if (res == -1) {
			fprintf(stderr, "%s\n",(char *)strerror(errno));
			printf("sending data failed with errno %d\n", errno);
		}
		pthread_mutex_unlock(&m);
		close(new_sock);
		if (!strncmp(msg.msg_data,"BYE",3))
			break;
	}


	printf("thread %d or %d died\n",tid,tid2);
	return NULL;

}

void receive_http_requests(int prtcl, short listen_ports[], int n_ports, char *ip, mqd_t mqid) {

	//deal with address info
	struct sockaddr_in s_addr;
	struct in_addr ip_addr;
	memset(&s_addr,0,sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	if (ip != NULL) {
		memset(&ip_addr,0,sizeof(struct in_addr));
		if (inet_pton(s_addr.sin_family,ip,&ip_addr) != 1) {
			fprintf(stderr, " inet_pton failed \n ");
		}
	} else {
		ip_addr.s_addr = INADDR_LOOPBACK;
		ip = inet_ntoa(ip_addr);
	}
	s_addr.sin_addr = ip_addr;

	//bind sockets to address
	int *sockets = (int *)malloc(n_ports * sizeof(int));
	for (int sock_i = 0; sock_i < n_ports; sock_i++) {
		sockets[sock_i] = socket(AF_INET, SOCK_STREAM, 0);
		if (sockets[sock_i] == -1) {
			fprintf(stderr,"unable to set up socket with protcol %d\n", prtcl);
			return;
		}

		s_addr.sin_port  = htons(listen_ports[sock_i]); 
		if (bind(sockets[sock_i], (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
			fprintf(stderr, "bind failed: %s on ip: %s and port %d\n",(char *)strerror(errno), ip, ntohs(s_addr.sin_port));
		}

		if (listen(sockets[sock_i], 100) == -1) {
			if (errno == EADDRINUSE) { 
				fprintf(stderr, "%s %d\n",(char *)strerror(errno), errno);
			}	
			else if (errno == EOPNOTSUPP) {
				printf("somehting");
			}
		}

#ifdef NETWORK_DEBUG
		printf("socket is up!\n");
#endif
	}
	
	int n_sockets = n_ports;
	short events = POLLIN | POLLPRI;
#ifdef __USE_GNU
	short err_events = POLLRDHUP | POLLERR | POLLNVAL | POLLHUP;
#else	
	short err_events = POLLERR | POLLNVAL | POLLHUP;
#endif
	struct pollfd *pollfd_info = calloc(n_sockets,sizeof(struct pollfd));
	for (int i = 0; i < n_sockets; i++) {
		pollfd_info[i].events = events;
		pollfd_info[i].revents = 0;
		pollfd_info[i].fd = sockets[i];
	}
	
	free(sockets);
	int n_requests = 0;
	while (1) {
		if (n_requests = poll(pollfd_info,n_sockets,-1)) { 
			for (int i = 0; i < n_sockets && n_requests > 0; i++) {
				if (pollfd_info[i].revents & events)  {
					struct client_info_struct_t input = { .source_socket=pollfd_info[i].fd , .mqid=mqid }; 
					pthread_t tid;
					pthread_create(&tid,NULL,&handle_client_thread_function,&input);
					pollfd_info[i].revents = 0;
					n_requests--;
				} 
			}
		}
	}
	free(pollfd_info);
	pthread_mutex_destroy(&m);

	
}
void *handle_dead_children(void *arg) {
	int status;
//	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))
//		printf("__func__ failed\n");
	while (1) {
		pid_t child_pid = wait(&status);
		if (child_pid == -1) {
			//no children error
			if (errno == 10)
				return NULL;
			printf("__func__ failed: %d and %s\n", errno, strerror(errno));
		}
		printf("pid %d stdout,error %d\n",child_pid, status);
		if (WIFSIGNALED(status))
			printf("out by signal %s\n", strsignal(WTERMSIG(status)));
		if (WIFSTOPPED(status))
			printf("stopped by signal %s\n", strsignal(WSTOPSIG(status)));
	}
	return NULL;
}

void printPath() {

	char *s = getenv("PATH");
	char *sub_path = strtok(s,":");	
	while (sub_path != NULL) {
#ifdef PATH_DEBUG
		printf("%s\n",sub_path);
#endif
		sub_path = strtok(NULL,":");
	}
	char **e = environ;        	
	while (e != NULL && *e != NULL) {
		if (!strncmp(*e,"PATH",4)) {
			char *new_path_var = (char *)malloc(strlen(*e)+strlen(":/home/aviva/clinuxtot/:.")+1);
			sprintf(new_path_var,"%s%s",*e,":/home/aviva/clinuxtot/:.");
			*e = new_path_var;
#ifdef PATH_DEBUG
			printf("path manipulated: %s\n",*e);
#endif
		}
		e++;
	}
	
}

void proc_child_process(mqd_t mqid) {
		printPath();
//		close (2); close(1);
		mq_close(mqid);	
		execlp("client","client",(char *)NULL);
		fprintf(stderr, "client failed to instantiate: %d,%s\n",errno,(char *)strerror(errno));
		exit(1);
}
void setup_reader_units(int n_readers, mqd_t mqid, int **child_pids ) {
	pid_t pid;
	*child_pids = (int *)malloc(n_readers * sizeof(int));
	int reader_i = 0;
	while (pid != 0 && reader_i < n_readers) {
		pid = fork();
		if (pid == 0 ) { 
			proc_child_process(mqid);
		} else if (pid == -1) {
			fprintf(stderr, "%s\n",(char *)strerror(errno));
			exit(-1);
		} else {
			(*child_pids)[reader_i++] = pid;
		}
	}
}
void setup_child_sig_thread(pthread_t *sig_thread_id) {
#ifdef NETWORK_DEBUG
		printf("master\n");
#endif
		if (!(pthread_create(sig_thread_id,NULL,&handle_dead_children,NULL)))
			printf("handle children thread is up!\n");
}
void setup_mq(struct mq_attr *attr, mqd_t *mqid) {
	attr->mq_msgsize = sizeof(my_msg);
	attr->mq_maxmsg = 10; 
	*mqid = mq_open (mqueue_name,O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, attr) ;
#ifdef NETWORK_DEBUG
	printf ("new messege queue of id %d\n",*mqid);
#endif

	if (*mqid == (mqd_t) -1) {
		fprintf(stderr, "%s FAILED! %s %d\n",__func__ ,(char *)strerror(errno), errno);
		fprintf(stderr, "%s %ld %ld\n",__func__,attr->mq_msgsize,attr->mq_maxmsg);
		exit(-1);
	}
}



void execute_server(mqd_t mqid) {


		pthread_mutexattr_t attr;
		attr.__align = PTHREAD_MUTEX_ADAPTIVE_NP;

		pthread_mutex_init(&m,&attr);
		short listen_ports[] = {8081,8080,8079};
		receive_http_requests(AF_INET,listen_ports,3,"127.0.0.1",mqid);
		/*while (1) {
			my_msg msg = {.msgid=msg_id++, .parent=pid? getpid():getppid(), .msg = {'c'}}; 
			int res = mq_send(mqid, (const char *)&msg, sizeof(msg), 1);
			if (res == -1) {
				fprintf(stderr, "%s\n",(char *)strerror(errno));
				printf("sending data failed with errno %d\n", errno);
			}
		}*/
}
void on_program_exit(pthread_t sig_thread_id, int **child_pids, int n_readers) {
	int r = 0;
	if (r = pthread_cancel(sig_thread_id)) {
		printf("%d\n",r);
		fprintf(stderr,"thread joining failed with errno %d and %s\n", errno, strerror(errno));
	}
	int *pids = *child_pids;
	int i = 0;
	for (; i < n_readers ; i++) {
		kill(pids[i],SIGTERM);
		printf("client with pid %d is kaputt\n",pids[i]);

	}
	free(*child_pids);


}

int main(int argc, char **argv) 
{
	struct mq_attr attr;
	pthread_t sig_thread_id;
	mqd_t mqid;
	int *child_pids;
	struct server_configuration_t *config = NULL;
	build_config(&config,"server_configure.json");
	setup_mq(&attr, &mqid);
	setup_reader_units(5,mqid,&child_pids);
	setup_child_sig_thread(&sig_thread_id);
	execute_server(mqid);
	on_program_exit(sig_thread_id,&child_pids,2);
	return 0;
}
