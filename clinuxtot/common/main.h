#ifndef __MAINH__GUARD_H
#define __MAINH__GUARD_H

static const char * mqueue_name = "/msgmsg2";
static const char * conf_mapping_file = "/tmp/map_conf";

typedef short PORT_T;

typedef struct my_msg_t {
	int msgid;
	int parent;
	char msg_data[50];
} my_msg;

#define MAX_PORTS 250
#ifdef SERVER
struct server_configuration_t {
	short nports;
	PORT_T listen_ports[MAX_PORTS];
	short client_name;
};
#elif CLIENT
struct client_configuration_t {
	short nports;
	PORT_T dest_ports[MAX_PORTS];
	char  pad[sizeof(short)]; //zero padding
};
#endif


#endif
