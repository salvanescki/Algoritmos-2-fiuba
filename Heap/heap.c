#include "heap.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAM_INICIAL 10

struct heap {
	void** datos;
	size_t tam;
	size_t cant;
	cmp_func_t cmp;
};

/*
#########################################
FUNCIONES AUXILIARES
#########################################
*/

size_t padre_pos(size_t pos_hijo){
	return (pos_hijo - 1) / 2;
}

size_t hijo_izq_pos(size_t pos_padre){
	return (2 * pos_padre) + 1;
}

size_t hijo_der_pos(size_t pos_padre){
	return (2 * pos_padre) + 2;
}

void swap(heap_t* heap, size_t pos_1, size_t pos_2){
	if (((pos_1 < 0) || (pos_1 >= heap->cant)) || ((pos_2 < 0) || (pos_2 >= heap->cant))) return;
	void* aux = heap->datos[pos_1];
	heap->datos[pos_1] = heap->datos[pos_2];
	heap->datos[pos_2] = aux;
}

/*
#########################################
FUNCIONES IMPORTANTES
#########################################
*/
void upheap(heap_t* heap, size_t pos, size_t cant){
	if(pos <= 0) return;
	size_t pos_padre = padre_pos(pos);
	void* padre = heap->datos[pos_padre];
	void* nodo = heap->datos[pos];
	if(heap->cmp(nodo, padre) > 0){
		swap(heap, pos_padre, pos);
		upheap(heap, pos_padre, cant);
	}
}

void downheap(heap_t* heap, size_t pos, size_t cant){
	if(pos>=cant) return;
	size_t p_hijoDer = hijo_der_pos(pos);
	size_t p_hijoIzq = hijo_izq_pos(pos);
	void* d_hijoDer = heap->datos[p_hijoDer];
	void* d_hijoIzq = heap->datos[p_hijoIzq];
	if(p_hijoIzq < cant && heap->cmp(heap->datos[pos], d_hijoIzq) < 0){
		swap(heap, pos, p_hijoIzq);
		downheap(heap, p_hijoIzq, cant);
	} else if(p_hijoDer < cant && heap->cmp(heap->datos[pos], d_hijoDer) < 0){
		swap(heap, pos, p_hijoDer);
		downheap(heap, p_hijoDer, cant);
	}
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
	void** datos_nuevo = realloc(heap->datos, sizeof(void*) * tam_nuevo);
	if(datos_nuevo == NULL) return false;
	heap->datos = datos_nuevo;
	heap->tam = tam_nuevo;
	return true;
}

/*
#########################################
HEAPSORT
#########################################
*/

void heapify(heap_t* heap, size_t cant){
	size_t k = (cant - 2) / 2;
	while(k >= 0){
		downheap(heap, k, cant);
		k--;
	}
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t* heap = heap_crear_arr(elementos, cant, cmp);
	size_t k = cant - 1;
	while(k + 1 > 0){
		elementos[k] = heap_desencolar(heap);
		k--;
	}
	heap_destruir(heap, NULL);
}

/*
#########################################
PRIMITIVAS DEL HEAP
#########################################
*/

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(heap == NULL) return NULL;
	heap->datos = malloc(sizeof(void*) * TAM_INICIAL);
	if(heap->datos == NULL){
		free(heap);
		return NULL;
	}
	heap->cmp = cmp;
	heap->tam = TAM_INICIAL;
	heap->cant = 0;
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(heap == NULL) return NULL;
	heap->datos = malloc(sizeof(void*) * n);
	if(heap->datos == NULL){
		free(heap);
		return NULL;
	}
	heap->cmp = cmp;
	heap->tam = n;
	heap->cant = 0;
	for(size_t i = 0; i < n; i++){
		heap->datos[i] = arreglo[i];
	}
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *)){
	if(heap->datos != NULL){
		for(size_t i = 0; i < heap->cant; i++){
			if(destruir_elemento){
				destruir_elemento(heap->datos[i]);
			}
		}
		free(heap->datos);
	}
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap? heap->cant : 0;
}

bool heap_esta_vacio(const heap_t *heap){
	return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
	if(heap == NULL) return false;
	if(heap->cant == heap->tam){
		if(!heap_redimensionar(heap, heap->tam *2)) return false;
	}
	heap->datos[heap->cant] = elem;
	heap->cant++;
	upheap(heap, heap->cant -1, heap->cant);
	
	return true;
}

void *heap_ver_max(const heap_t *heap){
	return heap_esta_vacio(heap)? NULL : heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	if(heap->cant == heap->tam/4){
		if(!heap_redimensionar(heap, heap->tam /2)) return false;
	}
	swap(heap, 0, heap->cant -1);
	void* dato = heap->datos[heap->cant - 1];
	heap->cant--;
	downheap(heap, 0, heap->cant);
	return dato;
}