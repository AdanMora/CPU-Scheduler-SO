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

int  main() {
	/*pthread_t  h1;
	pthread_t  h2;
	pthread_create(&h1, 0 , hola,0);
	pthread_create(&h2, 0 , mundo,0);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	printf ("Fin\n");*/
	
	ListaColaReady * ptr_colaReady = malloc(sizeof(ListaColaReady));   
    ptr_colaReady->primerNodo = 0;

	
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
	
	
	return(0);
}