#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t* pila_crear(){
    pila_t* pila = malloc(sizeof(pila_t));
    pila->datos = malloc(8 * sizeof(void*));    //8 tam min de la pila
    pila->capacidad = 8;
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);

}

void pila_destruir_wrapper(void* elem){
    pila_destruir((pila_t*)elem);
}

void encolar_varios_elementos(cola_t* cola, int veces){
    int* valor = 0;
    for (int i=0;i<=veces;i++){
        cola_encolar(cola, valor);
    }
}

void desencolar_varios_elementos(cola_t* cola, int veces){
    for(int i=0;i<=veces;i++){
        cola_desencolar(cola);
    }
}

void pruebas_cola_alumno() {
    cola_t* cola = NULL;
    pila_t* pila = NULL;
    print_test("Puntero inicializado a NULL", cola == NULL);
    print_test("Puntero inicializado a NULL", pila == NULL);

    /*Pruebas encolar-desencolar*/
    int* valor = 0;
    int valor1 = 1;
    printf("COMIENZO DE PRUEBAS ENCOLAR-DESENCOLAR\n");
    cola = cola_crear();
    print_test("Prueba crear cola", cola!=NULL);
    print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
    print_test("Prueba encolar 'valor'", cola_encolar(cola, valor));
    print_test("Prueba 'valor' primer elemento", cola_ver_primero(cola)==valor);
    print_test("Prueba encolar 'valor1'", cola_encolar(cola, &valor1));
    print_test("Prueba 'valor' sigue como primer elemento", cola_ver_primero(cola)==valor);
    print_test("Prueba desencolar 'valor'", cola_desencolar(cola)==valor);
    print_test("Prueba 'valor1' primer elemento", cola_ver_primero(cola) == &valor1);
    print_test("Prueba desencolar 'valor1'", cola_desencolar(cola) == &valor1);
    print_test("Prueba cola esta vacia", cola_esta_vacia(cola));

    /*Elemento NULL*/
    printf("COMIENZO DE PRUEBAS DEL ELEMENTO NULL\n");
    cola_encolar(cola, NULL);
    print_test("Prueba encolar NULL", cola_ver_primero(cola) == NULL);
    cola_desencolar(cola);
    print_test("Prueba desencolar NULL", cola_esta_vacia(cola));
    cola_destruir(cola, NULL);
    print_test("Prueba destruir cola", true);

    /*Condicion de borde*/
    printf("COMIENZO DE PRUEBAS DE CONDICION DE BORDE\n");
    printf("Creo una nueva cola\n");
    cola = cola_crear();
    print_test("Prueba esta vacia",cola_esta_vacia(cola));
    print_test("Prueba no tiene elementos",cola_ver_primero(cola) == NULL);
    print_test("Prueba no se puede desencolar",cola_desencolar(cola) == NULL);
    printf("Encolo muchos elementos\n");
    encolar_varios_elementos(cola,6);
    printf("Desencolo todos los elementos\n");
    desencolar_varios_elementos(cola,6);
    print_test("Prueba la cola esta vacia",cola_esta_vacia(cola));
    print_test("Prueba la cola no tiene elementos",cola_ver_primero(cola) == NULL);
    print_test("Prueba es imposible desencolar",cola_desencolar(cola) == NULL);
    cola_destruir(cola, NULL);
    
    /*Pruebas de Destruccion*/
    printf("COMIENZO DE PRUEBAS DE DESTRUCCION\n");
    cola = cola_crear();
    pila = pila_crear();
    print_test("Prueba encolar una pila", cola_encolar(cola, pila));
    print_test("Prueba pila primer elemento de la cola", cola_ver_primero(cola) == pila);
    cola_destruir(cola, pila_destruir_wrapper);
    print_test("Prueba destruir con la funcion pila_destruir funciona", true);
    print_test("Prueba cola esta destruida", true);
    cola = cola_crear();
    print_test("Prueba volver a crear la cola", cola != NULL);
    print_test("Prueba encolar valor", cola_encolar(cola, valor));
    print_test("Prueba encolar valor1", cola_encolar(cola, &valor1));
    cola_destruir(cola, NULL);
    print_test("Prueba destruir cola no vacia", true);
    /**/

}