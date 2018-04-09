#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>


#include "estructuras.h"

/*
 * Inserta un proceso en la Cola del Ready
 * Entradas: Burst, Prioridad y PID del proceso a insertar
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void insertar(int b,int p, int id, int t, ListaColaReady * ready){
    NodoJobScheduler * nuevo = malloc(sizeof(NodoJobScheduler));
    nuevo->burst = b; nuevo->burst_temp = b; nuevo->prioridad = p;
    nuevo->PID = id; nuevo->tLlegada = t; nuevo->tSalida = 0; nuevo->escogido = false;
	
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
 * Inserta un proceso en la Cola del Ready
 * Entradas: Burst, Prioridad y PID del proceso a insertar
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void insertarNodo(NodoJobScheduler * nuevo, ListaColaReady * ready){	
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
    printf("\nProceso ejecutado (Historial) -> PID: %d, BURST: %d, PRIORIDAD: %d \n", nuevo->PID, nuevo->burst, nuevo->prioridad);
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
        return 0;
    }else{
        NodoJobScheduler * temp = ready->primerNodo->siguiente;
        while (temp != ready->primerNodo){
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
int extraerFIFO(ListaColaReady * ready, ListaColaReady * hist){
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
        borrado->tSalida = time(NULL) - hist->tiempoInicial + borrado->burst;
		insertarNodo(borrado, hist);
        return borrado->burst;
    }
}



/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
int extraerSJF(ListaColaReady * ready, ListaColaReady * hist){
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
		
        borrado->tSalida = time(NULL) - hist->tiempoInicial + borrado->burst;
		insertarNodo(borrado, hist);
        return borrado->burst;
    }
}

/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
int extraerHPF(ListaColaReady * ready, ListaColaReady * hist){
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
		borrado->tSalida = time(NULL) - hist->tiempoInicial + borrado->burst;
		insertarNodo(borrado, hist);
        return borrado->burst;
    }
}

/*
 * Borra de la lista
 * Entradas: Nodo a eliminar
 * Salidas: puntero al nodo borrado
 * Restricciones: No tiene
 */
int extraerRR(int quantum, ListaColaReady * ready, ListaColaReady * hist){
    int burstExtraido;
    if (isListaEmpty(ready)){
        return 0;
    }else{
        int pos = 0;
        NodoJobScheduler * temp = ready->primerNodo;
        while (temp->escogido){
            if (temp == ready->primerNodo && temp->anterior->escogido){
                resetearEscogido(ready);
            } else {
                temp = temp->siguiente;
                pos ++;
            }
        }
        if (temp->burst_temp > quantum){
            burstExtraido = quantum;
            temp->burst_temp = temp->burst_temp - quantum;
            temp->escogido = true;
        } else {
            NodoJobScheduler * borrado = borrarEnPosicion(pos, ready);
            burstExtraido = borrado->burst_temp;
            borrado->tSalida = time(NULL) - hist->tiempoInicial + borrado->burst_temp;
	        insertarNodo(borrado, hist);
            
        }
        printf("\nProceso escogido: PID %d, Burst: %d\n",temp->PID, burstExtraido);
        return burstExtraido;
    }
}

void resetearEscogido(ListaColaReady * ready){
    ready->primerNodo->escogido = false;
    NodoJobScheduler * temp = ready->primerNodo->siguiente;
    while(temp != ready->primerNodo){
        temp->escogido = false;
        temp = temp->siguiente;
    }
}

/*
 * Entradas: no tiene.
 * Salidas: true si la lista esta vacia, false en caso contrario
 * Restricciones: no tiene.
 */
bool isListaEmpty(ListaColaReady * ready){
    if (ready->primerNodo == 0){
        return true;
    }
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
        printf("\nProceso PID: %d, BURST: %d, PRIORIDAD: %d, TIEMPO LLEGADA: %d, TIEMPO SALIDA: %d\n", ready->primerNodo->PID, ready->primerNodo->burst_temp, ready->primerNodo->prioridad, ready->primerNodo->tLlegada, ready->primerNodo->tSalida);
        NodoJobScheduler * temp = ready->primerNodo->siguiente;
        while (temp != ready->primerNodo){
            printf("\nProceso PID: %d, BURST: %d, PRIORIDAD: %d, TIEMPO LLEGADA: %d, TIEMPO SALIDA: %d\n", temp->PID, temp->burst_temp, temp->prioridad, temp->tLlegada, temp->tSalida);
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
        if (ready->primerNodo->siguiente == ready->primerNodo){
            ready->primerNodo = 0;
        } else {
            ready->primerNodo->anterior->siguiente = ready->primerNodo->siguiente;
            ready->primerNodo->siguiente->anterior = ready->primerNodo->anterior;
            ready->primerNodo = ready->primerNodo->siguiente;
        }
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

void reporteCPU(ListaColaReady * hist){
    printf("\n~~~REPORTE CPU SCHEDULING~~~\n");
    if (isListaEmpty(hist)){
        printf("\nNo hay procesos finalizados...\n");
    } else{
        NodoJobScheduler * temp = hist->primerNodo->siguiente;
        int largo = largoLista(hist);
        int ocioso = 0;
        int TAT_Total = 0;
        int TW_Total = 0;
        int TAT = hist->primerNodo->tSalida - hist->primerNodo->tLlegada;
        int TW = TAT - hist->primerNodo->burst;
        TAT_Total += TAT;
        TW_Total += TW;
        printf("\n|PID\t|B \t|P \t|T.L\t|T.S\t|TAT\t|TW\n");
        printf("|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\n",hist->primerNodo->PID, hist->primerNodo->burst, hist->primerNodo->prioridad, hist->primerNodo->tLlegada, hist->primerNodo->tSalida, TAT,TW);
        while (temp != hist->primerNodo){
            TAT = temp->tSalida - temp->tLlegada;
            TW = TAT - temp->burst;
            TAT_Total += TAT;
            TW_Total += TW;
            printf("|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\n",temp->PID, temp->burst, temp->prioridad, temp->tLlegada, temp->tSalida, TAT,TW);
            if (temp->tLlegada - temp->anterior->tSalida > 0){
                ocioso += temp->tLlegada - temp->anterior->tSalida;
            }
            temp = temp->siguiente;
        }
        printf("\nTiempo Final: %d\n", hist->tiempoFinal);
        printf("\nCantidad de procesos terminados: %d\n", largo);
        
        if (hist->tiempoFinal - temp->anterior->tSalida > 0){
            ocioso += hist->tiempoFinal - temp->anterior->tSalida;
        }
        
        printf("\nPromedio Turn-Around Time: %d\n", TAT_Total / largo);
        printf("Promedio Waiting Time: %d\n", TW_Total / largo);
        printf("\nTiempo CPU Ocioso: %d\n", ocioso);
        
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

bool cmpString(char str1[], char str2[]){
    int i = 0;
    while(str1[i]){
        if (!(str1[i] == str2[i])){
            return false;
        }
        i++;
    }
    return true;
}



