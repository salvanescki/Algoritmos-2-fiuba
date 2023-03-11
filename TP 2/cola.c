#include "cola.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

struct nodo;
typedef struct nodo nodo_t;

struct nodo {
    void* dato;
    nodo_t* sig;
};

struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

int cola_cantidad(cola_t* cola){
	
	if(!cola->primero) return 0;
	int cant = 1;
	if(cola->primero == cola->ultimo) return 1;
	nodo_t* act = cola->primero;
	while((act = act->sig)){
		cant++;
	}
	return cant;
}

cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if(cola==NULL) return NULL;
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola;
}

nodo_t* nodo_crear(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo==NULL) return NULL;
	nodo->dato = valor;
	nodo->sig = NULL;
	return nodo;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)){
	//Caso vacia
	if(cola_esta_vacia(cola)){
		free(cola);
		return;
	}
	//Caso base
	if(cola->ultimo == cola->primero){
		if (destruir_dato) destruir_dato(cola->primero->dato);
		free(cola->primero);
		free(cola);
		return;
	}
	//Caso recursivo
	nodo_t* aux_nodo = cola->primero;
	cola->primero = cola->primero->sig;
	if (destruir_dato) destruir_dato(aux_nodo->dato);
	free(aux_nodo);
	cola_destruir(cola, destruir_dato);
	return;
}

bool cola_esta_vacia(const cola_t *cola){
	return !(cola->primero);
}

bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = nodo_crear(valor);
	if (nodo==NULL) return false;

	if(cola_esta_vacia(cola)){
		cola->primero = nodo;//No estoy seguro que quiere decir la correccion aca
		cola->ultimo = nodo;//Aca hay un return, y no puedo asignar el ultimo antes
		return true;//Porque sino dejaria de estar vacia
	}
	cola->ultimo->sig = nodo;
	cola->ultimo = nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	return cola->primero->dato;
}

void* cola_desencolar(cola_t *cola){

	if (cola_esta_vacia(cola)) return NULL;
	
	void* aux_dato = cola->primero->dato;
	
	if (cola->ultimo == cola->primero) cola->ultimo = NULL;
	nodo_t* aux_nodo = cola->primero;
	cola->primero = cola->primero->sig;
	free(aux_nodo);

	return aux_dato;
}