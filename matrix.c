#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

struct matrix *matrix_init(unsigned int nlines, unsigned int ncols){
	struct matrix *mat;
	mat = (struct matrix *) malloc(sizeof(struct matrix));
	if(mat==NULL){
		return NULL;
	}
	mat->lines=NULL;
	mat->nlines=nlines;
	mat->ncols=ncols;
	return mat;
}

void matrix_free(struct matrix *matrix){
	struct line* l=matrix->lines;
	while(l!=NULL){ //parcours de toutes les lignes
		struct line* cur=l;
		struct elem *e=cur->elems;
		while(e!=NULL){ //parcours de tous les elements de la ligne courante
			struct elem *current=e;
			e=e->next;
			free(current);
		}
		free(e);
		l=l->next;
		free(cur);
	}
	free(l);
	free(matrix);
	
}

int matrix_set(struct matrix *matrix, unsigned int i, unsigned int j, int val){
	int countlines=0; //compteur de lignes permettant de savoir sur quelle ligne on se situe
	struct line* l=matrix->lines;
	struct line* prev; //permet de garder une trace de la precedente ligne parcourue
	while(l!=NULL){ //parcours de toutes les lignes existantes
		if((l->i)==i){ //nous sommes sur la bonne ligne
			int countelem=0; //compteur d'elements permettant de savoir sur quel element on se situe
			struct elem* el=l->elems;
			struct elem* bef; //permet de garder une trace du precedent element parcouru
			while(el!=NULL){ //parcours de tous les elements existants
				if(el->j==j){ //nous sommes sur le bon element
					if(val==0){ // si la nouvelle valeur vaut 0, cela revient a supprimer l'element courant de la matrice
						if(countlines==0 && (l->next==NULL) && countelem==0 && (el->next==NULL)){ // cas où il n'y a qu'un seul élément
							matrix->lines=NULL;
							free(el);
							free(l); 
							return 0;
						}else if(countlines==0 && countelem==0 && (el->next==NULL)){ // cas où il y a plus d'une ligne mais qu'un seul élément ET que nous sommes sur la première ligne
							matrix->lines=l->next;
							free(l);
							return 0;
						}else if(countelem==0 && (el->next==NULL)){ // cas où il y a plus d'une ligne mais qu'un élément sur la ligne courante
							prev->next=l->next;
							free(l);
							return 0;
						}else if(countelem==0){ //cas où il y a plus d'une ligne et plusieurs éléments mais que nous sommes sur le premier element
							l->elems=el->next;
							free(el);
							return 0;
						}else{ // cas normal où il faut supprimer un élément d'une ligne qui en contient plusieurs
							bef->next=el->next;
							free(el);
							return 0;
						}	
					}else{ //attribution de la nouvelle valeur
						el->value=val;
						return 0;
					}
				}else if((el->j)<j){ // nous n'avons pas encore atteint l'element souhaite, nous avancons donc dans les elements
					bef=el;
					el=el->next;
					++countelem;
				}else{ //l'element n'existe pas encore, du coup...
					if(val==0){ //...si la valeur de l'element a ajouter est 0, rien a changer
						return 0;
					}else{ //...sinon, creation du nouvel element
						struct elem *newelem = (struct elem *) malloc(sizeof(struct elem));
						if(newelem==NULL){
							return -1;
						}
						newelem->j=j;
						newelem->value=val;
						newelem->next=el;
						l->elems=newelem;
						if(countelem==0){ //cas où on rajoute un élément au tout début
							l->elems=newelem;
						}else{ //cas general
							bef->next=newelem;
						}
						return 0;
					}
				}
			}//fin du while(el!=NULL)

			// A ce stade, nous sommes sur une ligne qui a ete creee et l'element est a ajouter en derniere position vu que el==NULL
			if(val==0){ //la valeur de l'element a ajouter vaut 0, rien a changer
				return 0;
			}else{
				struct elem *newelem = (struct elem *) malloc(sizeof(struct elem));
				if(newelem==NULL){
					return -1;
				}
				newelem->j=j;
				newelem->value=val;
				newelem->next=el;
				bef->next=newelem;
				return 0;
			}					

		}else if((l->i)<i){ // nous n'avons pas encore atteint la ligne souhaitee, nous avancons donc dans les lignes
			prev=l;
			l=l->next;
			++countlines;
		}else{ //la ligne n'existe pas encore, du coup...
			if(val==0){ //...si la valeur de l'element a ajouter vaut 0, rien a changer
				return 0;
			}else{ //...sinon, creation de la nouvelle ligne et du nouvel element
				struct line *newline = (struct line *) malloc(sizeof(struct line));
				if(newline==NULL){
	                        	return -1;
		               	}
				struct elem *e = (struct elem *) malloc(sizeof(struct elem));
				if(e==NULL){
	                	        return -1;
	               		}
	            		if(countlines==0){ // cas où rajoute une ligne au tout début
					e->next=NULL;
					e->j=j;
					e->value=val;
					newline->i=i;
					newline->elems=e;
					newline->next=l;
					matrix->lines=newline;
					return 0;
				}else{ //cas general
					e->next=NULL;
					e->j=j;
					e->value=val;
					newline->i=i;
					newline->elems=e;
					newline->next=l;
					prev->next=newline;
					return 0;
				}
			}
		}
	} //fin while(l!=NULL)


	// A ce stade l==NULL, ce qui implique qu'il faut soit ajouter le tout premier element, soit ajouter un element à la derniere ligne qui ne contient pas encore d'element
	if(val==0){ //la valeur de l'element a ajouter vaut 0, rien a changer
		return 0;
	}else{
		struct line *l=(struct line*) malloc(sizeof(struct line));
		if(l==NULL){
                	return -1;
	  	}
		struct elem *e=(struct elem*) malloc(sizeof(struct elem));
		if(e==NULL){
        	        return -1;
	        }
		l->next=NULL;
		l->i=i;
		e->next=NULL;
		e->j=j;
		e->value=val;
		l->elems=e;
		if(countlines==0){ //cas du premier element a ajouter dans la matrice
			matrix->lines=l;
		}else{ //element a rajouter sur la derniere ligne
			prev->next=l;
		}			
		return 0;
	}
	
	return -1; //nous ne devrions normalement jamais atteindre ce stade
}

