#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x); 
}

struct bien
{
    int ID;
    double prix;
    char region[50] ;
    char type[30];
    char caractristique[300];
};

typedef struct noeud{
    struct bien val;
    struct noeud *suiv;
}noeud;

typedef noeud* liste;

struct idcom
{
	int id;
	char commentaire[300];
}idcom;
bool in (int x , int tab[100], int taille){
	bool test=false;
	int i=0;
	while ((test==false)&&(i<taille)){
		if (tab[i]==x){
			test=true;
		}
		i++;
	}
	return test;
}
void ajouter(liste *biens,struct  bien b, int tidf[100],int taille ){ //cette procédure permet de prendre en paramétre une pile et un bien, et d'ajouter ce bien à la téte de cette pile (empiler) et aussi de l'ajouter dans la base de donné (fichier)
	liste l1;
	FILE *f;
	f=fopen("base de donnés","a+");
    l1=(noeud*)malloc(sizeof(noeud));
    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    printf("donner l'identifiant du bien:");
	scanf("%d",&b.ID);	
	while((in(b.ID,tidf,taille))){
    	setColor(4);
		printf("identifiant deja existant, donner un autre identifiant:");
		setColor(7);
		scanf("%d",&b.ID);	 
	}
    bool test=false ;
    while(test==false){ //un controle de saisie pour le type
        printf("donner le type du bien:");
        scanf("%s",b.type);
        if (strcmp(b.type, "terrain") == 0 || strcmp(b.type, "appartement") == 0 || strcmp(b.type, "villa") == 0) {
            test = true;
        }
        else {
            setColor(4);
            printf("                         Type invalide. Veuillez entrer 'terrain', 'appartement' ou 'villa'.\n");
            setColor(7);
        }
    }
    printf("donner la region du bien:");
    scanf("%s",b.region);
    printf("donner le prix du bien :");
    scanf("%lf",&b.prix);
    printf("donner les caracteristiques du bien :");
    getchar();
    fgets(b.caractristique,300,stdin);
    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    l1->suiv=*biens;
    l1->val=b;
    *biens=l1;
    fprintf(f,"%d %s %s %lf %s \n",b.ID,b.type,b.region,b.prix,b.caractristique);
	fclose(f);

}

