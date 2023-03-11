#include "heap.h"
#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGO_ARRAY 20


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
void pila_destruir_wrapper(void* elem){
    pila_destruir(elem);
}

int comparar_numeros (const void *a, const void *b){
   return *(int*)a - *(int*)b;
 }

void encolar_un_elemento_varias_veces(heap_t* heap, int veces){
    int a = 9;
    for (int i=0;i<veces;i++){
        heap_encolar(heap, &a);
    }
}

void desencolar_varios_elementos(heap_t* heap, int veces){
    for(int i=0;i<=veces;i++){
        heap_desencolar(heap);
    }
}

/*
    menor a 0 si a < b
    0 si a == b
    mayor a 0 si a > b
*/




void pruebas_crear_destruir(){
    heap_t* heap = NULL;
    print_test("Puntero heap inicializado a NULL", heap == NULL);

    printf("COMIENZO DE PRUEBAS CREAR/DESTRUIR\n");
    
    heap = heap_crear(NULL);

    print_test("Prueba crear heap vacio", heap!=NULL);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba largo del heap es 0", heap_cantidad(heap) == 0);
    print_test("Prueba no hay elemento maximo", heap_ver_max(heap) == NULL);
    heap_destruir(heap, NULL);
    print_test("Prueba destruir heap vacio", true);
/*
    void** arr = malloc(sizeof(void*) * LARGO_ARRAY);
    int valor = 60;
    for(size_t i = 0; i < LARGO_ARRAY; i++) arr[i] = &valor;
    heap = heap_crear_arr(arr, LARGO_ARRAY, NULL);
    print_test("Prueba crear heap con array lleno de num 60", heap!=NULL);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba largo del heap es el largo del array", heap_cantidad(heap) == LARGO_ARRAY);
    print_test("Prueba el elemento maximo es 60", *(int*)heap_ver_max(heap) == 60);
    heap_destruir(heap, NULL);
    print_test("Prueba destruir heap vacio", true);
    */
}

void pruebas_encolar_desencolar(){
    heap_t* heap = NULL;
    print_test("Puntero heap inicializado a NULL", heap == NULL);
    printf("COMIENZO DE PRUEBAS ENCOLAR/DESENCOLAR\n");
    int a = 9;
    int b = 6;
    int c = 10;

    heap = heap_crear(comparar_numeros);

    print_test("Prueba crear heap vacio", heap != NULL);
    print_test("Prueba el array esta vacio", heap_esta_vacio(heap));
    print_test("Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba no hay primer elemento", heap_ver_max(heap) == NULL);
    print_test("Prueba encolar num 9", heap_encolar(heap, &a));
    print_test("Prueba num 9 maximo elemento", *(int*)heap_ver_max(heap) == a);
    print_test("Prueba la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba encolar num 6", heap_encolar(heap, &b));
    print_test("Prueba num 9 sigue como maximo elemento", *(int*)heap_ver_max(heap) == a);
    print_test("Prueba la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba encolar num 10 (mayor a num 9)", heap_encolar(heap, &c));
    print_test("Prueba num 10 es maximo elemento", *(int*)heap_ver_max(heap) == c);
    print_test("Prueba la cantidad de elementos es 3", heap_cantidad(heap) == 3);
    print_test("Prueba el array no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba desencolar num 10", *(int*)heap_desencolar(heap) == c);
    print_test("Prueba el valor maximo es num 9", *(int*)heap_ver_max(heap) == a);
    print_test("Prueba la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba desencolar num 9", *(int*)heap_desencolar(heap) == a);
    print_test("Prueba el valor maximo es num 6", *(int*)heap_ver_max(heap) == b);
    print_test("Prueba la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba desencolar num 6", *(int*)heap_desencolar(heap) == b);
    print_test("Prueba array esta vacio", heap_esta_vacio(heap));
    print_test("Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
    print_test("Prueba destruir heap con array vacio", true);
}

void pruebas_elemento_null(){
    printf("COMIENZO DE PRUEBAS DEL ELEMENTO NULL\n");
    heap_t* heap = NULL;
    print_test("Puntero inicializado a NULL", heap == NULL);
    heap = heap_crear(NULL);
    print_test("Prueba crear heap", heap!=NULL);
    heap_encolar(heap, NULL);
    print_test("Prueba encolar NULL", heap_ver_max(heap) == NULL);
    heap_desencolar(heap);
    print_test("Prueba desencolar NULL", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
    print_test("Prueba destruir heap", true);
}

void pruebas_redimension(){
    printf("COMIENZO DE PRUEBAS DE REDIMENSION Y CONDICION DE BORDE\n");
    heap_t* heap = NULL;
    print_test("Puntero inicializado a NULL", heap == NULL);
    int valor0 = 0;
    heap = heap_crear(comparar_numeros);
    
    print_test("Prueba crear heap", heap!=NULL);
    print_test("Prueba esta vacio",heap_esta_vacio(heap));
    print_test("Prueba no tiene elementos",heap_ver_max(heap) == NULL);
    print_test("Prueba no se puede desencolar",heap_desencolar(heap) == NULL);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    heap_encolar(heap, &valor0);
    /*
    encolar_un_elemento_varias_veces(heap,1000);
    print_test("Prueba encolar varios elementos", heap_ver_max(heap) == valor0);
    print_test("Prueba cantidad es 1001", heap_cantidad(heap) == 1001);
    desencolar_varios_elementos(heap,1001);
    */
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    heap_desencolar(heap);
    print_test("Prueba el heap esta vacio",heap_esta_vacio(heap));
    print_test("Prueba el heap no tiene elementos",heap_ver_max(heap) == NULL);
    print_test("Prueba es imposible desencolar",heap_desencolar(heap) == NULL);
    heap_destruir(heap, NULL);
    print_test("Prueba destruir heap", true);
}

void pruebas_destruccion(){
    printf("COMIENZO DE PRUEBAS DE DESTRUCCION\n"); 
    heap_t* heap = NULL;
    print_test("Puntero inicializado a NULL", heap == NULL);
    pila_t* pila = NULL;
    print_test("Puntero inicializado a NULL", pila == NULL);
    int valor0 = 0;
    int valor1 = 1;
    heap = heap_crear(NULL);
    pila = pila_crear();
    print_test("Prueba crear heap", heap!=NULL);
    print_test("Prueba encolar una pila", heap_encolar(heap, pila));
    print_test("Prueba pila elemento maximo del heap", heap_ver_max(heap) == pila);
    heap_destruir(heap, pila_destruir_wrapper);
    print_test("Prueba destruir con la funcion pila_destruir funciona", true);
    print_test("Prueba heap esta destruido", true);
    heap = heap_crear(comparar_numeros);
    print_test("Prueba volver a crear el heap", heap != NULL);
    print_test("Prueba encolar valor0", heap_encolar(heap, &valor0));
    print_test("Prueba encolar valor1", heap_encolar(heap, &valor1));
    heap_destruir(heap, NULL);
    print_test("Prueba destruir cola no vacia", true);
}



void pruebas_heap_alumno() {
    pruebas_crear_destruir();
    pruebas_encolar_desencolar();
    pruebas_elemento_null();
    pruebas_redimension();
    pruebas_destruccion();
}