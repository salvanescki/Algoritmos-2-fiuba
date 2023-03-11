#include "pila.h"
#include <stddef.h>
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
const size_t capacidad_i = 4;
const size_t espacioOcupado_pReducir = 4; //La pila se reducirá cuando 1/x de su capacidad
										//este ocupada.
const size_t espacio_aReducir = 2; //Se reducira a 1/x de lo que era.

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo);

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad==0;
}

pila_t* pila_crear(void){
	pila_t* pila=malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;
	pila->cantidad=0;
	pila->capacidad=0;
	pila->datos=malloc(sizeof(void*)*capacidad_i);
	if (pila->datos == NULL){
		free(pila);
		return NULL;
	}
	pila->capacidad+=capacidad_i;
	return pila;
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo){
	void* datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    pila->datos = datos_nuevo;
    pila->capacidad = tam_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void* valor){
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	size_t espacioOcupado = pila->cantidad;
	size_t espacioTotal = pila->capacidad;
	if(espacioOcupado >= espacioTotal/2){
		pila_redimensionar(pila,pila->capacidad*2);
	}
	return true;	
}

size_t pila_get_cantidad(pila_t* pila){
	return  pila->cantidad;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	return pila->datos[(pila->cantidad)-1];
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

void* pila_desapilar(pila_t *pila){
	if (pila->cantidad==0) return NULL;
	void* aux;
	aux=pila_ver_tope(pila);
	pila->cantidad--;

	if(pila->cantidad<=(pila->capacidad/espacioOcupado_pReducir)){
		pila_redimensionar(pila,pila->capacidad/espacio_aReducir);
	}

	return aux;
}