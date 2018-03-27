#include "funciones.c"

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
	
	struct ListaColaReady colaReady;
	colaReady->insertar(3,3,0);
	colaReady->insertar(4,1,1);
	colaReady->insertar(8,2,2);
	colaReady->insertar(1,4,3);
	colaReady->insertar(6,1,4);
	
	printf("Length: %d \n",colaReady->largo());
	
	colaReady->imprimir();
	
	colaReady->extraerPID();
	colaReady->imprimir();
	
	colaReady->extraerBurst();
	colaReady->imprimir();
	
	colaReady->extraerPrioridad();
	colaReady->imprimir();
	
	
	
	return(0);
}