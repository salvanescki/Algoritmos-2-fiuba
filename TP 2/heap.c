#include "heap.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAM_INICIAL 1
#define CONDICION_AGR 1
#define AGRANDAMIENTO 2

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

size_t hijo_mas_grande(heap_t* heap, size_t pos){
	size_t hijo1 = hijo_izq_pos(pos);
	size_t hijo2 = hijo_der_pos(pos);
	if(hijo1>=heap->cant) return hijo2;
	if(hijo2>=heap->cant) return hijo1;
	if(heap->cmp(heap->datos[hijo1], heap->datos[hijo2])>0){
		return hijo1;
	}else{
		return hijo2;
	}
}

void heap_swap(heap_t* heap, size_t pos_1, size_t pos_2){
	if ((pos_1 >= heap->cant) || (pos_2 >= heap->cant)) return;
	void* aux = heap->datos[pos_1];
	heap->datos[pos_1] = heap->datos[pos_2];
	heap->datos[pos_2] = aux;

}

/*
#########################################
FUNCIONES IMPORTANTES
#########################################
*/
/*EL DE NICO*//*
void upheap(heap_t* heap, size_t pos){
	if(pos <= 0) return;
	size_t pos_padre = padre_pos(pos);
	void* padre = heap->datos[pos_padre];
	void* nodo = heap->datos[pos];
	if(heap->cmp(nodo, padre) > 0){
		heap_swap(heap, pos_padre, pos);
		upheap(heap, pos_padre);
	}
}*/
/*
void downheap(heap_t* heap, size_t pos, size_t cant){
	if(pos >= cant) return;
	size_t pos_max = pos;
	size_t pos_hijo_der = hijo_izq_pos(pos);
	size_t pos_hijo_izq = hijo_der_pos(pos);
	void* dato_pos_max = heap->datos[pos_max];
	void* dato_hijo_izq = heap->datos[pos_hijo_izq];
	void* dato_hijo_der = heap->datos[pos_hijo_der];
	if(pos_hijo_izq < cant && heap->cmp(dato_pos_max, dato_hijo_izq) < 0){
		pos_max = pos_hijo_izq;
	}
	if(pos_hijo_der < cant && heap->cmp(dato_pos_max, dato_hijo_der) < 0){
		pos_max = pos_hijo_der;
	}
	if(pos_max != pos){
		heap_swap(heap, pos, pos_max);
		downheap(heap, pos_max, cant);
	}
}*/

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
	void** datos_nuevo = realloc(heap->datos, sizeof(void*) * tam_nuevo);
	if(datos_nuevo == NULL) return false;
	heap->datos = datos_nuevo;
	heap->tam = tam_nuevo;
	return true;
}
/* EL DE POLI:*/
bool upheap (heap_t* heap, size_t pos){
	size_t p_padre = padre_pos(pos);
	if(pos<=0) return false;
	if(heap->cmp(heap->datos[pos], heap->datos[p_padre])>0){
		heap_swap(heap, pos, p_padre);
		upheap(heap, p_padre);
	}
	return true;
}

bool downheap (heap_t* heap, size_t pos){
	size_t p_hijoIzq = hijo_izq_pos(pos);
	size_t p_hijoDer = hijo_der_pos(pos);
	if(p_hijoDer>=heap->cant && p_hijoIzq>=heap->cant) return false;
	size_t p_hijoGrande = hijo_mas_grande(heap, pos);
	if(heap->cmp(heap->datos[pos], heap->datos[p_hijoGrande])<0){
		heap_swap(heap, pos, p_hijoGrande);
		downheap(heap, p_hijoGrande);
	}
	return true;
}

/*
#########################################
HEAPSORT
#########################################
*/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t* heap = heap_crear(cmp);
	for(int i=0; i<cant; i++){
		heap_encolar(heap, elementos[i]);
	}
	for(int i = 0; i<cant ; i++){
		elementos[i] = heap_desencolar(heap);
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

void heap_destruir(heap_t *heap, void destruir_elemento(void *)){
	if(heap->datos != NULL){
		if(destruir_elemento){
			for(size_t i = 0; i < heap->cant; i++){
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
	if(heap->cant == heap->tam/CONDICION_AGR){
		if(!heap_redimensionar(heap, heap->tam *AGRANDAMIENTO)) return false;
	}
	heap->datos[heap->cant] = elem;
	heap->cant++;
	upheap(heap, heap->cant -1);
	
	return true;
}

void *heap_ver_max(const heap_t *heap){
	return heap_esta_vacio(heap)? NULL : heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	heap_swap(heap, 0, heap->cant -1);
	void* dato = heap->datos[heap->cant - 1];
	heap->cant--;
	downheap(heap, 0);
	return dato;
}