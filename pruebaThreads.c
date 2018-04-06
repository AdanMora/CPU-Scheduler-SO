#include "estructuras.c"
#include <time.h>

void *hola(void *arg) {
	char *msg = "Hola";
	int i;
	for ( i = 0 ; i < 5 ; i++ ) {
		printf ("%s\n", msg);
		sleep (1) ;
	}
	return 0;
}

void *mundo(void *arg ) {
	char *msg = " mundo ";
	int i;
	for ( i = 0 ; i < 5 ; i++ ) {
		printf ("%s\n", msg);
		sleep (1) ;
	}
	return 0;
}

void * jobScheduling(void *cola){
	ListaColaReady * colaReady = cola;
	int i;
	for (i = 1; i < 5; i++){
		insertar(rand()%10+1, (rand()%5)+1, colaReady->contPID, time(NULL) - colaReady->tiempoInicial, colaReady);
		printf("\nProceso insertado a la cola\n");
		colaReady->contPID ++;
		sleep(1);
	}
	return 0;
}

void * cpuScheduling(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	ListaColaReady * hist = listas->hist;
	int burst;
	int cont = 0;
	while (cont != 10){
		if (!(isListaEmpty(colaReady))){
			printf("\nREADY\n");
			imprimirLista(colaReady);
			//burst = extraerRR(4, colaReady, hist);
			burst = extraerSJF(colaReady, hist);
			sleep(burst);
			printf("\nHISTORIAL\n");
			imprimirLista(hist);
		} else{
			sleep(1);
			cont ++;
		}
		
	}
	hist->tiempoFinal = time(NULL) - hist->tiempoInicial;
	
	return 0;
}

int  main() {
	srand(time(NULL));
	
	ListasArgs * listas = malloc(sizeof(ListasArgs));
	
	ListaColaReady * ptr_colaReady = malloc(sizeof(ListaColaReady));
    ptr_colaReady->primerNodo = 0;
	ptr_colaReady->tiempoInicial = time(NULL);
	ptr_colaReady->contPID = 0;
	
	ListaColaReady * hist = malloc(sizeof(ListaColaReady));
    hist->primerNodo = 0;
	hist->tiempoInicial = time(NULL);
	
	listas->colaReady = ptr_colaReady;
	listas->hist = hist;
	
	pthread_t jobSch;
	pthread_t cpuSch;
	pthread_create(&jobSch, 0, jobScheduling,(void *)ptr_colaReady);
	pthread_create(&cpuSch, 0, cpuScheduling,(void *)listas);
	pthread_join(jobSch, NULL);
	pthread_join(cpuSch, NULL);
	
	reporteCPU(hist);
	
	return(0);
}