void afficher_Liste(liste biens){  //mettre en evidance la notion  FILO (car la liste des biens se comporte comme une pile )
    liste Q=biens;
    if(Q==NULL){
        printf(" la liste est vide \n");
    }
    while(Q){
        printf("\n-----------------------------------------------------------------------------------------------------------\n");
        //setColor(2);
        printf("\n                           l'identifiant du bien :%d",Q->val.ID);
        printf("\n                           le prix du bien :%.2lf",Q->val.prix);
        printf("\n                           le type du bien : %s",Q->val.type);
        printf("\n                           la region  du bien : %s",Q->val.region);
        printf("\n                           les caracteristiques  du bien : %s",Q->val.caractristique);
        setColor(7);
        printf("\n------------------------------------------------------------------------------------------------------------\n");
        Q=Q->suiv;
    }
}
void suppression(liste *biens, int x) {  //permet de supprimer le bien dont lidentifiant est x de la liste chainé et du fichier:
    liste aux =*biens;                             //fonctionnement:
    struct bien b;                                         //la liste est déja remplie alors on a supprimer l'élement de la liste puis on a ouvrie le fichier
    liste p;                                               // en mode W pour écraser les donnés existante et ajouter de nouveaux les élement restant dans la liste
    FILE *f;
    if (aux != NULL && aux->val.ID == x) {
        *biens = aux->suiv;
        free(aux);
        printf("Element supprime avec succes.\n");
        f = fopen("base de donnés", "w");
        if (f == NULL) {
            printf("Erreur d'ouverture du fichier.\n");
            return;
        }
        liste q = *biens;
        while (q) {
            b = q->val;
            fprintf(f, "%d %s %s %lf %s\n", b.ID, b.type, b.region, b.prix, b.caractristique);
            q = q->suiv;
        }
        fclose(f);
        return;
    }
    while (aux != NULL && aux->suiv != NULL && aux->suiv->val.ID != x) {
        aux = aux->suiv;
    }
    if (aux == NULL || aux->suiv == NULL) {
        setColor(4);
        printf("                                             L'element n'existe pas.");
        setColor(7);
        return;
    }
    p = aux->suiv;
    aux->suiv = p->suiv;
    free(p);
    setColor(4);
    printf("                                         l'element est supprime avec succes.");
    setColor(7);
    liste q;
    q=*biens;
    f=fopen("base de donnés","w");
	while (q){
		b=q->val;
		fprintf(f, "%d %s %s %lf %s \n", b.ID, b.type, b.region, b.prix, b.caractristique);
		q=q->suiv;
	}
    fclose(f);
}
void modifier(liste biens, int x) {
    liste aux = biens;
    FILE *f;
    int bt=0;
    while (aux != NULL && bt!=1) {
        if (aux->val.ID == x) {
            int choix;
            setColor(3);
            printf("                                         Bien trouve! Que voulez-vous modifier ?");
            setColor(7);
            printf("\n   +---------------------+             +---------------------------+               +-------------------------+            \n ");
            printf("\n   | 1-Modifier le type  |             |   2-Modifier la région    |               |  3-Modifier le prix     |             \n ");
            printf("\n   +---------------------+             +---------------------------+               +-------------------------+             \n ");
            printf("\n                                       +----------------------------+                                        \n ");
            printf("\n                                       |        Votre choix:        |                                        \n ");
            printf("\n                                       +----------------------------+                                        \n");
            gotoxy(62,27);
            scanf("%d", &choix);
            switch (choix) {
                case 1:
                    gotoxy(0,30);
                    setColor(3);
                    printf("                                     Entrez le nouveau type du bien: ");
                    setColor(7);
                    getchar();
                    fgets(aux->val.type, sizeof(aux->val.type), stdin);
                    setColor(4);
                    printf("                                      Le type a ete modifie avec succes!\n");
                    setColor(7);
                    break;
                case 2:
                    gotoxy(0,30);
                    setColor(3);
                    printf("                                     Entrez la nouvelle region du bien: ");
                    setColor(7);
                    getchar();
                    fgets(aux->val.region, sizeof(aux->val.region), stdin);
                    setColor(4);
                    printf("                                     La region a ete modifiee avec succes!\n");
                    setColor(7);
                    break;
                case 3:
                    gotoxy(0,30);
                    setColor(3);
                    printf("                                      Entrez le nouveau prix du bien: ");
                    setColor(7);
                    scanf("%lf", &aux->val.prix);
                    setColor(4);
                    printf("                                      Le prix a ete modifie avec succes!\n");
                    setColor(7);
                    break;
                default:
                    setColor(4);
                    printf("                                            Option invalide.\n");
                    setColor(7);
                    break;
            }
            bt=1;
        }
        aux = aux->suiv;
    }
	if(bt==0){
    setColor(4);
    printf("                                            Aucun bien trouve avec l'ID %d.\n", x);
    setColor(7);
    }
    liste q;
    q=biens;
    f=fopen("base de donnés","w");
	while (q){
		struct bien b=q->val;
		fprintf(f, "%d %s %s %lf %s \n", b.ID, b.type, b.region, b.prix, b.caractristique);
		q=q->suiv;
	}
    fclose(f);


}

