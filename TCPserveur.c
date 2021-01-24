#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "TCPserveur.h"
#define PORT 1024
#define pile_donnee(tas)  tas->debut->donnee


int main(){		
		// les id et le mot de passe du client
		strcpy(banque.id_client,"25595");
		strcpy(banque.id_compte,"25590");
		strcpy(banque.password,"9910");

		// les paramètres du serveur
     	int sockfd, newsockfd;
     	socklen_t lenclient;
     	char buffer[256]; // buffer pour client envoyer des donnees
     	struct sockaddr_in serv_addr, cli_addr; // l'address de socket serveur client
     	int n;

		// les paramètres de banque 
		Pile *tas; // Pile de tas
		char id_client[100];
		char id_compte[100];
		char password[100]; 
		int somme; // somme initialiser a 0
		char sendBuffer[256]; // buffer a envoyer le message au client
		int ind=0;  // indicateur qui compte 10 operations
		int operator; // numero de operation
  		char *nom; // string de tas
 		
		initialisation (tas);
		if ((tas = (Pile *) malloc (sizeof (Pile))) == NULL)
    			return -1;
 			 if ((nom = (char *) malloc (50 * sizeof (char))) == NULL)
    			return -1;

	    // Création de socket SOCK_STREAM se signifie TCP
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0) {
         	fprintf(stderr,"Impossible d'ouvrir la socket\n");
			return 1;
		}

		if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
			printf("echec de setsockopt(SO_REUSEADDR)");
			return 1;
		}	

		// Assigner les IP, PORT et on demande le port pour passe dans le parametre 
		// INADDR_ANY dit que la socket va être affectée à toutes les interfaces locales
     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	serv_addr.sin_family = AF_INET; // IPv4 
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(PORT);

		// Bind la socket avce l'address du serveur 
     	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         	fprintf(stderr,"Impossible de faire bind()\n");
			return 1;
		}

		// On écoute sur la socket,le 5 est le nombre max de connexions
     	listen(sockfd,5);
		
     	while (1){    
     		
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &lenclient);
     		if (newsockfd < 0) {
         		fprintf(stderr,"Impossible de faire accept()\n");
				return 1;
			}

     		bzero(buffer,256);
     		n = read(newsockfd,buffer,255);
     		if (n < 0) {
         		fprintf(stderr,"Impossible de faire read()\n");
				return 1;
			}
			
			
		// Acquittement les id et le mot de passe et choisir l'operation et la somme
		
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
					
				}else{
					if(ind<10){
					
					ind++;
					ajouterSomme(somme);
					sprintf(nom,"+%d",somme);
					sprintf(sendBuffer,"OK AJOUT\n");
					empiler(tas, nom);
					affiche(tas);
					printf("%s",asctime(timeinfo));	
							
					// Affiche le montant ajoute dans serveur
					
					}else{
							
							ajouterSomme(somme);
							sprintf(nom,"+%d",somme);
							sprintf(sendBuffer,"OK AJOUT\n");
							empiler(tas,nom);
							depiler(tas);
							affiche(tas);	
							printf("%s",asctime(timeinfo));	
											
					}
				}
				break;

				case 2:
				if(somme>banque.somme){
					sprintf(sendBuffer,"KO Depasser votre somme reste\n");
				 	printf("Depasser la somme reste\n");

				}else{
					if(ind<10){
						
							retraitSomme(somme);
							sprintf(nom,"-%d",somme);
							sprintf(sendBuffer,"OK RETRAIT\n");
							empiler(tas, nom);
							affiche(tas);
							printf("%s",asctime(timeinfo));		
							
					}else{
							
							retraitSomme(somme);
							sprintf(nom,"-%d",somme);
							sprintf(sendBuffer,"OK RETRAIT\n");
							empiler(tas, nom);
							depiler(tas);
							affiche(tas);
							printf("%s",asctime(timeinfo));	
							
					}
					
				}
				break;

				case 3:
				
				afficheCompte();
				sprintf(sendBuffer,"RES_SOLED:%d\ntime:%s\n",banque.somme,asctime(timeinfo));
				break;
				

				case 4:
				sprintf(sendBuffer,"RES_OPERATIONS\ntime:%s",asctime(timeinfo));
				AfficheOperation(newsockfd,tas);
				affiche(tas);
				
				break;
			}
					
		}else{
				sprintf(sendBuffer,"ID client ou ID compte ou mot de pasee est est incorrect\n");
		}

		n=write(newsockfd,sendBuffer,strlen(sendBuffer));	
     	close(newsockfd);
     	}
		// On ferme la socket
     	close(sockfd);
     	return 0; 
}
