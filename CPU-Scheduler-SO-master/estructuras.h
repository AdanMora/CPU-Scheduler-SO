#ifndef ESTRUCTURAS
#define ESTRUCTURAS

typedef struct NodoJobScheduler NodoJobScheduler;
typedef struct ListaColaReady ListaColaReady;

typedef struct NodoJobScheduler{
    int burst;
    int burst_temp;
	int prioridad;
    int PID;
    int tLlegada;
    int tSalida;
    bool escogido;

    NodoJobScheduler * siguiente;
	NodoJobScheduler * anterior;
	
} NodoJobScheduler;

typedef struct ListaColaReady{
	NodoJobScheduler * primerNodo;
	int contPID;
    int tiempoInicial;
    int tiempoFinal;
	
} ListaColaReady;


typedef struct ListasArgs{
	ListaColaReady * colaReady;
	ListaColaReady * hist;
    bool * seguir;
    int tipoAlgoritmo;
    int quantum;
	
} ListasArgs;
 
void insertar(int, int, int, int, ListaColaReady *);

void insertarNodo(NodoJobScheduler*, ListaColaReady *);

int largoLista(ListaColaReady *);

int extraerFIFO(ListaColaReady *, ListaColaReady *);

int extraerSJF(ListaColaReady *, ListaColaReady *);

int extraerHPF(ListaColaReady *, ListaColaReady *);

int extraerRR(int, ListaColaReady *, ListaColaReady *);

void resetearEscogido(ListaColaReady *);

bool isListaEmpty(ListaColaReady *);

void imprimirLista(ListaColaReady *);

NodoJobScheduler* borrarEnPosicion(int, ListaColaReady *);

void reporteCPU(ListaColaReady *);

bool cmpString(char [], char []);
 
#endif /* ESTRUCTURAS */
