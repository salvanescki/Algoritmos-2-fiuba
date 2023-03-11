#include "abb.h"
#include "pila.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct abb_nodo abb_nodo_t;

struct abb_nodo{
	abb_nodo_t* izq;
	abb_nodo_t* der;
	char* clave;
	void* dato;
};

struct abb{
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cant;
};

struct abb_iter{
	abb_nodo_t* act;
	pila_t* pila;
	const abb_t* abb;
};

/*
################################################
AUXILIARES
################################################
*/

char* abb_mystrdup(const char *str) {
    return strcpy(malloc(strlen(str) + 1), str);
}

/*
################################################
FUNCIONES AUXILIARES DE NODOS
################################################
*/
/*
void swap(abb_nodo_t* x, abb_nodo_t* y){
	abb_nodo_t z = *x;
	*x = *y;
	*y = z;
}*/



abb_nodo_t* abb_nodo_crear(char* clave, void* dato){
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = clave;
	nodo->dato = dato;
	return nodo;
}

abb_nodo_t* abb_nodo_buscar(const abb_t* arbol, abb_nodo_t* nodo, const char* clave){
	if (arbol->cmp(clave, nodo->clave) == 0){
		return nodo;
	}else if (arbol->cmp(clave, nodo->clave) < 0){
		if(!nodo->izq) return NULL;
		return abb_nodo_buscar(arbol, nodo->izq, clave);
	}else{
		if(!nodo->der) return NULL;
		return abb_nodo_buscar(arbol, nodo->der, clave);
	}
}

bool _es_padre_de(abb_nodo_t* nodo_h, abb_nodo_t* nodo_p, abb_t* abb){
	if(!abb->cmp(nodo_p->izq->clave, nodo_h->clave) || !abb->cmp(nodo_p->der->clave, nodo_h->clave)){
		return true;
	}
	return false;
}

bool es_padre_de (char* clave_h, char* clave_p, abb_t* abb){
	abb_nodo_t* nodo_h = abb_nodo_buscar(abb, abb->raiz, clave_h);
	abb_nodo_t* nodo_p = abb_nodo_buscar(abb, abb->raiz, clave_p);
	return _es_padre_de(nodo_h, nodo_p, abb);
}



abb_nodo_t* abb_nodo_buscar_padre(const abb_t* arbol, abb_nodo_t* nodo, const char* clave, int* izq){
	*izq = 0;
	if(!arbol->raiz || !arbol->cmp(clave, nodo->clave)) return NULL;
	if(nodo->izq && !arbol->cmp(nodo->izq->clave, clave)){
		*izq = 1;
		return nodo;
	} else if (nodo->der && !arbol->cmp(nodo->der->clave, clave)){
		*izq = 0;
		return nodo;
	}else if (arbol->cmp(clave, nodo->clave)<0){
		if(!nodo->izq) return NULL;
		return abb_nodo_buscar_padre(arbol, nodo->izq, clave, izq);
	} else{
		if(!nodo->der) return NULL;
		return abb_nodo_buscar_padre(arbol, nodo->der, clave, izq);
	}
}

bool tiene_padre(const abb_t* arbol, const char* clave){
	int* izq = malloc(sizeof(int));
	bool resul = abb_nodo_buscar_padre(arbol, arbol->raiz, clave, izq);
	free(izq);
	return resul;
}

void abb_nodo_insertar(abb_t* arbol, abb_nodo_t* nodo, char* clave, void* dato){
	if(arbol->cmp(clave, nodo->clave) < 0){
		if(nodo->izq){  
			abb_nodo_insertar(arbol, nodo->izq, clave, dato);
			return;
		}else{
			nodo->izq = abb_nodo_crear(clave, dato);
		}
	}else{
		if(nodo->der){
			abb_nodo_insertar(arbol, nodo->der, clave, dato);
			return;
		}else{
			nodo->der = abb_nodo_crear(clave, dato);
		}
	}
}

abb_nodo_t* abb_nodo_minimo(abb_nodo_t* nodo){
	abb_nodo_t* actual = nodo;
	while(actual->izq){
		actual = actual->izq;
	}
	return actual;
}

abb_nodo_t* abb_nodo_maximo(abb_nodo_t* nodo){
	abb_nodo_t* actual = nodo;
	while(actual->der){
		actual = actual->der;
	}
	return actual;
}

