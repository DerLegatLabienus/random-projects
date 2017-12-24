#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int connect_to_mq() {

	mqd_t mqid = mq_open(mqueue_name, O_RDONLY);
	struct mq_attr mqattr;
	mq_getattr(mqid,&mqattr);
	printf("size of msg %ld\n",mqattr.mq_msgsize);
	printf("size of %d\n",sizeof(my_msg));
	if (mqid == -1) {
		fprintf(stderr, "process %d failed connecting mqueue named %s with error: %s", 
				getpid(), mqueue_name, strerror(errno));
		exit(-1);
	}
	return mqid;
}

int main(int argc, char **argv) {

	int mqid = connect_to_mq();
	while (1) {
		my_msg buf;
		memset(&buf.msg_data,0,sizeof(buf.msg_data));
		if (mq_receive(mqid,(char *)&buf,sizeof(my_msg),NULL) == -1) {
			printf("%s error: %d, %s\n", __func__, errno,strerror(errno));
			exit(1);

		}
		if (buf.msgid < 0) {
			continue;
		}
		mq_getattr(
		printf("child process %d printed message of type %d\n",getpid(),buf.msgid);
		printf("content in size of is %s\n", buf.msg_data);
		//char *tmp = malloc(buf.msg_len * sizeof(char) + 1);
		//strncpy(tmp,buf.msg_data,buf.msg_len + 1);
	}

}
