

#define _GNU_SOURCE
#define __USE_GNU
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void *connect_to_port_thread(void *args) {

	pthread_detach(pthread_self());
	int sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in s_addr;
	struct in_addr ip_addr;
	short port = *((short *)args);

	memset(&s_addr,0,sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	const char * ip = "127.0.0.1";
	if (inet_pton(s_addr.sin_family,ip,&ip_addr) != 1) {
		fprintf(stderr, " inet_pton failed \n ");
		ip_addr.s_addr = ntohl(INADDR_LOOPBACK);
	}
	s_addr.sin_addr = ip_addr;
	s_addr.sin_port = htons(port);


	if (!connect(sock,(struct sockaddr *)&s_addr,sizeof(s_addr))) {
		char base_txt[250] = {0};
		strcpy(base_txt,"mambo jambo");
		strfry(base_txt);
		send(sock,(const void *)base_txt,strlen(base_txt)+1,0);
	} else {
	    printf("error on socket to port %d: %s\n",port, strerror(errno));
	    close(sock);
	    return NULL;
	}
	close(sock);

}
void connect_to_server(short ports[], int n_ports) {
	int i;

	pthread_t *ids = (pthread_t *)malloc(n_ports*sizeof(pthread_t));
	memset(ids,0,sizeof(pthread_t)*n_ports);
	while (1) 
	for (i = 0; i < n_ports; i++) {
		pthread_t id;
		ids[i] = pthread_create(&id,NULL,&connect_to_port_thread,(void *)&ports[i]);
	}

}

int main() {

	short ports[] = {8079,8080,8081};
	connect_to_server(ports, sizeof(ports)/sizeof(ports[0]));

}
