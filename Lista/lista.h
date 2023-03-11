#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

/* "lista_iter" es el iterador externo de la lista. */

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void* lista_borrar_primero(lista_t* lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void* lista_ver_primero(const lista_t* lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del último, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void* lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista. Si la lista está vacía devuelve 0.
// Pre: la lista fue creada.
// Post: se devolvió el largo de la lista.
size_t lista_largo(const lista_t* lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void *));

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Aplica una función pasada por referencia a cada uno de los nodos
// de la lista.
// Pre: la lista fue creada.
// Post: la función fue aplicada a cada elemento de la lista.

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador externo para la lista.
// Pre: la lista fue creada.
// Post: devuelve un nuevo iterador externo para la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al iterador al siguiente nodo. Si el iterador se encuentra
// al final de la lista, devuelve false.
// Pre: el iterador fue creado.
// Post: devuelve true si fue posible avanzar, false si la lista
// está vacía, el iterador se encuentra al final 
// o si un error impidió el avance del iterador.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor de la actual posición en la que se encuentra
// el iterador.
// Pre: el iterador fue creado.
// Post: devuelve el dato de la actual posición del iterador. Devuelve
// NULL si se encuentra en el final de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true o false si el iterador se encuentra en la
// posición final o no respectivamente.
// Pre: el iterador fue creado.
// Post: devuelve true si el iterador está al final
// de la lista, o false si no es así.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: destruye el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un dato entre la actual posicion del iterador y el
// correspondiente nodo anterior.
// Pre: el iterador fue creado.
// Post: inserta un nuevo dato en la lista de acuerdo a la posición
// actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el nodo al que el iterador se encuentra apuntando. Devuelve
// el dato de dicho nodo, o NULL si la lista esta vacia
// Pre: el iterador fue creado.
// Post: borra el nodo actual, devuelve su dato.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // LISTA_H
