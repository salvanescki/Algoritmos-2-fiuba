#include "lista.h"
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
    if(pila == NULL) return NULL;
    pila->datos = malloc(8 * sizeof(void*));
    if(pila->datos == NULL) return NULL;    //8 tam min de la pila
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

bool sumar_numero(void* a, void* b){
    int* a_int = a;
    int* b_int = b;
    *a_int += *b_int;
    return true;
}

bool imprimir_elementos(void* a, void* b){
    int* a_int = a;
    printf("%d\n",*a_int);
    return true;
}

void insertar_varios_elementos(lista_t* lista, int veces){
    for (int i=0;i<veces;i++){
        lista_insertar_primero(lista, &i);
    }
}

void borrar_varios_elementos(lista_t* lista, int veces){
    for(int i=0;i<=veces;i++){
        lista_borrar_primero(lista);
    }
}

void pruebas_insertar_borrar(lista_t* lista){
    int* valor = 0;
    int valor1 = 1;
    printf("COMIENZO DE PRUEBAS INSERTAR/BORRAR\n");
    lista = lista_crear();
    print_test("Prueba crear lista", lista!=NULL);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba largo de lista es 0", lista_largo(lista) == 0);
    print_test("Prueba no hay primer elemento", lista_ver_primero(lista) == NULL);
    print_test("Prueba no hay ultimo elemento", lista_ver_ultimo(lista) == NULL);
    print_test("Prueba insertar 'valor' en primera posicion", lista_insertar_primero(lista, valor));
    print_test("Prueba 'valor' primer y ultimo elemento", lista_ver_primero(lista) == valor && lista_ver_ultimo(lista) == valor);
    print_test("Prueba largo de lista es 1", lista_largo(lista) == 1);
    print_test("Prueba insertar 'valor1' en primera posicion", lista_insertar_primero(lista, &valor1));
    print_test("Prueba 'valor1' es el primer elemento", lista_ver_primero(lista) == &valor1);
    print_test("Prueba 'valor' es el ultimo elemento", lista_ver_ultimo(lista) == valor);
    print_test("Prueba largo de lista es 2", lista_largo(lista) == 2);
    print_test("Prueba lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba borrar primer elemento 'valor1'", lista_borrar_primero(lista) == &valor1);
    print_test("Prueba 'valor' primer elemento", lista_ver_primero(lista) == valor);
    print_test("Prueba largo de lista es 1", lista_largo(lista) == 1);
    print_test("Prueba borrar primer elemento 'valor'", lista_borrar_primero(lista) == valor);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba largo de lista es 0", lista_largo(lista) == 0);
    print_test("Prueba insertar 'valor' en ultima posicion", lista_insertar_ultimo(lista, valor));
    print_test("Prueba 'valor' primer y ultimo elemento", lista_ver_primero(lista) == valor && lista_ver_ultimo(lista) == valor);
    print_test("Prueba largo de lista es 1", lista_largo(lista) == 1);
    print_test("Prueba insertar 'valor1' en ultima posicion", lista_insertar_ultimo(lista, &valor1));
    print_test("Prueba 'valor' es el primer elemento", lista_ver_primero(lista) == valor);
    print_test("Prueba 'valor1' es el ultimo elemento", lista_ver_ultimo(lista) == &valor1);
    print_test("Prueba largo de lista es 2", lista_largo(lista) == 2);
    print_test("Prueba borrar primer elemento 'valor'", lista_borrar_primero(lista) == valor);
    print_test("Prueba borrar primer elemento 'valor1'", lista_borrar_primero(lista) == &valor1);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba largo de lista es 0", lista_largo(lista) == 0);
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista", true);
}

