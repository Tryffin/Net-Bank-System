#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "UDPserveur.h"
#define PORT    1024 
#define MAXLINE 1024 

int main() { 
	// les id et le mot de passe du client
	strcpy(banque.id_client,"25595");
	strcpy(banque.id_compte,"25590");
	strcpy(banque.password,"9910");

	// les paramètres du serveur
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in serv_addr, cliaddr; 

	// les paramètres de banque 
	char id_client[100];
	char id_compte[100];
	char password[100]; 
	char tab_somme[256]; // le tableau des operations
	char tab_somme2[256]; // le tableau de chaque ajout et retrait
	char tab_somme3[66]; // les 10 dernieres operation
	int somme=0; // somme initialiser a 0
	char sendBuffer[256]; // buffer a envoyer le message au client
	int ind=0;  // indicateur qui compte 10 operations
	int ind2=6; // indicateur pour la RES_OPERATION
	int operator; // numero de operation
      
    // Création de socket SOCK_DGRAM se signifie UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr,"Impossible d'ouvrir le socket\n");
		return 1;
	}

      
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Assigner les IP, PORT et on demande le port pour passe dans le parametre 
	// INADDR_ANY dit que le socket va être affectée à toutes les interfaces locales
    serv_addr.sin_family    = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(PORT); 
      
    // Bind le socket avce l'address du serveur 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr,"Impossible de faire bind()\n");
		return 1;
	}
      
    int len, n; 
    len = sizeof(cliaddr);  // len est valeur/resusltat 
	while(1){
		// message recu par le client
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len); 
    	buffer[n] = '\0'; 

		sscanf(buffer,"%s %s %s %d %d",id_client,id_compte,password,&operator,&somme);

		// Afficher les id et le mot de passe sur serveur
		printf("ID_client:%s ID_compte:%s pwd:%s \n",id_client,id_compte,password);

		// Si la vérification passe
		if(trouverCompte(id_client,id_compte,password)==1){
				time_t t;
				struct tm *timeinfo;
				time(&t);
				timeinfo = localtime(&t);
			switch(operator){

				case 1:
				if(somme<0.0){
					sprintf(sendBuffer,"KO veuillez saisir le montant positif\n");
					sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				}else{
					if(ind<10){
					ind++;
					ajouterSomme(somme);
					sprintf(tab_somme2,"+%d ",somme);
					sprintf(sendBuffer,"OK AJOUT\n");
					sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
					strcat(tab_somme,tab_somme2);
					printf("%s\n",tab_somme2); // Afficher le montant ajoute dans serveur
					printf("%s\n",tab_somme); // Afficher les operations recemment
					}else{
							
							ajouterSomme(somme);
							sprintf(tab_somme2,"+%d ",somme);
							sprintf(sendBuffer,"OK AJOUT\n");
							sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
							strncpy(tab_somme3,tab_somme+ind2,66);
							strncat(tab_somme,tab_somme2,6);
							printf("%s\n",tab_somme2);
							printf("%s\n",tab_somme3); // Afficher les 10 dernieres operations
							ind2+=6;
							
					}
				}
				break;

				case 2:
				if(somme>banque.somme){
					sprintf(sendBuffer,"KO Dépasser votre somme reste\n");
					sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
				 	printf("Dépasser la somme reste\n");

				}else{
					if(ind<10){
					ind++;
					retraitSomme(somme);
					sprintf(tab_somme2,"-%d ",somme);
					sprintf(sendBuffer,"OK RETRAIT\n");
					strcat(tab_somme,tab_somme2);
					printf("%s\n",tab_somme2); // Affiche le montant retrait dans serveur
					printf("%s\n",tab_somme); // Afficher les operations recemment
					}else{
							
							retraitSomme(somme);
							sprintf(tab_somme2,"-%d ",somme);
							sprintf(sendBuffer,"OK RETRAIT\n");
							sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
							strncpy(tab_somme3,tab_somme+ind2,66);
							printf("%s\n",tab_somme);
							strncat(tab_somme,tab_somme2,6);
							printf("%s\n",tab_somme2);
							printf("%s\n",tab_somme3); // Afficher les 10 dernieres operations
							ind2+=6;
					}
					
				}
				break;

				case 3:
				afficheCompte();
				sprintf(sendBuffer,"RES_SOLED %d\ntime:%s",banque.somme,asctime(timeinfo));
				sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
				break;

				case 4:
				if(ind<10){
					sprintf(sendBuffer,"RES_OPERATIONS %s\n",tab_somme);
					sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
				}else{
					sprintf(sendBuffer,"RES_OPERATIONS %s\n",tab_somme3);
					sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
				}
				
				
				break;
			}
					
		}else{
				sprintf(sendBuffer,"ID client ou ID compte ou mot de pasee est incorrect\n");
				sendto(sockfd, (const char *)sendBuffer, strlen(sendBuffer),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
		}

    }
    return 0; 
} 