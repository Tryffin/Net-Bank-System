#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#define PORT 1024


int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
	char buffer[256];
    char receiveBuffer[256];

		// Création de la socket, INTERNET et TCP
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);

    	if (sockfd < 0) {
			fprintf(stderr,"Impossible d'ouvrir la socket\n");
			return 1;
		}

		// Configurer tous les parametres sur le serveur
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    	serv_addr.sin_port = htons(PORT);

    	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
			fprintf(stderr,"Impossible de faire connect()\n");
			return 1;
		}
		//Message à envoyer
		sprintf(buffer,"%s\n",argv[1]);
        n = write(sockfd,buffer,strlen(buffer));

		//Message reçu
		n = read(sockfd,receiveBuffer,256);
		printf("Message:%s\n",receiveBuffer);
		
		// On ferme la socket
    	close(sockfd);
    	return 0;
}
