#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT    1024
#define MAXLINE 1024 

int main(int argc, char *argv[]) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in     servaddr; 
  
    // Création de la socket, INTERNET et UDP
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    	if (sockfd < 0) {
			fprintf(stderr,"Impossible d'ouvrir la socket\n");
			return 1;
		}
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Configurer tous les paraèetres sur le serveur
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len; 
    //Message à envoyer
    sendto(sockfd, (const char *)argv[1], strlen(argv[1]),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	//Message reçu        
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len); 
    buffer[n] = '\0'; 
    printf("Message: %s\n", buffer); 
    // On ferme la socket
    close(sockfd); 
    return 0; 
} 