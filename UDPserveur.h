#ifndef UDPSERVEUR_H
#define UDPSERVEUR_H
#include <stdio.h>
#include <stdlib.h>

// Strcuture de la banque
struct _banque
{
	char id_client[100];
	char id_compte[100];
	char password[100];
	int somme;
}banque;

// Vérification du compte
int trouverCompte(char *id_client,char *id_compte,char *password){

	if(strcmp(banque.id_client,id_client)==0 && strcmp(banque.id_compte,id_compte)==0 && strcmp(banque.password,password)==0){

		return 1;
	}else{

		return -1;
	}
	
}

// AJOUT de la somme
void ajouterSomme(int somme){
				banque.somme+=somme;
}

// RETRAIT de la somme
void retraitSomme(int somme){
				banque.somme-=somme;
}

// Afficher le compte sur le serveur
void afficheCompte(){
			printf("ID_Client:%s ID_Compte%s: somme %d\n",banque.id_client,banque.id_compte,banque.somme);
		
}

#endif