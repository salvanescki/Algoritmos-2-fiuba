#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

/* 
################################################
PRIMITIVAS DEL ABB 
################################################
*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
/*
Crea un abb
POST: Devuelve un abb vacio
*/

bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
/*
Guarda una clave y un dato en el abb
PRE: El abb fue creado
POST: Clave y dato fueron guardados en el abb
*/

void *abb_borrar(abb_t *arbol, const char *clave);
/*
Borra el dato (y la clave) de una determinada clave en el abb
PRE: El abb fue creado
POST: Devuelve el dato borrado respectivo a la clave
*/

void *abb_obtener(const abb_t *arbol, const char *clave);
/*
Obtiene el dato del abb correspondiente a la clave introducida.
PRE: El abb fue creado
POST: Devuelve el dato correspondiente a la clave
*/

bool abb_pertenece(const abb_t *arbol, const char *clave);
/*
Devuelve true (o false) si una clave pertenece al abb (o no)
PRE: El abb fue creado
POST: Devuelve true o false si la clave pertenece o no al abb
*/

size_t abb_cantidad(abb_t *arbol);
/*
Devuelve la cantidad de nodos que posee el abb
PRE: El abb fue creado
POST: Devuelve la cantidad de nodos del abb
*/

void abb_destruir(abb_t *arbol);
/*
Destruye el abb
PRE: El abb fue creado
POST: Libera cada nodo del abb y luego a él
*/

/* 
################################################
ITERADOR INTERNO IN ORDER
################################################
*/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra, void* clave_desde, void* clave_hasta);

/*
Recorre cada nodo del abb aplicando la funcion visitar
PRE: El abb fue creado
POST: Aplica la funcion visitar a todos los nodos del abb.
*/

/* 
################################################
PRIMITIVAS DEL ITERADOR EXTERNO 
################################################
*/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
/*
Crea un iterador externo del abb
PRE: El abb fue creado
POST: Se crea un iterador externo para dicho abb
*/

bool abb_iter_in_avanzar(abb_iter_t *iter);
/*
Avanza el iterador segun la busqueda IN ORDER
PRE: El iterador fue creado
POST: Avanza el iterador de acuerdo a IN ORDER
*/

const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
/*
Devuelve la clave del nodo al cual el iterador se encuentra
apuntando.
PRE: El iterador fue creado
POST: Devuelve la clave actual que apunta el iterador, o NULL si esta
al final
*/

bool abb_iter_in_al_final(const abb_iter_t *iter);
/*
Devuelve true o false si el iterador apunta al final
PRE: El iterador fue creado
POST: Devuelve true si iter apunta al final del abb,
false si aun no esta al final
*/

void abb_iter_in_destruir(abb_iter_t* iter);
/*
Destruye el iterador externo del abb
PRE: El iterador fue creado
POST: El iterador es destruido
*/

#endif // ABB_H