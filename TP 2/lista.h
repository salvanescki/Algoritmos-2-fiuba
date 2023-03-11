#ifndef LISTA_H
#define LISTA_H
#include <stddef.h>
#include <stdbool.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Crea una lista
//Post: devuelve una lista vacia
lista_t *lista_crear(void);

//Verfica que una lista este vacia
//Pre: la lista fue creada. Devuelve true si esta vacia y false en
//caso contrario
bool lista_esta_vacia(const lista_t *lista);

//Inserta el dato dado en la primer posicion de la lista.
//Pre: la lista fue creada.
//Post: la lista tiene un nuevo elemento en la primer posicion.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Inserta el dato dado en la ultima posicion de la lista.
//Pre: la lista fue creada.
//Post: la lista tiene un nuevo elemento en la ultima posicion.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Borra el dato en primera posicion y lo devuelve, para ser usado
//o liberado. De estar vacia devuelve NULL.
//Pre: la lista fue creada.
//Post: la lista tiene como primer elemento el que anteriormente
//era segundo.
void *lista_borrar_primero(lista_t *lista);

//Devuelve el primer dato en la lista sin modificarla. De estar 
//vacia devuelve NULL.
//Pre: la lista fue creada.
void *lista_ver_primero(const lista_t *lista);

//Devuelve el ultimo dato en la lista sin modificarla. De estar 
//vacia devuelve NULL.
//Pre: la lista fue creada.
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve el largo de la lista sin modificarla.
//Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

//Destruye toda la lista, en caso de necesitar una funcion para
//liberar memoria de los datos se puede pasar como argumento. En
//caso de no necesitarlo pasar NULL.
//Pre: la lista fue creada.
//Post: la lista fue completamente borrada.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

//Aplica una funcion a todos los elementos de la lista.
//Pre: la lista fue creada.
//Post: la función pasada como argumento fue ejecutada sobre todos
//los elementos de la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea un iterador externo.
//Post: el iterador fue creado.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza el iterador una posicion. En caso de estar ya al final
//devuelve false. En caso contrario devuelve true.
//Pre: el iterador fue creado.
//Post: el iterador esta en la posicion siguiente
bool lista_iter_avanzar(lista_iter_t *iter);

//Devuelve el dato del elemento de la lista sobre el cual este
//el iterador sin modificarla. En caso de que este al final 
//devuelve void.
//Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Verifica que el iterador este al final de la lista. En ese caso
//devuelve true, en caso contrario devuelve false.
//Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador. Son descartables, no estes triste.
//Pre: el iterador fue creado.
//Post: el iterador fue completamente destruido.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta el dato dado en la posicion donde esta el iterador, 
//"corriendo" los datos siguientes. Devuelve false en caso de
//no poder, en caso contrario devuelve true.
//Pre: el iterador fue creado.
//Post: la lista tiene un nuevo elemento en la posicion del iterador
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Borra el elemento sobre el que este el iterador y devuelve el dato
//que estaba en ese elemento. En caso de que este al final devuelve NULL
//Pre: el iterador fue creado.
//Post: La lista tiene un elemento menos en la posicion donde esta el
//iterador.
void *lista_iter_borrar(lista_iter_t *iter);



/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
void pruebas_lista_alumno(void);

#endif // LISTA_H
