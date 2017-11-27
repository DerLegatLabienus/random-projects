#define SERVER
#include "json.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>


const char *PORTS_HEADER = "ports";
const char *CLIENT_HEADER = "client_name";
const char *PORTS_NUMBER = "num";

int comparePorts(const void * port1, const void * port2) { 
	return *(PORT_T *)port1 - *(PORT_T *)port2;
}

void updateMappingAddr(struct server_configuration_t **p_to_conf_map) {
	int f = open(conf_mapping_file, O_RDWR | O_CREAT);
	if (f == -1) {
#ifdef CONFIGURATION_DEBUG
		fprintf(stderr,"open file for mmap failed!");
#endif
		fprintf(stderr,"%s\n",strerror(errno));
		exit (2);
	}
	char buf[sizeof(struct server_configuration_t)] = { 0 };
	ssize_t s = write(f,buf,sizeof(struct server_configuration_t));
	if (s == -1) {
#ifdef CONFIGURATION_DEBUG
		fprintf(stderr,"write file for mmap failed!");
#endif
		fprintf(stderr,"%s\n",strerror(errno));
		exit (2);
	}
	struct server_configuration_t *mapping = 
		(struct server_configuration_t *)mmap(
				(void *)*p_to_conf_map,
				sizeof(struct server_configuration_t),
				PROT_READ | PROT_WRITE,
				MAP_SHARED, f, 0);

	if (mapping == MAP_FAILED) {
#ifdef CONFIGURATION_DEBUG
		fprintf(stderr,"mapping failed!");
#endif
		fprintf(stderr,"%s\n",strerror(errno));
		exit (2);
	}
	if (mapping != *p_to_conf_map) {
#ifdef CONFIGURATION_DEBUG
		printf("mapping overrided!");
#endif
	}
	*p_to_conf_map = mapping;
	close(f);
}
struct server_configuration_t *build_config(struct server_configuration_t **p_to_conf_map, const char *conf_file) {
	updateMappingAddr(p_to_conf_map);
	struct server_configuration_t *config = *p_to_conf_map;
	memset(config,0,sizeof(struct server_configuration_t));
	FILE *conf_handler = fopen("server_configure.json","r");
	json_value *parsed_conf = NULL;
	if (conf_handler) {
		fseek(conf_handler,0,SEEK_END);
		long length = ftell(conf_handler);
		json_char *json = malloc(sizeof(json_char)*length);
		rewind(conf_handler);
		if (json) {
			fread(json,1,length,conf_handler);
			fclose(conf_handler);
		}
#ifdef CONFIGURATION_DEBUG
		for (int i = 0; i < length; i++)
			printf("%c",json[i]);
#endif
		parsed_conf = json_parse(json,length);
		if (parsed_conf->type == json_object) {
			json_object_entry *entries = parsed_conf->u.object.values;
			int n_entries = parsed_conf->u.object.length;
			for (int i = 0; i < n_entries; i++) { // ports, client_name entries
				if (!strcmp(PORTS_HEADER,entries[i].name)) { // array of ports {num , protocol}
					int port_i = 0;
					if (entries[i].value->type == json_array) {
						config->nports = entries[i].value->u.array.length;	
						json_value **arr = entries[i].value->u.array.values;
						for (int j = 0; j < config->nports; j++) { 
							json_value *val = arr[j];
							if (val->type == json_object) {
								int length = val->u.object.length; 
								for (int k = 0; k < length; k++) {
									json_object_entry ports_data  = val->u.object.values[k];
									if (!strcmp(ports_data.name,PORTS_NUMBER)) {
										config->listen_ports[port_i++]=ports_data.value->u.integer;
									}
									break;

								}
							} else {
#ifdef CONFIGURATION_DEBUG
								printf("error, each port should be object with \"num\" and \"protocol\"\n");
#endif
							}


						}
					} else {
#ifdef CONFIGURATION_DEBUG
						printf("error, ports entry should be an array\n");
#endif

					}
				} else if (!strcmp(CLIENT_HEADER,entries[i].name)) {
					if (entries[i].value->type == json_string) {

					} else {

#ifdef CONFIGURATION_DEBUG
						printf("error, client name should be a string value\n");
#endif
					}

				}
			}
		}
		
		json_value_free(parsed_conf);
		qsort(config->listen_ports, config->nports, sizeof(PORT_T), &comparePorts);

	}

}


