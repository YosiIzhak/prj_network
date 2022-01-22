#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define _GNU_SOURCE
#define IP "224.4.4.4"

int main(int argc, char *argv[])
{
	FILE* f1, *f2;
    int pid1, pid2;
    char command[100];
    sprintf(command, "gnome-terminal -q -- ./chatin.out %s %s",IP, "aaaa");               /* --geometry=20x20*/
    system(command);
	
	f1 = fopen("ip.txt","r");
	fscanf(f1,"%d",&pid1);
    fclose(f1);

    sprintf(command, "gnome-terminal -q -- ./chatout.out %s %s %s",IP, "abcd", "aaaa");
    system(command);
	f2 = fopen("ip2.txt","r");
    fscanf(f2,"%d",&pid2);
    fclose(f2);
    
}
