#include <sys/time.h>
#include <time.h>
#include "sys/select.h"
#include "unistd.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "unistd.h"
#include <string.h>
#include <errno.h>

char userlogon[] = "userlogon:";
char userlogoff[] = "userlogoff:";
char addfriend[] = "addfriend:";
char removefriend[] = "removefriend:";
char ctrlprotocol[] = "\x50\x43\x81\x76";

#define MAX_ENTITIES_NUM 20
#define MAX_FRIENDS_NUM MAX_ENTITIES_NUM

#define STATUS_ONLINE 1
#define STATUS_OFFLINE 2

struct iot_entity {
	int idx;
	int fd;
	int status;
	unsigned char id[6];
	int friends[MAX_FRIENDS_NUM];
};

struct iot_entity entities[MAX_ENTITIES_NUM];

void iot_entities_init()
{
	int i;

	for(i=0;i<MAX_ENTITIES_NUM;i++) {
		entities[i].idx = i;
		entities[i].fd = -1;
		entities[i].status = STATUS_OFFLINE;
	}
	return;
}

void dump_mem(char *addr, int len)
{
	int i;
	for(i=0;i<len;i++) {
		printf("%x ", addr[i]&0xff);
	}
	printf("\n");
}

void dump_entities()
{
	int i, j;
	struct timeval time;

	gettimeofday(&time, NULL);

	printf("idx\tsockfd\tstatus\t\tuser id\t\t\tfriends\n");
	for (i=0;i<MAX_ENTITIES_NUM;i++) {
		if(memcmp(entities[i].id, "\x00\x00\x00\x00\x00\x00", 6) == 0)
			continue; 
		printf("%d\t%d\t", entities[i].idx, entities[i].fd);

		if (entities[i].status == STATUS_ONLINE) {
			printf("ONLINE\t\t");
		} else if (entities[i].status == STATUS_OFFLINE) {
			printf("OFFLINE\t\t");
		} else {
			printf("ERROR\t\t");
		} 

		printf("%02x:%02x:%02x:%02x:%02x:%02x\t",
			 entities[i].id[0],entities[i].id[1],entities[i].id[2],entities[i].id[3],entities[i].id[4],entities[i].id[5]);

		for (j=0;j<MAX_FRIENDS_NUM;j++) {
			printf("%d ", entities[i].friends[j]);
		}
		printf("\t%s", ctime(&time.tv_sec));
	} 
}