void* abb_nodo_borrar(abb_t *arbol, abb_nodo_t* nodo, const char *clave){
	int *izq = malloc(sizeof(int));
	abb_nodo_t* padre = abb_nodo_buscar_padre(arbol, arbol->raiz, clave, izq);
	if(!nodo->izq && !nodo->der){
		if(arbol->raiz == nodo){
			arbol->raiz = NULL;
		}
		void* dato = nodo->dato;
		free(nodo->clave);
		free(nodo);
		if(padre){
			(*izq)? (padre->izq = NULL) : (padre->der = NULL);
		}
		free(izq);
		arbol->cant--;

		return dato;
	}else if(!nodo->izq || !nodo->der){

		void* dato = nodo->dato;
		abb_nodo_t* reemplazo;
		
		(!nodo->izq)? (reemplazo = nodo->der) : (reemplazo = nodo->izq);
		if(arbol->raiz == nodo){
			arbol->raiz = reemplazo;
		}
		if(padre){
			(*izq)? (padre->izq = reemplazo) : (padre->der = reemplazo);
		}
		free(izq);
		free(nodo->clave);
		free(nodo);
		arbol->cant--;
		return dato;
	} else {
		abb_nodo_t* reemplazo = abb_nodo_maximo(nodo->izq);

		free(izq);
		void* dato = nodo->dato;
		
		char* clave_reemplazo = abb_mystrdup(reemplazo->clave);
		void* dato_reemplazo = reemplazo->dato;
		abb_nodo_borrar(arbol, reemplazo, clave_reemplazo);
		free(nodo->clave);
		nodo->dato = dato_reemplazo;
		nodo->clave = clave_reemplazo;
		
		return dato;
	}
}

void abb_borrar_simple(abb_t* arbol, abb_nodo_t* nodo){
	if(nodo->izq) abb_borrar_simple(arbol, nodo->izq);
	if(nodo->der) abb_borrar_simple(arbol, nodo->der);
	if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
}



/*
################################################
PRIMITIVAS DEL ABB
################################################
*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
	if(abb == NULL) return NULL;
	abb->raiz = NULL;
	abb->cmp = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cant = 0;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if(arbol == NULL || clave == NULL) return false;
	char* clave_copia = abb_mystrdup(clave);
	if(!arbol->raiz){
		arbol->raiz = abb_nodo_crear(clave_copia, dato);
	}else{
		abb_nodo_insertar(arbol, arbol->raiz, clave_copia, dato);
	}
	arbol->cant++;

	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if(!abb_pertenece(arbol, clave)) return NULL;
	abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
	return abb_nodo_borrar(arbol, nodo, clave);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if(!abb_pertenece(arbol, clave)) return NULL;
	abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!arbol->raiz) return NULL;
	return abb_nodo_buscar(arbol, arbol->raiz, clave);
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant;
}



void abb_destruir(abb_t *arbol){
	if(arbol->raiz) abb_borrar_simple(arbol, arbol->raiz);
	free(arbol);
}

/*
################################################
ITERADOR INTERNO
################################################
*/

bool abb_iterar(abb_t* arbol, abb_nodo_t* nodo_act, bool visitar(const char *, void *, void *), void *extra, char* clave_desde, char* clave_hasta){
	if(nodo_act == NULL) return false;
	if(arbol->cmp(nodo_act->clave, clave_desde) > 0){
		if(nodo_act->der){
			if (!abb_iterar(arbol, nodo_act->der, visitar, extra, clave_desde, clave_hasta)) return false;
		}
	}else if(arbol->cmp(nodo_act->clave, clave_hasta) < 0){
		if(nodo_act->izq){
			if(!abb_iterar(arbol, nodo_act->izq, visitar, extra, clave_desde, clave_hasta)) return false;
		}
	}else{
		if(nodo_act->izq){
			if(!abb_iterar(arbol, nodo_act->izq, visitar, extra, clave_desde, clave_hasta)) return false;
		}
		if(!visitar(nodo_act->clave, nodo_act->dato, extra)) return false;
		if(nodo_act->der){
			if(!abb_iterar(arbol, nodo_act->der, visitar, extra, clave_desde, clave_hasta)) return false;
		}
	}
	return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra, void* clave_desde, void* clave_hasta){
	abb_iterar(arbol, arbol->raiz, visitar, extra, clave_desde, clave_hasta);
	return;
}

/*
################################################
PRIMITIVAS DEL ITERADOR EXTERNO
################################################
*/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	pila_t* pila =  pila_crear();
	if(pila == NULL) return NULL;
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(iter == NULL) return NULL;
	iter->abb = arbol;
	iter->pila = pila;
	iter->act = arbol->raiz;
	pila_apilar(pila, NULL);
	if(arbol->raiz) pila_apilar(pila, arbol->raiz);
	if (!iter->act) return iter;
	while(iter->act->izq){
		iter->act = iter->act->izq;
		pila_apilar(pila, iter->act);
	}
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	abb_nodo_t* nodo_act = pila_desapilar(iter->pila);
	if(!nodo_act) return false;
	if(nodo_act->der) {
		pila_apilar(iter->pila, nodo_act->der);
		nodo_act = nodo_act->der;
		while(nodo_act->izq){
			pila_apilar(iter->pila, nodo_act->izq);
			nodo_act = nodo_act->izq;
		}
	}
	iter->act = pila_ver_tope(iter->pila);	
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if (!iter->act) return NULL;
	return iter->act->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return !iter->act;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}