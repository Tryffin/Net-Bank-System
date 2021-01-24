#ifndef TCPSERVEUR_H
#define TCPSERVEUR_H
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
// Structure de la pile
typedef struct ElementListe{
  char *donnee;
  struct ElementListe *suivant;
} Element;

typedef struct ListeRepere{
  Element *debut;
  int taille;
} Pile;

void initialisation (Pile * tas){
  tas->debut = NULL;
  tas->taille = 0;
}

/* empiler (ajouter) un élément dans la pile */
int empiler (Pile * tas, char *donnee){
  Element *nouveau_element;
  if ((nouveau_element = (Element *) malloc (sizeof (Element))) == NULL)
    return -1;
  if ((nouveau_element->donnee = (char *) malloc (50 * sizeof (char)))
      == NULL)
    return -1;
   strcpy (nouveau_element->donnee, donnee);
    nouveau_element->suivant = tas->debut;
    tas->debut = nouveau_element;
   
  tas->taille++;
}


/* depiler (supprimer la derniere élément de la pile */
int depiler (Pile * tas){
  Element *supp_element=tas->debut;
  int i;
  if (tas->taille == 0)
    return -1;

 for(i=0;i<tas->taille-1;++i){
   
    supp_element=supp_element->suivant;
    }
 
      free (supp_element->donnee);
      free (supp_element);
  tas->taille--;
  return 0;
}

/* affichage de la pile */
void affiche(Pile * tas){
    
    Element *courant;
    int i;
    courant = tas->debut;
    
  for(i=tas->taille;i>0;--i){
	

    printf("%s\n", courant->donnee);
    courant=courant->suivant;
  }
}
/* affichage de l'operation */
void AfficheOperation(int newsockfd,Pile * tas){
  Element *courant;
  int i;
  char* sendBuffer;
  courant = tas->debut;

    for(i=tas->taille;i>0;--i){

	  sendBuffer=courant->donnee;
  	write(newsockfd, sendBuffer, sizeof(sendBuffer)); 
	  courant=courant->suivant;
 	}
}


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
			printf("ID_Client:%s ID_Compte%s: pwd:%s somme %d\n", banque.id_client, banque.id_compte, banque.password,  banque.somme);
		
}
#endif