void handle_data(int fd, char *buf, int len)
{
	int i, j, k, n;
	char *p, tmp_buf[1024] = {0};

	p = buf;
	while(p < buf+len) {
		if (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r' || *p == 0x0) {
			p++;
		} else if (strncmp(p, userlogon,strlen(userlogon))==0) {
			/* check if this user had already registered. */
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				if(memcmp(entities[i].id, p+strlen(userlogon), 6) == 0) {
					entities[i].fd = fd;
					entities[i].status = STATUS_ONLINE;
					/* send the on/off line status to it's friends. */	
					for(k=0;k<MAX_FRIENDS_NUM;k++) {
						if(entities[i].friends[k] == 1 && entities[k].status == STATUS_ONLINE) {
							n = write(entities[k].fd, p, strlen(userlogon)+6);
							if (n<0) {
								perror("write");
								entities[k].status = STATUS_OFFLINE;
							}
						}	
					}
					break;	
				}
			}
			if (i == MAX_ENTITIES_NUM) {
				/* not registered. */	
				for (i=0;i<MAX_ENTITIES_NUM;i++) {
					if (memcmp(entities[i].id, "\x00\x00\x00\x00\x00\x00", 6) == 0) {
						entities[i].fd = fd;	
						entities[i].status = STATUS_ONLINE;
						memcpy(entities[i].id, p+strlen(userlogon), 6);
						break;
					}
				}
				if (i == MAX_ENTITIES_NUM ) {
					printf("too much entities.\n");
				} 
			}
			/* send the on/off line status of it's friends. */
			if (i<MAX_ENTITIES_NUM) {
				for(k=0;k<MAX_FRIENDS_NUM;k++) {
					if(entities[i].friends[k] == 1 && entities[k].status == STATUS_ONLINE) {
						char *msg;
						bzero(tmp_buf, 1024);
						msg = (entities[k].status==STATUS_ONLINE)?userlogon:userlogoff;
						strcpy(tmp_buf, msg);
						memcpy(tmp_buf+strlen(msg), entities[k].id, 6);
						n = write(entities[i].fd, tmp_buf, strlen(msg)+6);
						if (n<0)
							perror("write");
					}
				}
			}
			p=p+strlen(userlogon)+6;
			dump_entities();
		} else if (strncmp(p, userlogoff, strlen(userlogoff))==0) {
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				if (memcmp(entities[i].id, p+strlen(userlogoff), 6) == 0) {
					entities[i].status = STATUS_OFFLINE;
					for (k=0;k<MAX_FRIENDS_NUM;k++) {
						if((entities[i].friends[k] == 1) && (entities[k].status == STATUS_ONLINE)) {
							n = write(entities[k].fd, p, strlen(userlogoff)+6);
							if (n<0) {
								perror("write");
								entities[k].status = STATUS_OFFLINE;
							}
						}
					}
					break;
				}
			}
			if (i == MAX_ENTITIES_NUM) {
				printf("WARNING: logoff no such user. %02x:%02x:%02x:%02x:%02x:%02x\n",
					p[strlen(userlogoff)], p[strlen(userlogoff)+1],p[strlen(userlogoff)+2],
					p[strlen(userlogoff)+3],p[strlen(userlogoff)+4],p[strlen(userlogoff)+5]);
			} 
			p=p+strlen(userlogoff)+6;
			dump_entities();
		} else if (strncmp(p, addfriend, strlen(addfriend))==0) {
			char *str = NULL;
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				/* check if the user exist. */
				if (memcmp(p+strlen(addfriend), entities[i].id, 6)==0) {
					for (j=0;j<MAX_ENTITIES_NUM;j++) {
						if(entities[j].fd == fd) {
							entities[j].friends[i] = 1;
							entities[i].friends[j] = 1;
							if (entities[i].status == STATUS_ONLINE) {
								/* send status to new friend. */
								if (entities[j].status == STATUS_ONLINE) {
									str = userlogon;
								} else {
									str = userlogoff;
								}
								strcpy(tmp_buf, str);
								memcpy(tmp_buf+strlen(str), entities[j].id, 6);
								n = write(entities[i].fd, tmp_buf, strlen(str)+6);
								if (n<0)
									perror("write");
							}
							if (entities[j].status == STATUS_ONLINE) {
								/* send status to new friend. */
								if (entities[i].status == STATUS_ONLINE) {
									str = userlogon;
								} else {
									str = userlogoff;
								}
								strcpy(tmp_buf, str);
								memcpy(tmp_buf+strlen(str), entities[i].id, 6);
								n = write(entities[j].fd, tmp_buf, strlen(str)+6);
								if (n<0)
									perror("write");
							}
							break;
						}
					}
					if (j==MAX_ENTITIES_NUM)
						printf("WARNING user bind to this fd not found.\n");
					break;
				} 
			}
			if (i==MAX_ENTITIES_NUM)
				printf("no such user.\n");
			p=p+strlen(addfriend)+6;
			dump_entities();
		} else if (memcmp(p, removefriend, strlen(removefriend))==0) { 
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				/* check if the user exist. */
				if (memcmp(p+strlen(removefriend), entities[i].id, 6) == 0) {
					for (j=0;j<MAX_ENTITIES_NUM;j++) {
						if(entities[j].fd == fd) {
							entities[j].friends[i] = 0;
							entities[i].friends[j] = 0;
							printf("i=%d, j=%d.\n", i, j);
							break;
						}
					}
					if (j==MAX_ENTITIES_NUM)
						printf("WARNING user bind to this fd not found.\n");
					break;
				} 
			}
			if (i==MAX_ENTITIES_NUM) {
				n=write(fd, "no such user", 12);
				if (n<0) {
					perror("write");
				}
				printf("WARNING no such user.\n");
			}
			p=p+strlen(removefriend)+6;
			dump_entities();
		} else if (memcmp(p, ctrlprotocol, strlen(ctrlprotocol))==0) { /* just foward the data */ 
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				if(memcmp(entities[i].id, p+strlen(ctrlprotocol)+6, 6) == 0) {
					for(k=0;k<MAX_FRIENDS_NUM;k++) {
						if(entities[i].friends[k] == 1 && 
							memcmp(entities[k].id, p+strlen(ctrlprotocol), 6) == 0 &&
							entities[i].status == STATUS_ONLINE) {
							printf("TXFD %d----", entities[k].fd);
							dump_mem(p, buf+len-p);
							usleep(300000);
							n = write(entities[k].fd, p, buf+len-p);
							if (n<0) {
								perror("write");
								entities[k].status = STATUS_OFFLINE;
							}
						}
					}
					break;
				}
			}
			if (i== MAX_ENTITIES_NUM) 
				printf("WARNING src id not found\n");
			break;
		} else {
			printf("unknown data: %d\n", len);
			break;
		} 
	}
	return;
}
int main(int argc, char **argv)
{
	int listenfd, sockfd, connfd, maxfd,ret, maxi,i,j,k,n;
	char buf[1024] = {0};
	socklen_t cli_addr_len;	
	struct sockaddr_in cli_addr;
	struct sockaddr_in srv_addr;
	fd_set rset, allset;
	int clients[MAX_ENTITIES_NUM];

	iot_entities_init();

	bzero(&cli_addr, sizeof(cli_addr));

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(7681);
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd<0) {
		perror("socket");
	}
	
	int on=1;
	if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))<0)
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}
	
	ret = bind(listenfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	if (ret < 0) {
		perror("bind");
	}

	ret = listen(listenfd, MAX_ENTITIES_NUM);
	if (ret<0) {
		perror("listen");
	}

	maxfd = listenfd;
	for (i=0;i<MAX_ENTITIES_NUM;i++) 
		clients[i] = -1;

	FD_ZERO(&allset);
	FD_SET(maxfd, &allset);
	maxi = -1;
	
	while(1) {
		rset = allset;
		ret = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(FD_ISSET(listenfd, &rset)) {
			cli_addr_len = sizeof(cli_addr);
			connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
			printf("ip %s:%d connected with fd %d\n", inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port),connfd);
			for (i=0;i<MAX_ENTITIES_NUM;i++) {
				if (clients[i] < 0) {
					clients[i] = connfd;
					break;
				}
			}
			if (i==MAX_ENTITIES_NUM) {
				printf("not enough buffer for new connection\n");
				return 0;
			}
			FD_SET(connfd, &allset);
			if(connfd>maxfd)
				maxfd=connfd;
			if(i>maxi)
				maxi=i;
		}
		for (i=0;i<=maxi;i++) {
			if((sockfd = clients[i])<0)
				continue;
			if(FD_ISSET(sockfd, &rset)){
				if ((n=read(sockfd, buf, 1024)) == 0) {
					close(sockfd);
					FD_CLR(sockfd, &allset);
					clients[i] = -1;
					for (j=0;j<MAX_ENTITIES_NUM;j++) {
						if (entities[j].fd == sockfd) { 
							entities[j].fd = -1;
							entities[j].status = STATUS_OFFLINE;
							for(k=0;k<MAX_FRIENDS_NUM;k++) {
								if(entities[j].friends[k] == 1 && entities[k].status == STATUS_ONLINE)
								{
									strcpy(buf, userlogoff);
									memcpy(buf+strlen(userlogoff),entities[i].id,6);
									n=write(entities[k].fd,buf,strlen(userlogoff)+6);
									if (n<0)
										perror("write");
								}
							}
						}
					}
					dump_entities();
				} else if (n > 0) {

					printf("RXFD %d----", sockfd);
					dump_mem(buf, n);
					handle_data(sockfd, buf, n);
					bzero(buf, 1024);	
				} else {
					printf("fd %d read:%s\n", sockfd, strerror(errno));
				}
			}
		}
	}
}
