/* Ann Mai   X0470198
 * ICS 53
 * Lab 5 - Client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

 #define MAXLINE 256
 
 int open_clientfd(char * hostname, int port);
 
 int main(int argc, char * argv[])
 {
	 int clientfd, port;
	 char * host, input[MAXLINE];
	 char sendmsg[MAXLINE], recvmsg[MAXLINE];
	 host = argv[1];
	 port = atoi(argv[2]);
	 clientfd = open_clientfd(host, port);
	 
	 while(1)
	 {
		 printf("> ");
		 fflush(stdout);
		 
		 fgets(input, MAXLINE, stdin);
		 char * msg = strtok(input, " \r\n");
		 if(strcmp(msg,"+++") == 0)
		 {
			 close(clientfd);
			 exit(0);
		 }
		 
		 sendmsg[0] = strlen(msg);
		 sendmsg[1] = '\0';
		 strcat(sendmsg,msg);
		 write(clientfd, sendmsg, strlen(sendmsg));
		 read(clientfd, recvmsg, MAXLINE);
		 recvmsg[recvmsg[0]+1] = '\0';
		 int len = strlen(recvmsg);
		 int i;
		 for(i = 1; i < len; ++i)
			 printf("%c", recvmsg[i]);
		 
		 printf("\n");
		 memset(recvmsg,0,MAXLINE);
		 memset(sendmsg,0,MAXLINE);
	 }

 }
 
 int open_clientfd(char * hostname, int port)
 {
	 int clientfd;
	 struct hostent * hp;
	 struct sockaddr_in servaddr;
	 
	 if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		 return -1;
	 
	 if((hp = gethostbyname(hostname)) == NULL)
		 return -2;
	 
	 bzero((char *) &servaddr, sizeof(servaddr));
	 
	 servaddr.sin_family = AF_INET;
	 bcopy((char *) hp->h_addr_list[0], (char *) &servaddr.sin_addr.s_addr, hp->h_length);
	 
	 servaddr.sin_port = htons(port);
	 
	 if(connect(clientfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		 return -1;
	 
	 return clientfd;
 }
 