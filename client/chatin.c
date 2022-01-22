#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGBUFSIZE 256
#define PORT 12345

void SaveUIdToFile(pid_t _pid)
{		
	FILE *pID;
	pID = fopen("ip.txt" , "w+");
	if(pID == NULL)
	{
		printf("open file failed!\n");
		return;
	}
	fprintf(pID,"%d\n", _pid);	
	fclose(pID);
}

int main(int argc, char *argv[])
{
    pid_t pidIn;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    char* ip = argv[1];
    char* groupName = argv[2]; 
    int yes = 1;
    int fd, nbytes, addrlen;
    char msgbuf[MSGBUFSIZE];

    switch (argc)
    {
    case 1:
        strcpy(ip, argv[1]);
    case 2:
        strcpy(groupName, argv[2]);
        break;
    default:
        break;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket() failed");
        return 1;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
    {
       perror("Reusing ADDR failed");
       return 1;
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(PORT);
    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }
    pidIn = getpid();
    SaveUIdToFile(pidIn);
    /*use setsockopt() to request that the kernel join a multicast group*/
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(ip);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt failed");
        return 1;
    }
    while (1) 
    {
        addrlen = sizeof(addr);
        nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen);
        if (nbytes < 0) 
        {
            perror("recvfrom failed");
            return 1;
        }
        msgbuf[nbytes] = '\0';
        puts(msgbuf);
     }
}



