#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo {
	void* dato;
	struct nodo* prox;
}nodo_t;

struct cola {
	nodo_t* prim;
	nodo_t* ult;
};

/********************************************
*				PRIMITIVAS					*
*********************************************/

cola_t* cola_crear(){
	cola_t* cola = malloc(sizeof(cola_t));
	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	if(cola_esta_vacia(cola)){
		free(cola);
	}else{
		if(destruir_dato){
			nodo_t* aux;
			while(!cola_esta_vacia(cola)){
				aux = cola->prim;
				destruir_dato(cola->prim->dato);
				cola->prim = aux->prox;
				free(aux);
				if(cola->prim == NULL){
					cola->ult = cola->prim;
				}
			}
			free(cola);
		}else{
			while(!cola_esta_vacia(cola)){
				cola_desencolar(cola);
			}
			free(cola);
		}
	}
}



bool cola_esta_vacia(const cola_t *cola){
	return cola->prim && cola->ult? false:true;
}

nodo_t* nodo_crear(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

bool cola_encolar(cola_t *cola, void* valor){
	if (cola_esta_vacia(cola)){
		nodo_t* nodo = nodo_crear(valor);
		cola->prim = nodo;
		cola->ult = nodo;
		return true;
	}
	nodo_t* nodo = nodo_crear(valor);
	cola->ult->prox = nodo;
	cola->ult = nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->prim->dato;
}

void* cola_desencolar(cola_t *cola){
	if(cola_esta_vacia(cola)){
		return NULL;
	}
	if(cola->prim == cola->ult){
		nodo_t* aux = cola->prim;
		void* dato = cola->prim->dato;
		cola->prim = NULL;
		cola->ult = NULL;
		free(aux);
		return dato;
	}else{
		nodo_t* aux = cola->prim;
		void* dato = cola->prim->dato;
		cola->prim = aux->prox;
		free(aux);
		return dato;
	}
}