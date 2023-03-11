#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void apilar_varios_elementos(pila_t *pila, int veces){
    for (int i=0;i<veces;i++){
        pila_apilar(pila, &i);
    }
}

void desapilar_varios_elementos(pila_t *pila, int veces){
    for(int i=0;i<veces;i++){
        pila_desapilar(pila);
    }
}

void pruebas_pila_vacia(pila_t* ejemplo){
    print_test("Puntero inicializado a NULL", ejemplo == NULL);
    printf("COMIENZO DE PRUEBAS CREAR-DESTRUIR CON PILA VACIA\n");
    ejemplo = pila_crear();
    print_test("Prueba crear pila", ejemplo != NULL);
    print_test("Prueba pila esta vacia", pila_esta_vacia(ejemplo));
    pila_destruir(ejemplo);
    print_test("Prueba destruir pila vacia", true);
}

void pruebas_apilar_desapilar(pila_t* ejemplo){
    int* valor0 = 0;
    int valor1 = 1;
    char valor2 = 'r';
    printf("COMIENZO DE PRUEBAS APILAR-DESAPILAR\n");
    ejemplo = pila_crear();
    print_test("Prueba crear pila", ejemplo != NULL);
    print_test("Prueba apilar 'valor0'", pila_apilar(ejemplo, valor0));
    print_test("Prueba el nuevo tope es 'valor0'", pila_ver_tope(ejemplo) == valor0);
    print_test("Prueba apilar 'valor1'", pila_apilar(ejemplo, &valor1));
    print_test("Prueba 'valor1' es el nuevo tope", pila_ver_tope(ejemplo) == &valor1);
    print_test("Prueba apilar 'valor2'", pila_apilar(ejemplo, &valor2));
    print_test("Prueba 'valor2' es el nuevo tope", pila_ver_tope(ejemplo) == &valor2);
    print_test("Prueba desapilar 'valor2'", pila_desapilar(ejemplo) == &valor2);
    print_test("Prueba 'valor1' es el nuevo tope", pila_ver_tope(ejemplo) == &valor1);
    print_test("Prueba desapilar 'valor1'", pila_desapilar(ejemplo) == &valor1);
    print_test("Prueba 'valor0' es el nuevo tope", pila_ver_tope(ejemplo) == valor0);
    print_test("Prueba desapilar 'valor0'", pila_desapilar(ejemplo) == valor0);
    print_test("Prueba pila esta vacia", pila_esta_vacia(ejemplo));
    pila_destruir(ejemplo);
}

void pruebas_redimension(pila_t* ejemplo){
    int valor = 2;
    printf("COMIENZO DE PRUEBAS REDIMENSION\n");
    ejemplo = pila_crear();
    apilar_varios_elementos(ejemplo,9);
    pila_apilar(ejemplo, &valor);
    print_test("Prueba apilar hasta la redimension de pila", pila_ver_tope(ejemplo) == &valor);
    desapilar_varios_elementos(ejemplo,10);
    print_test("Prueba desapilar hasta pila vacia",pila_esta_vacia(ejemplo));
    pila_destruir(ejemplo);
}

void pruebas_elemento_NULL(pila_t* ejemplo){
    int* valor = 0;
    printf("COMIENZO DE PRUEBAS DEL ELEMENTO NULL\n");
    ejemplo = pila_crear();
    pila_apilar(ejemplo, valor);
    print_test("Prueba apilar 'valor'",pila_ver_tope(ejemplo) == valor);
    pila_apilar(ejemplo, NULL);
    print_test("Prueba apilar NULL", pila_ver_tope(ejemplo) == NULL);
    desapilar_varios_elementos(ejemplo,2);
    print_test("Prueba despilar ambos elementos", pila_esta_vacia(ejemplo));
    pila_destruir(ejemplo);
}

void pruebas_condiciones_de_borde(pila_t* ejemplo){
    int valor = 99;
    printf("COMIENZO DE PRUEBAS DE CONDICION DE BORDE\n");
    ejemplo = pila_crear();
    print_test("Prueba crear pila", true);
    print_test("Prueba pila esta vacia",pila_esta_vacia(ejemplo));
    print_test("Prueba pila no tiene tope",pila_ver_tope(ejemplo) == NULL);
    print_test("Prueba es imposible desapilar",pila_desapilar(ejemplo) == NULL);
    apilar_varios_elementos(ejemplo,99);
    pila_apilar(ejemplo, &valor);
    print_test("Prueba apilar 100 elementos", true);
    print_test("Prueba elemento '100' es el tope", pila_ver_tope(ejemplo) == &valor);
    desapilar_varios_elementos(ejemplo,100);
    print_test("Prueba desapilar todos los elementos", true);
    print_test("Prueba pila esta vacia",pila_esta_vacia(ejemplo));
    print_test("Prueba pila no tiene tope",pila_ver_tope(ejemplo) == NULL);
    print_test("Prueba es imposible desapilar",pila_desapilar(ejemplo) == NULL);
    pila_destruir(ejemplo);
}

void pruebas_pila_no_vacia(pila_t* ejemplo){
    int valor = 9999;
    printf("COMIENZO DE PRUEBAS DE PILAS NO VACIAS\n");
    ejemplo = pila_crear();
    print_test("Prueba crear pila", true);
    apilar_varios_elementos(ejemplo, 9999);
    pila_apilar(ejemplo, &valor);
    print_test("Prueba apilar 10000 elementos", pila_ver_tope(ejemplo) == &valor);
    pila_destruir(ejemplo);
    print_test("Prueba destruir pila", true);
}
void pruebas_pila_alumno() {
    pila_t* ejemplo = NULL;
    pruebas_pila_vacia(ejemplo);
    pruebas_apilar_desapilar(ejemplo);
    pruebas_redimension(ejemplo);
    pruebas_elemento_NULL(ejemplo);
    pruebas_condiciones_de_borde(ejemplo);
    pruebas_pila_no_vacia(ejemplo);
}