void pruebas_elemento_null(lista_t* lista){
    printf("COMIENZO DE PRUEBAS DEL ELEMENTO NULL\n");
    lista = lista_crear();
    print_test("Prueba crear lista", lista!=NULL);
    lista_insertar_primero(lista, NULL);
    print_test("Prueba insertar primero NULL", lista_ver_primero(lista) == NULL);
    lista_borrar_primero(lista);
    print_test("Prueba borrar primero NULL", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista vacia", true);
}

void pruebas_condicion_de_borde(lista_t* lista){
    printf("COMIENZO DE PRUEBAS DE CONDICION DE BORDE\n");
    lista = lista_crear();
    print_test("Prueba crear lista", lista!=NULL);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba largo de lista es 0", lista_largo(lista) == 0);
    print_test("Prueba no hay primer elemento", lista_ver_primero(lista) == NULL);
    print_test("Prueba no hay ultimo elemento", lista_ver_ultimo(lista) == NULL);
    print_test("Prueba no se puede borrar elemento",lista_borrar_primero(lista) == NULL);
    printf("Inserto 6 elementos\n");
    insertar_varios_elementos(lista,6);
    print_test("Prueba largo de lista es 6", lista_largo(lista) == 6);
    print_test("Prueba lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba hay primer elemento", lista_ver_primero(lista) != NULL);
    print_test("Prueba hay ultimo elemento", lista_ver_ultimo(lista) != NULL);
    printf("Borro todos los elementos\n");
    borrar_varios_elementos(lista,6);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba largo de lista es 0", lista_largo(lista) == 0);
    print_test("Prueba no hay primer elemento", lista_ver_primero(lista) == NULL);
    print_test("Prueba no hay ultimo elemento", lista_ver_ultimo(lista) == NULL);
    print_test("Prueba no se puede borrar elemento",lista_borrar_primero(lista) == NULL);
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista vacia", true);
}

void pruebas_destruccion(lista_t* lista, pila_t* pila){
    int* valor = 0;
    int valor1 = 1;
    printf("COMIENZO DE PRUEBAS DE DESTRUCCION\n");
    lista = lista_crear();
    pila = pila_crear();
    print_test("Prueba insertar una pila en primera posicion", lista_insertar_primero(lista, pila));
    print_test("Prueba pila primer elemento de la lista", lista_ver_primero(lista) == pila);
    lista_destruir(lista, pila_destruir_wrapper);
    print_test("Prueba destruir lista con la funcion pila_destruir funciona", true);
    print_test("Prueba lista esta destruida", true);
    lista = lista_crear();
    print_test("Prueba volver a crear la lista", true);
    print_test("Prueba insertar valor", lista_insertar_primero(lista, valor));
    print_test("Prueba insertar valor1", lista_insertar_primero(lista, &valor1));
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista no vacia", true);
}

void pruebas_iter_interno(lista_t* lista){
    printf("COMIENZO DE PRUEBAS DE ITERADOR INTERNO\n");
    lista = lista_crear();
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    lista_insertar_ultimo(lista,&a);
    lista_insertar_ultimo(lista,&b);
    lista_insertar_ultimo(lista,&c);
    lista_insertar_ultimo(lista,&d);
    print_test("Prueba inserto numeros del 1 al 4", true);
    lista_iterar(lista, imprimir_elementos, NULL);
    print_test("Prueba los imprimo en pantalla",true);
    lista_iterar(lista, sumar_numero, &e);
    print_test("Prueba sumo 5 a todos los elementos", true);
    lista_iterar(lista, imprimir_elementos, NULL);
    print_test("Prueba imprimo los resultados en pantalla", true);
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista no vacia", true);
}

void pruebas_iter_externo(lista_t* lista){
    int* valor = 0;
    int valor1 = 1;
    int valor2 = 2;
    int valor3 = 3;
    int num = 5;
    printf("COMIENZO DE PRUEBAS DE ITERADOR EXTERNO\n");
    lista_iter_t* iter = NULL;
    print_test("Puntero iter inicializado a NULL", iter == NULL);
    lista = lista_crear();
    iter = lista_iter_crear(lista);
    print_test("Prueba crear iter para lista vacia", iter!=NULL);
    print_test("Prueba iter apunta al ultimo elemento", lista_iter_ver_actual(iter) == NULL);
    print_test("Prueba insertar un elemento 'valor' en la primera posicion", lista_iter_insertar(iter, valor));
    print_test("Prueba el primer elemento de la lista es 'valor'", lista_ver_primero(lista) == valor);
    print_test("Prueba 'valor' es el elemento al que esta apuntando iter", lista_iter_ver_actual(iter) == valor);
    print_test("Prueba avanzar iter", lista_iter_avanzar(iter));
    print_test("Prueba iter esta en el final", lista_iter_al_final(iter));
    print_test("Prueba iter no puede avanzar mas", !lista_iter_avanzar(iter));
    print_test("Prueba insertar elemento 'valor1'", lista_iter_insertar(iter, &valor1));
    print_test("Prueba 'valor1' fue insertado e iter lo esta apuntando", lista_iter_ver_actual(iter) == &valor1);
    print_test("Prueba avanzar iter", lista_iter_avanzar(iter));
    print_test("Prueba iter esta en el final (insertar al final es como insertar ultimo)", lista_iter_al_final(iter));
    lista_iter_destruir(iter);
    print_test("Prueba destruir iter", true);
    iter = lista_iter_crear(lista);
    print_test("Prueba crear iter para lista con elementos (apunta al primer elemento)", lista_iter_ver_actual(iter) == lista_ver_primero(lista));
    print_test("Prueba avanzar iter", lista_iter_avanzar(iter));
    print_test("Prueba el elemento actual es 'valor1' el ultimo elemento", lista_iter_ver_actual(iter) == &valor1);
    print_test("Prueba insertar elemento 'num' entre 'valor' y 'valor1'", lista_iter_insertar(iter,&num));
    lista_iter_avanzar(iter);
    print_test("Prueba el siguiente elemento es 'valor1'", lista_iter_ver_actual(iter) == &valor1);
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    print_test("Prueba el primer elemento sigue siendo 'valor'", lista_iter_ver_actual(iter) == valor);
    lista_iter_avanzar(iter);
    print_test("Prueba 'num' esta entre 'valor' y 'valor1' definitivamente", lista_iter_ver_actual(iter) == &num);
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    lista_iter_borrar(iter);
    print_test("Prueba cambia el primer elemento de la lista al remover apenas creado iter", lista_ver_primero(lista) == &num);
    print_test("Prueba iter queda apuntando al nuevo primer elemento 'num'", lista_iter_ver_actual(iter) == &num);
    print_test("Prueba insertar 'valor1' en la posicion actual (primera posicion)", lista_iter_insertar(iter, &valor1));
    print_test("Prueba iter queda apuntando a 'valor1' el nuevo primer elemento", lista_iter_ver_actual(iter) == &valor1);
    print_test("Prueba el primer elemento es 'valor1'",lista_ver_primero(lista) == &valor1);
    lista_iter_avanzar(iter);
    print_test("Prueba 'num' es el proximo elemento", lista_iter_ver_actual(iter) == &num);
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    lista_iter_borrar(iter);
    lista_iter_avanzar(iter);
    lista_iter_borrar(iter);
    print_test("Prueba al remover el ultimo elemento, en la lista cambia el ultimo elemento", lista_ver_ultimo(lista) == &num);
    print_test("Prueba iter apunta al final", lista_iter_ver_actual(iter) == NULL && lista_iter_al_final(iter));
    lista_iter_insertar(iter, valor);
    lista_iter_insertar(iter, &valor1);
    lista_iter_insertar(iter, valor);
    lista_iter_borrar(iter);
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    print_test("Prueba el primer elemento es 'num'",lista_iter_ver_actual(iter) == &num);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    lista_iter_borrar(iter);
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("Prueba al remover un elemento del medio, este desaparece", lista_iter_ver_actual(iter) != &valor1);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    lista = lista_crear();
    iter = lista_iter_crear(lista);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba iter esta al final", lista_iter_al_final(iter));
    print_test("Prueba insertar primero 'valor1'", lista_iter_insertar(iter, &valor1));
    print_test("Prueba primer elemento es 'valor1'", lista_ver_primero(lista) == &valor1);
    print_test("Prueba largo de lista es 1", lista_largo(lista) == 1);
    print_test("Prueba insertar primero 'valor2'", lista_iter_insertar(iter, &valor2));
    print_test("Prueba primer elemento es 'valor2'", lista_ver_primero(lista) == &valor2);
    print_test("Prueba largo de lista es 2", lista_largo(lista) == 2);
    print_test("Prueba insertar primero 'valor3'", lista_iter_insertar(iter, &valor3));
    print_test("Prueba primer elemento es 'valor3'", lista_ver_primero(lista) == &valor3);
    print_test("Prueba ultimo elemento es 'valor1'", lista_ver_ultimo(lista) == &valor1);
    print_test("Prueba largo de lista es 3", lista_largo(lista) == 3);
    print_test("Prueba iter esta apuntando a 'valor3'", lista_iter_ver_actual(iter) == &valor3);
    print_test("Prueba borrar 'valor3'", lista_iter_borrar(iter) == &valor3);
    print_test("Prueba 'valor2' es el nuevo primer elemento", lista_ver_primero(lista) == &valor2);
    print_test("Prueba iter esta apuntando a 'valor2'", lista_iter_ver_actual(iter) == &valor2);
    print_test("Prueba largo de lista es 2", lista_largo(lista) == 2);
    print_test("Prueba borrar 'valor2'", lista_iter_borrar(iter) == &valor2);
    print_test("Prueba 'valor1' es el nuevo primer y ultimo elemento", lista_ver_primero(lista) == &valor1 && lista_ver_ultimo(lista) == &valor1);
    print_test("Prueba largo de lista es 1", lista_largo(lista) == 1);
    print_test("Prueba iter esta apuntando a 'valor1'", lista_iter_ver_actual(iter) == &valor1);
    lista_iter_avanzar(iter);
    print_test("Prueba avanzo iter y ahora esta en el final", lista_iter_al_final(iter));
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    print_test("Prueba lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba iter apunta al ultimo elemento", lista_iter_ver_actual(iter) == lista_ver_ultimo(lista));
    print_test("Prueba el primero sigue siendo 'valor1'", lista_ver_primero(lista) == &valor1);
    print_test("Prueba borrar 'valor1'", lista_iter_borrar(iter) == &valor1);
    print_test("Prueba lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba iter esta apuntando al final", lista_iter_al_final(iter));
    print_test("Prueba borrar es NULL", lista_iter_borrar(iter) == NULL);
    lista_iter_destruir(iter);
    print_test("Prueba destruir iter", true);
    lista_destruir(lista, NULL);
    print_test("Prueba destruir lista", true);
}

void pruebas_lista_alumno() {
    lista_t* lista = NULL;
    pila_t* pila = NULL;
    print_test("Puntero lista inicializado a NULL", lista == NULL);
    print_test("Puntero pila inicializado a NULL", pila == NULL);
    pruebas_insertar_borrar(lista);
    pruebas_elemento_null(lista);
    pruebas_condicion_de_borde(lista);
    pruebas_destruccion(lista, pila);
    pruebas_iter_interno(lista);
    pruebas_iter_externo(lista);
}