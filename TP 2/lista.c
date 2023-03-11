#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

struct nodo;
typedef struct nodo nodo_t;

struct nodo {
    void* dato;
    nodo_t* sig;
};

struct lista {
	nodo_t* primero;
	size_t largo;
	nodo_t* ultimo;
};

struct lista_iter {
	nodo_t* actual;
	nodo_t* anterior;
	lista_t* lista;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	lista->primero = NULL;
	lista->largo = 0;
	lista->ultimo = NULL;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	if(lista->largo == 0) return true;
	return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return false;
	nodo->dato = dato;
	if (lista_esta_vacia(lista)){
		nodo->sig = NULL;
		lista->primero = nodo;
		lista->ultimo = nodo;
	} else {
		nodo->sig = lista->primero;
		lista->primero = nodo;
	}
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	if (lista_esta_vacia(lista)){
		return(lista_insertar_primero(lista,dato));
	}
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return false;
	nodo->sig = NULL;
	nodo->dato = dato;
	lista->ultimo->sig = nodo;
	lista->ultimo = nodo;
	lista->largo++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	void* aux = lista->primero->dato;
	if (lista->largo == 1){
		free(lista->primero);
		lista->primero = NULL;
		lista->ultimo = NULL;
		lista->largo--;
		return aux;
	}
	void* dir_aux = lista->primero;
	lista->primero = lista->primero->sig;
	free(dir_aux);
	lista->largo--;
	return aux;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	//caso vacia
	if(lista_esta_vacia(lista)){
		free(lista);
		return;
	}
	//CASO SIN FUNCION
	if(destruir_dato == NULL){
		//caso base
		if(lista->ultimo == lista->primero){
			free(lista->primero);
			free(lista);
			return;
		}
		//caso recursivo
		void* aux_nodo = lista->primero;
		lista->primero = lista->primero->sig;
		free(aux_nodo);
		lista_destruir(lista, NULL);
		return;
	}
	//CASO CON FUNCION
	//caso base
	if(lista->ultimo == lista->primero){
		destruir_dato(lista->primero->dato);
		free(lista->primero);
		free(lista);
		return;
	}
	//caso recursivo
	nodo_t* aux_nodo = lista->primero;
	lista->primero = lista->primero->sig;
	destruir_dato(aux_nodo->dato);
	free(aux_nodo);
	lista_destruir(lista, destruir_dato);
	return;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if (!visitar) return;
	nodo_t* actual = lista->primero;
	while (actual){
		if (visitar(actual->dato,extra)==false) return;
		actual = actual->sig;
	}
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;
	iter->actual = lista->primero;
	iter->anterior = NULL;
	iter->lista = lista;
	return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	if (iter->actual == NULL) return true;
	return false;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (lista_iter_al_final(iter)) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->sig;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return false;
	nodo->dato = dato;
	nodo->sig = NULL;
	//CASO 0
	if(lista_esta_vacia(iter->lista)){
		iter->actual = nodo;
		iter->lista->primero = nodo;
		iter->lista->ultimo = nodo;
		iter->lista->largo++;
		return true;
	}
	//CASO AL FINAL
	if (lista_iter_al_final(iter)){
		iter->actual = nodo;
		iter->anterior->sig = nodo;
		iter->lista->ultimo = nodo;
		iter->lista->largo++;
		return true;
	}
	//CASO 1
	if(iter->lista->largo == 1){
		iter->actual = nodo;
		nodo->sig = iter->lista->primero;
		iter->lista->primero = nodo;
		iter->lista->largo++;
		return true;
	}
	//CASO GENERICO AL PRINCIPIO
	if(iter->anterior == NULL){
		iter->actual = nodo;
		nodo->sig = iter->lista->primero;
		iter->lista->primero = nodo;
		iter->lista->largo++;
		return true;
	}
	//CASO GENERICO
	iter->actual = nodo;
	nodo->sig = iter->anterior->sig;
	iter->anterior->sig = nodo;
	iter->lista->largo++;
	return true;

}

void *lista_iter_borrar(lista_iter_t *iter){
	//CASO 0
	if(lista_esta_vacia(iter->lista) || iter->actual == NULL) return NULL;
	//
	void* aux = iter->actual->dato;
	//CASO 1
	if(iter->lista->largo == 1){
		lista_borrar_primero(iter->lista);
		iter->actual = NULL;
		iter->anterior = NULL;
		return aux;
	}
	//CASO GENERICO AL PRINCIPIO
	if (iter->actual == iter->lista->primero){
		lista_borrar_primero(iter->lista);
		iter->actual = iter->lista->primero;
		return aux;
	}
	//CASO GENERICO
	iter->anterior->sig = iter->actual->sig;
	free(iter->actual);
	iter->actual = iter->anterior->sig;
	iter->lista->largo--;
	if(iter->lista->largo == 1){
		iter->lista->ultimo = iter->lista->primero;
	}
	return aux;
}