int matrix_get(const struct matrix *matrix, unsigned int i, unsigned int j){
	struct line *l;
	l=matrix->lines;
	while(l!=NULL){
		if(l->i==i){
			struct elem *e;
			e=l->elems;
			while(e!=NULL){
				if(e->j==j){
					return e->value;
				}else{
					e=e->next;
				}
			}
			return 0; // l'element n'est pas present sur la ligne et vaut donc 0
		}else{
			l=l->next;
		}
	}
	return 0; //la ligne ne contient que des 0
}

struct matrix *matrix_add(const struct matrix *m1, const struct matrix *m2){
	struct matrix* mat;
	int i;
	int j;
	int k;
	int l;
	int val;
	int length;
	int width;
	if(((m1->nlines)==(m2->nlines)) && ((m1->ncols)==(m2->ncols))){ //les 2 matrices sont de memes dimensions
		mat=matrix_init(m1->nlines,m1->ncols);
		for(i=0;i<(mat->nlines);i++){
			for(j=0;j<(mat->ncols);j++){
				val=matrix_get(m1,i,j)+matrix_get(m2,i,j);
				matrix_set(mat,i,j,val);
			}
		}
		return mat;
	}else{ //les 2 matrices sont de dimensions differentes
		length=(m1->nlines)+(m2->nlines);
		width=(m1->ncols)+(m2->ncols);
		mat=matrix_init(length,width);
		
		for(k=0;k<(m1->nlines);k++){
			for(l=0;l<(m1->ncols);l++){
				val=matrix_get(m1,k,l);
				matrix_set(mat,k,l,val);
			}
		}
		
		for(i=0;i<(m2->nlines);i++){
			for(j=0;j<(m2->ncols);j++){
				int val=matrix_get(m2,i,j);
				matrix_set(mat,i+(m1->nlines),j+(m1->ncols),val);
			}
		}
		return mat;
	}
}

struct matrix *matrix_transpose(const struct matrix *matrix){
	struct matrix* mat;
	int i;
	int j;
	int val;
	mat=matrix_init(matrix->ncols,matrix->nlines);
	for(i=0;i<(mat->nlines);i++){
			for(j=0;j<(mat->ncols);j++){
				val=matrix_get(matrix,j,i); //l'element (j,i) de matrix...
				matrix_set(mat,i,j,val); //...devient l'element (i,j) de la nouvelle matrice
			}
	}
	return mat;
}

struct matrix *matrix_convert(const int **array, unsigned int nlines,unsigned int ncols){
	struct matrix* mat;
	int i;
	int j;
	int val;
	mat=matrix_init(nlines,ncols);
	for(i=0;i<nlines;i++){
			for(j=0;j<ncols;j++){
				val=array[i][j]; //recuperation de la valeur dans le tableau...
				matrix_set(mat,i,j,val); //...et assignation a l'indice equivalent dans la matrice
			}
	}
	return mat;
}

// MAIN A SUPPRIMER ***********************************************************************************************
/*
int main (int argc, char *argv[]){
	struct matrix *m1 = matrix_init(3,3);
        struct matrix *m2 = matrix_init(3,3);

        matrix_set(m1,0,0,4);
        matrix_set(m1,0,2,8);
        matrix_set(m1,1,0,7);
        matrix_set(m1,2,2,1);

    
        matrix_set(m2,0,0,8);
        matrix_set(m2,2,1,3);
        matrix_set(m2,1,1,4);
        matrix_set(m2,2,2,4);

        struct matrix *m3 = matrix_add(m1,m2);

	printf(" %d \n",matrix_get(m3,0,0));
	printf(" %d \n",matrix_get(m3,0,2));
	printf(" %d \n",matrix_get(m3,1,0));
	printf(" %d \n",matrix_get(m3,2,2));
	printf(" %d \n",matrix_get(m3,2,1));
	printf(" %d \n",matrix_get(m3,1,1));

	matrix_free(m1);
        matrix_free(m2);
        matrix_free(m3);
}*/



