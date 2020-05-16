/* Ann Mai   X0470198
 * ICS 53
 * Lab 5 - Server Address Book
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <arpa/inet.h>

#define MAXLINE 256
#define LISTENQ 0

static volatile int keepRunning = 1;

int open_listenfd(int port);
char * getEmail(char * msg, int len);

int main(int argc, char * argv[])
{
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent * hp;
	char * haddrp;
	char sendmsg[MAXLINE], recvmsg[MAXLINE];
	unsigned short client_port;
	
	port = atoi(argv[1]);
	listenfd = open_listenfd(port);
	printf("Address server started\n");
	
	while(1)
	{
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
		hp = gethostbyaddr((const char *) &clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		client_port = ntohs(clientaddr.sin_port);
		
		while((read(connfd, recvmsg, MAXLINE)) > 0)
		{
			int len = strlen(recvmsg);
			int i;
			for(i = 1; i < len; ++i)
				printf("%c", recvmsg[i]);
		 
			printf("\n");
			char * email = getEmail(recvmsg, len);
			sendmsg[0] = strlen(email);
			sendmsg[1] = '\0';
			strcat(sendmsg,email);
			write(connfd, sendmsg, strlen(sendmsg));
			memset(recvmsg,0,MAXLINE);
			memset(sendmsg,0,MAXLINE);
		}
	}
	
	return 0;
}

int open_listenfd(int port)
{
	int listenfd, optval=1;
	struct sockaddr_in servaddr;
	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0)
		return -1;
	
	bzero((char *) &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons((unsigned short) port);
	
	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		return -1;
	
	if(listen(listenfd, LISTENQ) < 0)
		return -1;
	
	return listenfd;
}
 
 char * getEmail(char * msg, int len)
 {
	 char * email = malloc(len); 
	 int i;
	 for(i = 0; i < len; ++i)
		 email[i] = msg[i+1];
	 
	 email[len] = '\0';
	 
	 if(strcmp(email,"joe@cnn.com") == 0)
		 return "Joe Smith";
	 
	 if(strcmp(email,"jane@slashdot.org") == 0)
		 return "Jane Smith";
	 
	 if(strcmp(email,"harris@ics.uci.edu") == 0)
		 return "Ian G. Harris";
	 
	 return "unknown";
 }