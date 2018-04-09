#include "estructuras.c"
#include <time.h>

void * consola(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	bool * seguir = listas->seguir;
	int input;
	while (*seguir){
		printf("\nComandos:\n\n -> 'd' para detener la ejecucion.\n\n -> 'l' para ver cola del Ready. \n");
		input = getchar();
		//*input = putchar(tolower(*input));		
		if (input == 'd'){
			*seguir = false;
			printf("\nEJECUCION DETENIDA\n");
		} else {
			if (input == 'l'){
				imprimirLista(colaReady);
			} else {
				printf("\nCOMANDO INCORRECTO\n");
			}
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
			
			switch (listas->tipoAlgoritmo){ 
				case 0:
					burst = extraerFIFO(colaReady, hist);
					break;
				case 1:
					burst = extraerSJF(colaReady, hist);
					break;
				case 2:
					burst = extraerHPF(colaReady, hist);
					break;
				case 3:
					burst = extraerRR(listas->quantum,colaReady, hist);
					break;
			}
			
			sleep(burst);
		} else{
			sleep(1);
			cont ++;
		}
		
	}
	hist->tiempoFinal = time(NULL) - hist->tiempoInicial;
	
	return 0;
}

int  main(int argc, char * argv []) {  // Anadi (no tengo idioma espanol xd) el argV.
	if (argc == 1){
		printf("\nFalta indicar el tipo de algoritmo a utilizar (FIFO, SJF, HPF o RR), si es RR, indicar el quantum. Por ejemplo:\n./programa RR 4\n");
		return 0;
	}
	int i = 0;
	while( argv[1][i] ) {
		argv[1][i] = putchar(tolower(argv[1][i]));
		i++;
	}
	
	char inFifo[4] = "fifo";
	char inSjf[3] = "sjf";
	char inHpf[3] = "hpf";
	char inRr[2] = "rr";
	int algoritmo;
	int quantum = 0;
	if (cmpString(argv[1],inFifo)){
		algoritmo = 0;
	} else {
		if (cmpString(argv[1],inSjf)){
			algoritmo = 1;
		} else {
			if (cmpString(argv[1],inHpf)){
				algoritmo = 2;
			} else {
				if (cmpString(argv[1],inRr)){
					algoritmo = 3;
					quantum = *argv[2];
					if (argc < 3){
						printf("\nFalta el parametro del quantum, intente nuevamente.\n");
						return 0;
					}
				} else {
					printf("\nComando incorrecto. Intente nuevamente\n");
						return 0;
				}
			}
		}
	
	}
	
	printf("\nCont Arg: %d\n\nAlgoritmo: %d\n",argc,algoritmo);  // Hasta aqui acaba la parte de leer los parametros de entrada, se agrego funcion a estructuras.c
	
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
	listas->quantum = quantum;		// Se agrega a la lista de parametros el quantum y el tipo de algoritmo.
	listas->tipoAlgoritmo = algoritmo;
	
	pthread_t jobSch;
	pthread_t cpuSch;
	pthread_t console;
	pthread_create(&console, 0, consola,(void *)listas);
	pthread_create(&jobSch, 0, jobScheduling,(void *)listas);
	pthread_create(&cpuSch, 0, cpuScheduling,(void *)listas);
	pthread_join(jobSch, NULL);
	pthread_join(cpuSch, NULL);
	pthread_join(console, NULL);
	
	reporteCPU(hist);
	
	return(0);
}