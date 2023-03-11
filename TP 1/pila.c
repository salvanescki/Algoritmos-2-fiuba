#include "pila.h"
#include <stdlib.h>

#define TAM_MIN 8

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

//Auxiliares
bool redimensionar(pila_t *pila, size_t tam){
	void** datos_nuevo = realloc(pila->datos, tam * sizeof(void*));
	if(datos_nuevo == NULL) return false;
	pila->datos = datos_nuevo;
	pila->capacidad = tam;
	return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL) return NULL;
	pila->datos = malloc(TAM_MIN * sizeof(void*));
	if(pila->datos == NULL){ 
		free(pila);
		return NULL;
	}
	pila->capacidad = TAM_MIN;
	pila->cantidad = 0;
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void* valor){
	if(pila->cantidad == pila->capacidad){
		if(!redimensionar(pila, pila->capacidad * 2)) return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	return pila_esta_vacia(pila)? NULL : pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	 }
	if(pila->cantidad < pila->capacidad /4){
		if(!redimensionar(pila, pila->capacidad /2)) return NULL;
	}
	void* tope_ant = pila->datos[pila->cantidad -1];
	pila->datos[pila->cantidad -1] = NULL;
	pila->cantidad--;
	return tope_ant;
}