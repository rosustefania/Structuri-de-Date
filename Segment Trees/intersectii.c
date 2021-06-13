#include "intersectii.h"


// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    ListaIntervale2D *lista = malloc(sizeof(ListaIntervale2D));

    FILE* ptr = fopen(nume_fisier_intrare,"r"); 

    fscanf(ptr,"%d\n",&(lista->dimensiune));
    lista->intervale = malloc(lista->dimensiune * sizeof(Interval2D));

    for(int i = 0; i < lista->dimensiune; i++){

    	lista->intervale[i] = malloc(sizeof(Interval2D));
    	lista->intervale[i]->punct_stanga = malloc(sizeof(Punct2D));
    	lista->intervale[i]->punct_dreapta = malloc(sizeof(Punct2D));

    	fscanf(ptr,"%d %d %d %d\n",&(lista->intervale[i]->punct_stanga->x),
    					&(lista->intervale[i]->punct_stanga->y),
    					&(lista->intervale[i]->punct_dreapta->x),
    					&(lista->intervale[i]->punct_dreapta->y));
    }
    return lista;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}


int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
   	//calculez y-ul maxim;
   	int y_max = 0;
   	for( int i = 0; i < lista->dimensiune; i++){

   		if(lista->intervale[i]->punct_stanga->y > y_max){

   			y_max = lista->intervale[i]->punct_stanga->y;
   		}

   		if(lista->intervale[i]->punct_dreapta->y > y_max){

   			y_max = lista->intervale[i]->punct_dreapta->y;
   		}
		
   	}

   	//intializez arborele de intervale;
   	ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);
   	
   	//sortez lista in functie de coordonata x a capatului din stanga;
   	for( int i = 0; i < (lista->dimensiune - 1); i++){

		for(int j = 0; j < (lista->dimensiune - 1 - i); j++){

			int x1 = lista->intervale[j]->punct_stanga->x;
			int x2 = lista->intervale[j+1]->punct_stanga->x;

			if(x2 < x1){

				Interval2D *aux = lista->intervale[j];
				lista->intervale[j] = lista->intervale[j+1];
				lista->intervale[j+1] = aux;
			}
		}
	}

//afiseaza_lista_intervale(lista);

}


int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
	int nr=0;
	for( int i = 0; i < (lista->dimensiune - 1); i++){

		int x11 = lista->intervale[i]->punct_stanga->x;
    	int	y11 = lista->intervale[i]->punct_stanga->y;
    	int x12 = lista->intervale[i]->punct_dreapta->x;
    	int y12 = lista->intervale[i]->punct_dreapta->y;

		for(int j = i+1; j < lista->dimensiune; j++){

			int x21 = lista->intervale[j]->punct_stanga->x;
    		int	y21 = lista->intervale[j]->punct_stanga->y;
    		int x22 = lista->intervale[j]->punct_dreapta->x;
    		int y22 = lista->intervale[j]->punct_dreapta->y;

    		//daca segmentul i si segmentul j sunt verticale;
    		if(x11 == x12 && x12 == x21 && x21 == x22){
    			
    			if((y11 <= y21 && y21 <= y12) || (y11 <= y22 && y22 <= y12)){

    				nr++;
    			} 
    		}

    		//daca segmentul i si segmentul j sunt orizontale;
    		if(y11 == y12 && y12 == y21 && y21 == y22){
    			
    			if((x11 <= x21 && x21 <= x12) || (x11 <= x22 && x22 <= x12)){

    				nr++;
    			} 
    		}

    		//daca segmentul i este orizontal si segmentul j este vertical;
    		if( y11 == y12 && x21 == x22){

    			if( y21 <= y11 && y11 <= y22 && x11 <= x21 && x21 <= x12){

    			nr++;
    			
    			}
    		}

    		//daca segmentul i este vertical si segmentul j este orizontal;
    		if( x11 == x12 && y21 == y22){

    			if(x21 <= x11 && x11 <= x22 && y11 <= y21 && y21 <= y12){

    			nr++;
    			
    			}
    		}
    	}
	}
    return nr;
}
