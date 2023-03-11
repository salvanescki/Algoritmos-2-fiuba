#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct heap heap_t;
typedef int (*cmp_func_t) (const void *a, const void *b);

heap_t *heap_crear(cmp_func_t cmp);
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);
void heap_destruir(heap_t *heap, void destruir_elemento(void *e));

size_t heap_cantidad(const heap_t *heap);
bool heap_esta_vacio(const heap_t *heap);

bool heap_encolar(heap_t *heap, void *elem);
void *heap_ver_max(const heap_t *heap);
void *heap_desencolar(heap_t *heap);

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

void pruebas_heap_alumno(void);

#endif // HEAP_H