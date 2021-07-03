#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2000

double *niz;
double srednja_vrednost;
int duzina;

void *racunaj_srednju_vrednost();

int main (int argc, char* argv[]) {

    omp_set_dynamic(0);
    omp_set_num_threads(2); // 2 niti cemo da imamo
    int broj_niti = 2;

    duzina = N;
    int i;

    // pocetna vrednost na 0
    srednja_vrednost = 0;

    niz = (double*) malloc (broj_niti * duzina * sizeof(double));

    #pragma omp parallel for schedule(static) shared(niz) if(duzina*broj_niti>1000)
	for (i = 0; i < duzina * broj_niti; i++){
		if(i%2 != 0) {
            niz[i] = 10;
        } else {
            niz[i] = 20;
        } // srednja vrednost treba da je 15, jer imamo isti broj 10 i 20 u nizu
	}

    #pragma omp parallel shared(niz,duzina) reduction(+:srednja_vrednost)
	{
	    racunaj_srednju_vrednost();
	}

    printf("Srednja vrednost niza  =  %f \n", srednja_vrednost);

    free(niz);

    return 0;
}

void* racunaj_srednju_vrednost() {
    int i, s,k; // pocetak i kraj s i k
    int id;
    id = omp_get_thread_num(); // svaka nit racuna svoj deo, zbog id, id kod prve 0, kod druge 1
    s = id * duzina;
    k  = s + duzina;
    double suma = 0;

    for(i=s; i<k; i++) {
        suma += niz[i];
    }

    srednja_vrednost += suma / (2*duzina);
}
