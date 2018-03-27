#include "estructuras.h"

// Lista Cola de Ready

/*
 * Inserta un proceso en la Cola del Ready
 * Entradas: Burst, Prioridad y PID del proceso a insertar
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void ListaColaReady::insertar(int b,int p, int id){
    struct NodoJobScheduler nuevo = {b, p, id};
	struct NodoJobScheduler *ptr_nuevo = &nuevo;
	
    if (isEmpty()){
        primerNodo = nuevo;
        nuevo->anterior = nuevo;
        nuevo->siguiente = nuevo;
    }else{
        nuevo -> siguiente = primerNodo;
        nuevo->anterior = primerNodo->anterior;
        primerNodo -> anterior -> siguiente = nuevo;
        primerNodo -> anterior = nuevo;
    }
}

/*
 * Cuenta la cantidad de elementos.
 * Entradas: no tiene.
 * Salidas: la cantidad de procesos
 * Restricciones: No tiene
 */
int ListaColaReady::largo(){
    int cont = 1;
    if (isEmpty()){
        return 0;
    }else{
        struct NodoJobScheduler * temp = primerNodo->siguiente;
        while (temp != primerNodo){
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
NodoJobScheduler * ListaColaReady::extraerPID(){
    NodoJobScheduler * borrado;
    NodoJobScheduler * temp = primerNodo;
    if (isEmpty()){
        return 0;
    }else{
		borrado = primerNodo;
        if (primerNodo->siguiente == primerNodo){
            primerNodo = 0;
        }else{
			primerNodo->siguiente->anterior = primerNodo->anterior;
			primerNodo->anterior->siguiente = primerNodo->siguiente;
			primerNodo = primerNodo->siguiente;
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
NodoJobScheduler * ListaColaReady::extraerBurst(){
    NodoJobScheduler * borrado;
    NodoJobScheduler * temp = primerNodo->siguiente;
	int minBurst = primerNodo->burst;
	int minPos = 0;
	int pos = 0;
	
    if (isEmpty()){
        return 0;
    } else{
        if (primerNodo->siguiente == primerNodo){
            borrado = primerNodo;
            primerNodo = 0;
        } else{
			while (temp != primerNodo){
				pos ++;
            	if (temp->burst < minBurst){
					minBurst = temp->burst;
					minPos = pos;
				}
				temp = temp->siguiente;
        	}
        	borrado = borrarEnPosicion(minPos);
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
NodoJobScheduler * ListaColaReady::extraerPrioridad(){
    NodoJobScheduler * borrado;
    NodoJobScheduler * temp = primerNodo->siguiente;
	int minPrioridad = primerNodo->prioridad;
	int minPos = 0;
	int pos = 0;
	
    if (isEmpty()){
        return 0;
    } else{
        if (primerNodo->siguiente == primerNodo){
            borrado = primerNodo;
            primerNodo = 0;
        } else{
			while (temp != primerNodo){
				pos ++;
            	if (temp->prioridad < minPrioridad){
					minPrioridad = temp->prioridad;
					minPos = pos;
				}
				temp = temp->siguiente;
        	}
        	borrado = borrarEnPosicion(minPos);
        }
		
        return borrado;
    }
}

/*
 * Entradas: no tiene.
 * Salidas: true si la lista esta vacia, false en caso contrario
 * Restricciones: no tiene.
 */
bool ListaColaReady::isEmpty(){
    if (primerNodo == 0){
        return true;
    }
    return false;
}

/*
 * Entradas: no tiene
 * Salidas: no tiene
 * Restricciones: la lista no puede estar vacia
 */
void ListaColaReady::imprimir(){
	printf("\n~~~ESTADO DE LA COLA DE READY~~~\n");
    if (isEmpty()){
        printf("\nCola de Ready vacia...\n");
    }else{
        NodoJobScheduler * temp = primerNodo;
        while (temp != primerNodo){
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
NodoJobScheduler * ListaColaReady::borrarEnPosicion(int pos){
	NodoJobScheduler * borrado;
    NodoJobScheduler * temp = primerNodo;
	
    if (pos == 0){
		borrado = primerNodo;
        primerNodo->anterior->siguiente = primerNodo->siguiente;
        primerNodo->siguiente->anterior = primerNodo->anterior;
        primerNodo = primerNodo->siguiente;
    } else{
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





