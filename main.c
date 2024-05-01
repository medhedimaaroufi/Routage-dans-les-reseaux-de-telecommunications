#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define Nbs 10
#define infinie 9999

typedef struct sommet{
    int noeud;
    float poids;
    struct sommet *suiv;
}sommet;

typedef struct sommet* liste;

void affiche_matrice(int M[Nbs][Nbs],int nb_sommets){
    int i,j;
    printf("*/ Matrice : \n\t");
    if (M!=NULL && nb_sommets<Nbs) {
        for(i=0;i<nb_sommets;i++){
            for(j=0;j<nb_sommets;j++){
                printf("%2d ",M[i][j]);
            }
            printf("\n\t");
        }
    }
}

void affiche_P(int P[Nbs] , int nb_sommets){
    int i ;
    for(i=0 ; i< nb_sommets;i++){
        if (P[i]){
            printf("les Predecesseurs %d sont : ",i+1);
            printf("%d\n",P[i]);
        }
        else
            printf("La sommet %d est une source.\n",i+1);
    }
}


void affiche_Lambda(int Lambda[Nbs],int nb_sommets){
    int i;
    for(i=0;i<nb_sommets;i++){
        printf("Le plus court chemin jusqu'a %d est de poids ",i+1);
        printf("%d\n",Lambda[i]);
    }
    printf("\n");
}

void affiche_chemin(int chemin[Nbs],int nb_sommets){
    int i;
    printf("le chemin le plus court est : ");
    for(i=nb_sommets-1;i>=0;i--){
        printf("%d ",chemin[i]);
    }
    printf("\n");
}

void MDist(char *file ,int Mdist[Nbs][Nbs], int *nb){
    int x,y;
    int p;
    FILE *f=fopen(file,"r");
    fscanf(f,"%d %d",nb, &x);
    while (!feof(f)){
        fscanf(f,"%d %d %d",&x,&y,&p);
        Mdist[x-1][y-1]=p;
    }
    fclose(f);
}

void init_Lambda(int Lambda[Nbs],int nb_sommets){
    int i ;
    Lambda[0]=0;
    for(i=1; i < nb_sommets ; i++) Lambda[i]=infinie;
}

void init_P(int P[Nbs] , int nb_sommets){
    int i ;
    for(i=0 ; i< nb_sommets ; i++) P[i] = 0 ;
}

////////////Algorithme de recherche des plus courts chemins Bellman Ford.
void Ford_bellmann(int M[Nbs][Nbs],int Lambda[Nbs], int P[Nbs] ,int nb_sommets){
    int i,j,stop=0;
    init_Lambda(Lambda,nb_sommets);
    init_P(P,nb_sommets);

    while (!stop){
        stop=1;
        for (i = 0; i < nb_sommets; i++ ){
            for (j = 0; j < nb_sommets; ++j) {
                if ( M[i][j] && Lambda[i] + M[i][j] < Lambda[j]){
                    stop=0;
                    Lambda[j] = Lambda[i] + M[i][j];
                    P[j] = i+1 ;
                }
            }
        }
    }
}

int Plus_Court_Chemins(int dest,int chemin[Nbs], int P[Nbs], int nb_sommets){
    int i=dest-1,j=0;
    while (P[i]){
        chemin[j]=i+1;
        i=P[i]-1;
        j++;
    }
    chemin[j]=i+1;
    return j+1;
}

int main() {
    printf("-------Algorithme Routage dans les reseaux de telecommunications-------");
    int M[Nbs][Nbs]={{0}};
    int Lambda[Nbs];
    int P[Nbs];
    int chemin[Nbs]={0};
    int nb_sommets,c;
    MDist("graph.txt",M,&nb_sommets);
    printf("\n");
    affiche_matrice(M,nb_sommets);
    printf("\n");

    Ford_bellmann(M,Lambda,P,nb_sommets);
    printf("\n");
    affiche_Lambda(Lambda,nb_sommets);
    printf("\n");
    affiche_P(P,nb_sommets);
    printf("\n");
    c=Plus_Court_Chemins(1,chemin,P,nb_sommets);
    affiche_chemin(chemin,c);
    return 0;
}
