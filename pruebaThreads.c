#include "estructuras.c"

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

void * jobScheduling(void *arg){
	ListaColaReady * colaReady = arg;
	int i;
	for (i = 1; i < 7; i++){
		sleep(2);
		insertar(10-i,i,i,colaReady);
		printf("\nProceso insertado a la cola\n");
	}
	return 0;
}

void * cpuScheduling(void *arg){
	ListaColaReady * colaReady = arg;
	int cont = 0;
	while (cont != 20){
		if (!(isListaEmpty(colaReady))){
			imprimirLista(colaReady);
			NodoJobScheduler * proc = extraerBurst(colaReady);
			printf("\nSe extrajo el Proceso PID: %d, BURST: %d, PRIORIDAD: %d \n", proc->PID, proc->burst, proc->prioridad);
			sleep(proc->burst);
		} else{
			sleep(1);
			cont ++;
		}
		
	}
	
	return 0;
}

int  main() {
	ListaColaReady * ptr_colaReady = malloc(sizeof(ListaColaReady));   
    ptr_colaReady->primerNodo = 0;
	
	pthread_t  jobSch;
	pthread_t  cpuSch;
	pthread_create(&jobSch, 0 , jobScheduling,(void *)ptr_colaReady);
	pthread_create(&cpuSch, 0 , cpuScheduling,(void *)ptr_colaReady);
	pthread_join(jobSch, NULL);
	pthread_join(cpuSch, NULL);
	
	

	/*
	insertar(3,3,0,ptr_colaReady);
	insertar(4,1,1,ptr_colaReady);
	insertar(8,2,2,ptr_colaReady);
	insertar(1,4,3,ptr_colaReady);
	insertar(6,1,4,ptr_colaReady);
	
	printf("Length: %d \n",largoLista(ptr_colaReady));

	
	imprimirLista(ptr_colaReady);

	
	extraerPID(ptr_colaReady);
	imprimirLista(ptr_colaReady);
	
	extraerBurst(ptr_colaReady);
	imprimirLista(ptr_colaReady);
	
	extraerPrioridad(ptr_colaReady);
	imprimirLista(ptr_colaReady);
	*/
	
	
	return(0);
}