int filtrage_reg(liste L, const char* reg, struct bien T[100]) { // cette fonction consiste a remplir un tableau par des bien a partir d'une liste chainé de la region passer en parametre et de retourner sa taille
    liste p = L;
    int i = 0;

    while (p != NULL) {
        if (strcmp(p->val.region, reg) == 0) {
            T[i] = p->val;
            i++;
        }
        p = p->suiv;
    }

    return i;
}
int filtrage_type(struct bien t[100],int n ,const char* type,struct bien A[100]){ // cette fonction consiste a remplir un tableau par des bien a partir d'un autre tableau du type passer en parametre et de retourner sa taille
	int i;
	int j=0;
	for(i=0;i<n;i++){
		if (strcmp(t[i].type, type) == 0) {
            A[j] = t[i];
            j++;             //taille de A pour pouvoir l'utiliser dans filtrage prix
        }
	}
	return j;
}
int filtrage_prix(struct bien t[100],int n ,struct bien A[100]){ //mème principe mais juste dans un intervalle de prix donner
	int i;
	int j=0;
	float min,max ;
	gotoxy(0,30);
	setColor(3);
	printf("                                 donnez la valeur minimal et maximal de votre budjet:\n");
	setColor(7);
	do{
        setColor(2);
		printf("                                            valeur minimal:");
		scanf("%f",&min);
		printf("                                            valeur maximal:");
		scanf("%f",&max);
        setColor(7);
	}while (min>max);
	for (i = 0; i < n ;i++){
		if (t[i].prix<=max && t[i].prix>=min){
			A[j]=t[i];
			j++;
		}
	}
	return j;

}
void afficher_tableau(struct bien T[100], int N) {
    if (N > 100) {
        printf("Erreur : le nombre d'elements ne doit pas depasser 100.\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        printf("\n-----------------------------------------------------------------------------------------------------------\n");
        printf("                           L'identifiant du bien : %d\n", T[i].ID);
        printf("                           Le prix du bien : %.2lf\n", T[i].prix);
        printf("                           Le type du bien : %s\n", T[i].type);
        printf("                           La région du bien : %s\n", T[i].region);
        printf("                           Les caracteristiques du bien : %s\n", T[i].caractristique);
        printf("\n-----------------------------------------------------------------------------------------------------------\n");
    }
}

int main(){
	int choix,code,C,idf,R,t,p,n,i,com ;
	struct idcom IDCOM;
	FILE* f = fopen("base de donnés", "r");
	FILE *fcom;
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;               // Erreur d'ouverture
    }

    struct bien b; 
    liste biens = NULL; 
// Lire chaque ligne du fichier jusqu'à la fin
    while (fscanf(f, "%d %s %s %lf %[^\n]\n", &b.ID, b.type, b.region, &b.prix, b.caractristique) == 5) {
        liste l1 = (noeud*)malloc(sizeof(noeud));
        if (l1 == NULL) {
            printf("Erreur d'allocation memoire\n");
            fclose(f);
            return 1;            // Erreur d'allocation
        }
        l1->val = b;
        l1->suiv = biens;  
        biens = l1;      
    }

    fclose(f); 
    int TIDF[100];      // tableau d'identifiants des biens qui existent déja dans la base de donneé (fichier)
    liste aux=biens;
    int h=0;            //taille de ce tableau 
	while (aux!=NULL){
		TIDF[h]=aux->val.ID;
		h++;
		aux=aux->suiv;
	}
	setColor(3);
	printf("                    bienvenue dans l'application. Veuillez indiquer votre profil pour continuer:\n");
	setColor(7);
        printf("\n                                                                                                                    \n ");
        printf("\n                                  +--------------+             +--------------+                                  \n ");
        printf("\n                                  |  1-employe   |             |   2-Client   |                                  \n ");
        printf("\n                                  +--------------+             +--------------+                                  \n ");
        printf("\n                                         +----------------------------+                                        \n ");
        printf("\n                                         |        Votre choix:        |                                        \n ");
        printf("\n                                         +----------------------------+                                        \n");
    do{
        gotoxy(64,13);
        scanf("%d",&choix);
    }while (choix!=1 && choix!=2);
	if (choix==1){
		
		gotoxy(0,15);
		setColor(3);
		printf("\n                                              donnez votre code pin:                                             \n");
		setColor(7);
		gotoxy(69,16);
		scanf("%d",&code);
		while (code !=1962){
			gotoxy(0,15);
			setColor(4);
			printf("\n                                              code invalide!!,donnez votre code pin:                                              \n");
			setColor(7);
			gotoxy(85,16);
			scanf("%d",&code);
		}
		gotoxy(0,18);
		setColor(3);
		printf("                            bienvenue dans l'espace employe,que voullez vous faire ?                      \n");
		setColor(7);
        printf("\n                                                                                                                           \n ");
        printf("\n   +-------------------+     +---------------------+        +---------------------+     +-----------------------------+    \n ");
        printf("\n   |1- ajouter un bien |     |2-modifier un bien   |        |3-supprimer un bien  |     |4-affichage des commentaires |    \n ");
        printf("\n   +-------------------+     +---------------------+        +---------------------+     +-----------------------------+    \n ");
        printf("\n                                         +----------------------------+                                                         \n ");
        printf("\n                                         |        Votre choix:        |                                                         \n ");
        printf("\n                                         +----------------------------+                                                         \n");
        do{
			gotoxy(63,25);
			scanf("%d",&C);
		}while (C!=1 && C!=2 && C!=3 && C!=4 );
		if (C==1) {
            gotoxy(0,30);
		    setColor(3);
			printf("                                     combien de bien voulez vous ajoutez ?:");
            gotoxy(76,30);
            setColor(7);
			scanf("%d",&n);
			for (i=0;i<n;i++){
				ajouter(&biens,b,TIDF,h);
			}
		}
		else if(C==2) {
            gotoxy(0,30);
		    setColor(3);
			printf("                                     donnez l'identifiant du bien a modifier:");
			gotoxy(80,30);
            setColor(7);
            scanf("%d",&idf);
			modifier(biens, idf);
		}
		else if (C==3){
		    gotoxy(0,30);
		    setColor(3);
			printf("                                     donnez l'identifiant du bien a supprimer :");
			gotoxy(80,30);
            setColor(7);
			scanf("%d",&idf);
			suppression(&biens, idf );
		}
		else {
			fcom=fopen("fichier des commentaires.txt","r");
			while (fscanf(fcom, "%d ", &IDCOM.id) == 1) { // Lire l'ID
        		// Lire tout le commentaire après l'ID
        		fgets(IDCOM.commentaire, sizeof(IDCOM.commentaire), fcom);

        		// Affichage de l'ID et du commentaire
       			 printf("Le commentaire pour l'identifiant %d est : %s", IDCOM.id, IDCOM.commentaire);
    			}
    		fclose(fcom);
		}
	}

	else{
        gotoxy(0,16);
		setColor(3);
		printf("                              bienvenue dans l'espace client. voici tous les biens:\n");
        setColor(7);
		afficher_Liste(biens);
		setColor(3);
		printf ("                                 voulez-vous filtrer votre recherche par region ?\n");
		setColor(7);
        printf("\n                                  +--------------+             +--------------+                                  \n ");
        printf("\n                                  |    1-oui     |             |    2-non     |                                  \n ");
        printf("\n                                  +--------------+             +--------------+                                  \n ");
        printf("\n                                         +----------------------------+                                        \n ");
        printf("\n                                         |        Votre choix:        |                                        \n ");
        printf("\n                                         +----------------------------+                                        \n");
        do{
        gotoxy(64,27);
		scanf("%d",&R);
		}while (R!=1 && R!=2);
		if (R==1){
			struct bien RE[100];
			char reg[15];
			gotoxy(0,30);
		    setColor(3);
			printf ("                          donnez la region dans laquelle vous voulez trouver des biens:\n");
			setColor(7);
			gotoxy(88,29);
			getchar();
			fgets(reg, sizeof(reg),stdin);
			reg[strcspn(reg, "\n")] = '\0';
			int nre=filtrage_reg(biens,reg, RE);
			if (nre > 0) {
            afficher_tableau(RE, nre);
        		}
			else {
                setColor(4);
            	printf("                                     Aucun bien trouve pour la region %s.\n", reg);
            	setColor(3);
            	printf("                                           MERCI POUR VOTRE VISITE \n?");
            	setColor(7);
            	return 0;
            	}
            setColor(3);
			printf ("                          Est-ce que vous voulez filtrer par type de bien (terrain, villa, appartement) ?\n");
			setColor(7);
            printf("\n                                  +--------------+             +--------------+                                  \n ");
            printf("\n                                  |    1-oui     |             |    2-non     |                                  \n ");
            printf("\n                                  +--------------+             +--------------+                                  \n ");
            printf("\n                                         +----------------------------+                                        \n ");
            printf("\n                                         |        Votre choix:        |                                        \n ");
            printf("\n                                         +----------------------------+                                        \n");
            do{
            gotoxy(64,27);
			scanf("%d",&t);
			}while (t!=1 && t!=2);
			if (t==1){
				struct bien T[100];
				char type[13];
				bool test=false ;
    			while(test==false){
    			    gotoxy(0,30);
		            setColor(3);
        			printf("                                                  donner le type:");
                    setColor(7);
                    gotoxy(66,94);
        			scanf("%s",type);
        			if (strcmp(type, "terrain") == 0 || strcmp(type, "appartement") == 0 || strcmp(type, "villa") == 0) {
            			test = true;
       				}
        			else {
                        setColor(4);
            			printf("                         Type invalide. Veuillez entrer 'terrain', 'appartement' ou 'villa'.\n");
                        setColor(7);
        			}
				}
				int nt=filtrage_type(RE,nre,type,T);
				afficher_tableau(T,nt);
				setColor(3);
				printf ("                                       Est-ce que vous voulez filtrer par prix ?");
				setColor(7);

                    printf("\n                                  +--------------+             +--------------+                                  \n ");
                    printf("\n                                  |    1-oui     |             |    2-non     |                                  \n ");
                    printf("\n                                  +--------------+             +--------------+                                  \n ");
                    printf("\n                                         +----------------------------+                                        \n ");
                    printf("\n                                         |        Votre choix:        |                                        \n ");
                    printf("\n                                         +----------------------------+                                        \n");
                do{
                    gotoxy(64,27);
					scanf("%d",&p);
				}while (p!=1 && p!=2);
				if (p==1){
					struct bien P[100];
					int np=filtrage_prix(T,nt,P);
					if (np > 0) {
            			afficher_tableau(P, np);
        			}
					else {
                        setColor(4);
            			printf("                                       Aucun bien trouve dans l'intervalle");
                        setColor(7);

        			}
                }
				else{
                    gotoxy(0,30);
                    setColor(2);
					printf("                               tous les biens ont ete execute precedemment\n ");
                    setColor(7);
				}
            }
			else{
                gotoxy(0,30);
                setColor(2);
				printf("                                   tous les biens ont ete execute precedemment \n");
                setColor(7);
			}
		}
		else {
            gotoxy(0,30);
            setColor(2);
			printf ("                                      tous les biens ont ete execute precedemment \n ");
            setColor(7);
		}
    setColor(3);
	printf (" \n                                        voulez vous ajoutez des commentaires?");
	setColor(7);
	printf("\n                                  +--------------+             +--------------+                                  \n ");
    printf("\n                                  |    1-oui     |             |    2-non     |                                  \n ");
    printf("\n                                  +--------------+             +--------------+                                  \n ");
    printf("\n                                         +----------------------------+                                        \n ");
    printf("\n                                         |        Votre choix:        |                                        \n ");
    printf("\n                                         +----------------------------+                                        \n");
    do{
        gotoxy(64,27);
        scanf("%d",&com);
    }while (com!=1 && com!=2);
	if (com==1){
        gotoxy(0,30);
        setColor(3);
		printf("                                  donnez l'identifiant du bien que voulez vous commentez:");
        setColor(7);
        gotoxy(91,94);
		scanf("%d",&IDCOM.id);
		setColor(3);
		printf("                                             donnez votre commentaire: ");
		setColor(7);
		gotoxy(72,95);
		getchar();
    	fgets(IDCOM.commentaire,300,stdin);
    	fcom=fopen("fichier des commentaires.txt","a+");
    	fprintf(fcom,"%d  %s\n",IDCOM.id,IDCOM.commentaire);
    	fclose(fcom);
    	gotoxy(0,32);
	    setColor(3);
		printf ("                                            MERCI POUR VOTRE VISITE ");
		setColor(7);
	}else {
	    gotoxy(0,32);
	    setColor(3);
		printf ("                                            MERCI POUR VOTRE VISITE ");
		setColor(7);
	}
	}
	return 0;
}
