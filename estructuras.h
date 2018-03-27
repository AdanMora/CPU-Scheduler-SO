#ifndef ESTRUCTURAS
#define ESTRUCTURAS

typedef struct NodoJobScheduler NodoJobScheduler;
typedef struct ListaColaReady ListaColaReady;

typedef struct NodoJobScheduler{
    int burst;
	int prioridad;
    int PID;

    NodoJobScheduler * siguiente;
	NodoJobScheduler * anterior;
	
} NodoJobScheduler;

typedef struct ListaColaReady{
	NodoJobScheduler * primerNodo;
	
} ListaColaReady;
 
void insertar(int, int, int, ListaColaReady *);

int largoLista(ListaColaReady *);

NodoJobScheduler* extraerPID(ListaColaReady *);

NodoJobScheduler* extraerBurst(ListaColaReady *);

NodoJobScheduler* extraerPrioridad(ListaColaReady *);

bool isListaEmpty(ListaColaReady *);

void imprimirLista(ListaColaReady *);

NodoJobScheduler* borrarEnPosicion(int, ListaColaReady *);
 
#endif /* ESTRUCTURAS */