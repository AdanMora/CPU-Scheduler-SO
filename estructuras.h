#ifndef ESTRUCTURAS
#define ESTRUCTURAS

typedef struct NodoJobScheduler NodoJobScheduler;
typedef struct ListaColaReady ListaColaReady;

typedef struct NodoJobScheduler{
    int burst;
	int prioridad;
    int PID;
    int tLlegada;
    int tSalida;

    NodoJobScheduler * siguiente;
	NodoJobScheduler * anterior;
	
} NodoJobScheduler;

typedef struct ListaColaReady{
	NodoJobScheduler * primerNodo;
    int tiempoInicial;
    int tiempoFinal;
	
} ListaColaReady;

typedef struct ListasArgs{
	ListaColaReady * colaReady;
	ListaColaReady * hist;
	
} ListasArgs;
 
void insertar(int, int, int, int, ListaColaReady *);

void insertarNodo(NodoJobScheduler*, ListaColaReady *);

int largoLista(ListaColaReady *);

NodoJobScheduler* extraerPID(ListaColaReady *);

NodoJobScheduler* extraerBurst(ListaColaReady *);

NodoJobScheduler* extraerPrioridad(ListaColaReady *);

bool isListaEmpty(ListaColaReady *);

void imprimirLista(ListaColaReady *);

NodoJobScheduler* borrarEnPosicion(int, ListaColaReady *);

void reporteCPU(ListaColaReady *);
 
#endif /* ESTRUCTURAS */