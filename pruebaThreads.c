#include "estructuras.c"
#include <time.h>

void * consola(void *arg ) {
	bool * seguir = arg;
	int input;
	printf("\nSeguir: %d\n",*seguir);
	while (*seguir){
		printf("\nIngrese el comando 'd' para detener la ejecucion: \n");
		input = getchar();
		//*input = putchar(tolower(*input));		
		if (input == 'd'){
			*seguir = false;
			printf("\nEJECUCION DETENIDA\n");
		}
	}
}

void * jobScheduling(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	bool * seguir = listas->seguir;
	
	int i;
	for (i = 1; i < 5; i++){
		insertar(rand()%10+1, (rand()%5)+1, colaReady->contPID, time(NULL) - colaReady->tiempoInicial, colaReady);
		printf("\nProceso insertado a la cola\n");
		colaReady->contPID ++;
		sleep(2);
	}
	
	return 0;
}

void * cpuScheduling(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	ListaColaReady * hist = listas->hist;
	bool * seguir = listas->seguir;
	int burst;
	int cont = 0;
	while (*seguir){
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
	bool flag = true;
	bool * seguir = &flag;
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
	listas->seguir = seguir;
	
	pthread_t jobSch;
	pthread_t cpuSch;
	pthread_t console;
	pthread_create(&console, 0, consola,(void *)seguir);
	pthread_create(&jobSch, 0, jobScheduling,(void *)listas);
	pthread_create(&cpuSch, 0, cpuScheduling,(void *)listas);
	pthread_join(jobSch, NULL);
	pthread_join(cpuSch, NULL);
	pthread_join(console, NULL);
	
	reporteCPU(hist);
	
	return(0);
}