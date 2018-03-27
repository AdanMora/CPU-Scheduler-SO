#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include <stdbool.h>

#include "estructuras.h"

/*
 * Inserta un proceso en la Cola del Ready
 * Entradas: Burst, Prioridad y PID del proceso a insertar
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void insertar(int b,int p, int id, ListaColaReady * ready){
    NodoJobScheduler * nuevo = malloc(sizeof(NodoJobScheduler));
    nuevo->burst = b; nuevo->prioridad = p; nuevo->PID = id;
	
    if (isListaEmpty(ready)){
        ready->primerNodo = nuevo;
        nuevo->anterior = nuevo;
        nuevo->siguiente = nuevo;
    }else{
        nuevo->siguiente = ready->primerNodo;
        nuevo->anterior = ready->primerNodo->anterior;
        ready->primerNodo -> anterior -> siguiente = nuevo;
        ready->primerNodo -> anterior = nuevo;
    }
}

/*
 * Cuenta la cantidad de elementos.
 * Entradas: no tiene.
 * Salidas: la cantidad de procesos
 * Restricciones: No tiene
 */
int largoLista(ListaColaReady * ready){
    int cont = 1;
    if (isListaEmpty(ready)){
        printf("Vacio\n");
        return 0;
    }else{
        printf("No vacio\n");
        NodoJobScheduler * temp = ready->primerNodo->siguiente;
        while (temp != ready->primerNodo){
            printf("while\n");
            cont ++;
            temp = temp->siguiente;
        }
        return cont;
    }
}

/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
NodoJobScheduler* extraerPID(ListaColaReady * ready){
    NodoJobScheduler * borrado;
    if (isListaEmpty(ready)){
        return 0;
    }else{
		borrado = ready->primerNodo;
        if (ready->primerNodo->siguiente == ready->primerNodo){
            ready->primerNodo = 0;
        }else{
			ready->primerNodo->siguiente->anterior = ready->primerNodo->anterior;
			ready->primerNodo->anterior->siguiente = ready->primerNodo->siguiente;
			ready->primerNodo = ready->primerNodo->siguiente;
        }
		borrado->anterior = NULL;
        borrado->siguiente = NULL;
        return borrado;
    }
}

/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
NodoJobScheduler* extraerBurst(ListaColaReady * ready){
    NodoJobScheduler * borrado;
	int minBurst = ready->primerNodo->burst;
	int minPos = 0;
	int pos = 0;
	
    if (isListaEmpty(ready)){
        return 0;
    } else{
        if (ready->primerNodo->siguiente == ready->primerNodo){
            borrado = ready->primerNodo;
            ready->primerNodo = 0;
        } else{
			NodoJobScheduler * temp = ready->primerNodo->siguiente;
			while (temp != ready->primerNodo){
				pos ++;
            	if (temp->burst < minBurst){
					minBurst = temp->burst;
					minPos = pos;
				}
				temp = temp->siguiente;
        	}
        	borrado = borrarEnPosicion(minPos, ready);
        }
		
        return borrado;
    }
}

/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
NodoJobScheduler* extraerPrioridad(ListaColaReady * ready){
    NodoJobScheduler * borrado;
	int minPrioridad = ready->primerNodo->prioridad;
	int minPos = 0;
	int pos = 0;
	
    if (isListaEmpty(ready)){
        return 0;
    } else{
        if (ready->primerNodo->siguiente == ready->primerNodo){
            borrado = ready->primerNodo;
            ready->primerNodo = 0;
        } else{
    		NodoJobScheduler * temp = ready->primerNodo->siguiente;
			while (temp != ready->primerNodo){
				pos ++;
            	if (temp->prioridad < minPrioridad){
					minPrioridad = temp->prioridad;
					minPos = pos;
				}
				temp = temp->siguiente;
        	}
        	borrado = borrarEnPosicion(minPos, ready);
        }
		
        return borrado;
    }
}

/*
 * Entradas: no tiene.
 * Salidas: true si la lista esta vacia, false en caso contrario
 * Restricciones: no tiene.
 */
bool isListaEmpty(ListaColaReady * ready){
    printf("Entro isEmpty\n");
    if (ready->primerNodo == 0){
        printf("Ens Verdadero\n");
        return true;
    }
    printf("Es Falso\n");
    return false;
}

/*
 * Entradas: no tiene
 * Salidas: no tiene
 * Restricciones: la lista no puede estar vacia
 */
void imprimirLista(ListaColaReady * ready){
	printf("\n~~~ESTADO DE LA COLA DE READY~~~\n");
    if (isListaEmpty(ready)){
        printf("\nCola de Ready vacia...\n");
    }else{
        printf("\nProceso PID: %d, BURST: %d, PRIORIDAD: %d \n", ready->primerNodo->PID, ready->primerNodo->burst, ready->primerNodo->prioridad);
        NodoJobScheduler * temp = ready->primerNodo->siguiente;
        while (temp != ready->primerNodo){
            printf("\nProceso PID: %d, BURST: %d, PRIORIDAD: %d \n", temp->PID, temp->burst, temp->prioridad);
            temp = temp->siguiente;
        }
    }
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/*
 * Entradas: int de la pos a eliminar
 * Salidas: no tiene
 * Restricciones: no tiene
 */
NodoJobScheduler * borrarEnPosicion(int pos, ListaColaReady * ready){
	NodoJobScheduler * borrado;
	
    if (pos == 0){
		borrado = ready->primerNodo;
        ready->primerNodo->anterior->siguiente = ready->primerNodo->siguiente;
        ready->primerNodo->siguiente->anterior = ready->primerNodo->anterior;
        ready->primerNodo = ready->primerNodo->siguiente;
    } else{
    	NodoJobScheduler * temp = ready->primerNodo;
        while (pos != 0){
            pos --;
            temp = temp->siguiente;
        }
		borrado = temp;
        temp->anterior->siguiente = temp->siguiente;
        temp->siguiente->anterior = temp->anterior;
    }
	borrado->anterior = NULL;
    borrado->siguiente = NULL;
	return borrado;
}

