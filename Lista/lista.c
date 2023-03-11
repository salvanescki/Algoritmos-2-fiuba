#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                    TIPOS DE DATOS ABSTRACTOS
 * *****************************************************************/

typedef struct nodo {
	void* dato;
	struct nodo* prox;
}nodo_t;

struct lista {
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};

struct lista_iter{
	nodo_t* act;
	nodo_t* ant;
	lista_t* lista;
};

/* ******************************************************************
 *                     FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t* nodo_crear(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL) return NULL;
	nodo->dato = dato;
	nodo->prox = NULL;
	return nodo;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(){
	lista_t* lista = malloc(sizeof(lista_t));
	if(lista == NULL) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t* lista){
	return !(lista->prim && lista->ult);
}

bool lista_insertar_primero(lista_t* lista, void* dato){
	nodo_t* nodo = nodo_crear(dato);
	if(nodo == NULL) return false;
	if(lista_esta_vacia(lista)){
		lista->prim = nodo;
		lista->ult = nodo;
		lista->largo = 1;
		return true;
	}
	nodo_t* aux = lista->prim;
	lista->prim = nodo;
	lista->prim->prox = aux;
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato){
	nodo_t* nodo = nodo_crear(dato);
	if(nodo == NULL) return false;
	if(lista_esta_vacia(lista)){
		lista->prim = nodo;
		lista->ult = nodo;
		lista->largo = 1;
		return true;
	}
	lista->ult->prox = nodo;
	lista->ult = nodo;
	lista->largo++;
	return true;
}

void* lista_borrar_primero(lista_t* lista){
	if(lista_esta_vacia(lista)){
		return NULL;
	}
	nodo_t* aux = lista->prim;
	void* dato = aux->dato;
	if(lista->prim == lista->ult){
		lista->prim = NULL;
		lista->ult = NULL;
		lista->largo = 0;
		free(aux);
		return dato;		
	}
	lista->prim = aux->prox;
	lista->largo--;
	free(aux);
	return dato;
}

void* lista_ver_primero(const lista_t* lista){
	return lista_esta_vacia(lista)? NULL:lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
	return lista_esta_vacia(lista)? NULL:lista->ult->dato;
}

size_t lista_largo(const lista_t* lista){
	return lista->largo;
}

void lista_destruir(lista_t* lista, void destruir_dato(void *)){
	while(!lista_esta_vacia(lista)){
		if(destruir_dato){
			destruir_dato(lista->prim->dato);
		}
		lista_borrar_primero(lista);
	}
	free(lista);
}

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
	if(visitar){
		nodo_t* act = lista->prim;
		while(act){
			if(!visitar(act->dato,extra)) return;
			act = act->prox;
		}
	}
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t* lista_iter_crear(lista_t* lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;
	iter->lista = lista;
	if(lista_esta_vacia(lista)){
		iter->act = lista->ult;
		iter->ant = NULL;
		return iter;
	}
	iter->act = lista->prim;
	iter->ant = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter){
	if(lista_iter_al_final(iter)){
		return false;
	}
	iter->ant = iter->act;
	iter->act = iter->act->prox;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t *iter){
	return lista_iter_al_final(iter)? NULL:iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->ant == iter->lista->ult;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if(lista_iter_al_final(iter)){
		lista_insertar_ultimo(iter->lista, dato);
		iter->act = iter->lista->ult;
		return true;
	}
	if(iter->act == iter->lista->prim){
		lista_insertar_primero(iter->lista, dato);
		iter->act = iter->lista->prim;
		return true;
	}
	nodo_t* nodo = nodo_crear(dato);
	nodo_t* aux = iter->act;
	iter->ant->prox = nodo;
	iter->act = nodo;
	iter->act->prox = aux;
	iter->lista->largo++;
	return true;
}

void* lista_iter_borrar(lista_iter_t *iter){
	if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)){
		return NULL;
	}
	void* dato;
	if(iter->act == iter->lista->prim){
		dato = lista_borrar_primero(iter->lista);
		iter->act = iter->lista->prim;
		return dato;
	}
	nodo_t* aux = iter->act;
	dato = iter->act->dato;
	if(iter->act == iter->lista->ult){
		iter->lista->ult = iter->ant;
		iter->act = NULL;
		iter->lista->largo--;
		free(aux);
		return dato;
	}
	iter->act = aux->prox;
	iter->ant->prox = aux->prox;
	iter->lista->largo--;
	free(aux);
	return dato;
}