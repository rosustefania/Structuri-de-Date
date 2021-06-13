#include "arbori_intervale.h"

Nod *creare_nod(int capat_stanga, int capat_dreapta){
    
    Nod *nod = malloc(sizeof(Nod));
    nod->info = 0;
    nod->interval = malloc(sizeof(Interval));
    nod->interval->capat_stanga = capat_stanga;
    nod->interval->capat_dreapta = capat_dreapta;

    if( capat_stanga < capat_dreapta ){

        int mijloc = ( capat_dreapta + capat_stanga ) / 2;
        nod->copil_stanga = creare_nod(capat_stanga, mijloc);
        nod->copil_dreapta = creare_nod(mijloc + 1 , capat_dreapta);
    }

    return nod;
    
}

int dimensiune_arbore(Nod *radacina) {

 if(radacina == NULL) return 0;
  
  return 1 + dimensiune_arbore(radacina->copil_stanga) 
  + dimensiune_arbore(radacina->copil_dreapta);

}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {

    ArboreDeIntervale *adi = malloc(sizeof(ArboreDeIntervale));
    adi->radacina = creare_nod(capat_stanga, capat_dreapta);
    adi->dimensiune = dimensiune_arbore(adi->radacina);
    adi->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    adi->f_actualizare = f_actualizare;
    adi->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;
   
    return adi;

}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {


    if(interval->capat_stanga <= nod->interval->capat_stanga &&
        nod->interval->capat_dreapta <= interval->capat_dreapta){

            arbore->f_actualizare(nod,v_actualizare);
    }

    else{

        int mijloc = (nod->interval->capat_stanga + 
                      nod->interval->capat_dreapta) / 2;

        if(interval->capat_stanga <= mijloc){

            actualizare_interval_pentru_nod(arbore,nod->copil_stanga,interval,v_actualizare);     
         }
        
        if(mijloc < interval->capat_dreapta){

            actualizare_interval_pentru_nod(arbore,nod->copil_dreapta,interval,v_actualizare);
        }

        arbore->f_actualizare(nod,v_actualizare);
    }

}

// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {

    
    if(interval->capat_stanga <= nod->interval->capat_stanga &&
        nod->interval->capat_dreapta <= interval->capat_dreapta){

            return nod->info;
    }

    else{

        int mijloc = (nod->interval->capat_stanga + 
                      nod->interval->capat_dreapta) / 2;

        if(interval->capat_stanga <= mijloc){

            nod->info = interogare_interval_pentru_nod(arbore,nod->copil_stanga,interval);     
         }
        
        if(mijloc < interval->capat_dreapta){

            nod->info = interogare_interval_pentru_nod(arbore,nod->copil_dreapta,interval);
        }

        return arbore->f_combinare_raspunsuri_copii(nod->copil_stanga->info, nod->copil_dreapta->info);
    }

}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    // TODO cred ca e destul de clar ce vrea sa faca functia asta
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
