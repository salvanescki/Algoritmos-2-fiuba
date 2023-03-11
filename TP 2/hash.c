#include "hash.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


#define TAM_INICIAL 10
#define OCUPADO 'o'
#define VACIO 'v'
#define BORRADO 'b'
#define AGRANDAR 2
#define ACHICAR 2

struct hash_item;
typedef struct hash_item hash_item_t;

struct hash{
    hash_item_t* tabla;
    size_t m; //Tamanio
    size_t n; //Cantidad
    hash_destruir_dato_t hash_destruir_dato;
};

struct hash_item{
    void* dato;
    char* clave;
    char estado;
};

struct hash_iter{
  size_t actual;
  const hash_t* hash;
};

static size_t hashing(const char* cp)
{
    size_t hash = 0;
    while (*cp)
        hash = (hash * 10) + *cp++ - '0';
    return hash;
}

void hash_llenar_vacio(hash_t* hash){
  for(int i=0; i<(hash->m); i++){
      hash->tabla[i].estado = VACIO;
      hash->tabla[i].clave = NULL;
  }
  return;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;
    hash->tabla = NULL;
    hash->m = 0;
    hash->n = 0;
    hash->hash_destruir_dato = destruir_dato;

    
    return hash;
}

void swap (hash_t* x, hash_t* y) {
  hash_t z;
  z=*x;
  *x=*y;
  *y=z;
}

bool hash_limpiar(hash_t* hash){
  hash_t* hash_nuevo = hash_crear(hash->hash_destruir_dato);
  if (!hash_nuevo) return false;
  swap(hash, hash_nuevo);
  hash_destruir(hash_nuevo);
  return true;
}

bool hash_redimensionar(hash_t *hash, size_t nuevo_tam){
    if(nuevo_tam<TAM_INICIAL) return true;
    hash_t* hash_nuevo = hash_crear(hash->hash_destruir_dato);

    hash_nuevo->tabla = malloc(nuevo_tam * sizeof(hash_item_t));
    if(hash_nuevo->tabla == NULL) return false;
    hash_nuevo->m = nuevo_tam;
    hash_llenar_vacio(hash_nuevo);
    
    for(size_t i=0;i<hash->m;i++){
        if(hash->tabla[i].estado == OCUPADO){
            hash_guardar(hash_nuevo, hash->tabla[i].clave, hash->tabla[i].dato);
        }          
    }
    swap(hash,hash_nuevo);
    hash_nuevo->hash_destruir_dato = NULL;
    hash_destruir(hash_nuevo);
    return true;
}

char *my_strdup(const char *str) {
    return strcpy(malloc( strlen(str) + 1),str);
}

bool hash_reemplazar(hash_t* hash, char* clave, void* dato){
    size_t aux = hashing(clave)%hash->m;
    while (hash->tabla[aux].clave==NULL || strcmp(hash->tabla[aux].clave,clave)){
        aux++;
        if(aux == hash->m){
            aux = 0;
        }
    }
    if (hash->hash_destruir_dato) hash->hash_destruir_dato(hash->tabla[aux].dato);
    hash->tabla[aux].dato = dato;
    if (hash->n > 0){
      if((hash->n)*10/(hash->m) > 7){
        if(!hash_redimensionar(hash, hash->m *AGRANDAR)) return false;
      }
    }
    return true;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if (hash == NULL || clave == NULL) return false;
    if (hash->tabla == NULL){
      hash->tabla = malloc(TAM_INICIAL*sizeof(hash_item_t));
      if (hash->tabla == NULL) return false;
      hash->m = TAM_INICIAL;
      hash_llenar_vacio(hash);
    }
    char* clave_copia = my_strdup(clave);
    size_t aux = hashing(clave_copia)%hash->m;
    while (hash->tabla[aux].estado != VACIO){
      if (hash->tabla[aux].clave!=NULL && !(strcmp(hash->tabla[aux].clave, clave_copia))){
        hash_reemplazar(hash, clave_copia, dato);
        free(clave_copia);
        return true;
      }
        aux++;
        if(aux == hash->m){
            aux = 0;
        }
    }

    hash->tabla[aux].clave = clave_copia;
    hash->tabla[aux].dato = dato;
    hash->tabla[aux].estado = OCUPADO;
    hash->n++;
    if (hash->n > 0){
      if((hash->n)*10/(hash->m) > 7){
        if(!hash_redimensionar(hash, hash->m *AGRANDAR)) return false;
      }
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;

    size_t aux = hashing(clave)%hash->m;
    while (hash->tabla[aux].clave==NULL || strcmp(hash->tabla[aux].clave,clave)){
        aux++;
        if(aux == hash->m){
            aux = 0;
        }
    }
    free(hash->tabla[aux].clave);
    void* dato_aux = hash->tabla[aux].dato;
    hash->tabla[aux].clave = NULL;
    hash->tabla[aux].dato = NULL;
    hash->tabla[aux].estado = BORRADO;
    hash->n--; 
    if((hash->n)*10/hash->m < 3){
      hash_redimensionar(hash, hash->m /ACHICAR);
  }
    return dato_aux;
}

void *hash_obtener(const hash_t *hash, const char *clave){
  if (!hash_pertenece(hash, clave)) return NULL;
    size_t aux = hashing(clave)%hash->m;
    while (hash->tabla[aux].clave==NULL || strcmp(hash->tabla[aux].clave, clave)){
        aux++;
        if(aux == hash->m){
          aux = 0;
        }
    }
  return hash->tabla[aux].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
  if(hash->n == 0)
    return false;
  if(hash == NULL || clave == NULL) return false;
  size_t aux = hashing(clave)%hash->m;
  while(hash->tabla[aux].clave==NULL || strcmp(hash->tabla[aux].clave, clave)){
    if(hash->tabla[aux].estado==VACIO) return false;
    aux++;
    if(aux == hash->m){
      aux = 0;
    }
  }
  if(!strcmp(hash->tabla[aux].clave, clave)) return true;
  return false;
}

size_t hash_cantidad(const hash_t *hash){
  return hash->n;
}

void hash_destruir(hash_t *hash){
    if (hash == NULL) return;
    
      for(size_t i=0; i< hash->m; i++){
        if(hash->tabla[i].estado == OCUPADO){
            if (hash->hash_destruir_dato) hash->hash_destruir_dato(hash->tabla[i].dato);
            free(hash->tabla[i].clave);
        }
      }
    
    if (hash->tabla) free(hash->tabla);
    free(hash);
}

/* Iterador del hash */


hash_iter_t *hash_iter_crear(const hash_t *hash){
  if (hash == NULL) return NULL;
  hash_iter_t* iter = malloc(sizeof(hash_iter_t));
  if (iter == NULL) return NULL;
  iter->hash = hash;
  iter->actual = 0;
  while(iter->actual < hash->m && hash->tabla[iter->actual].estado == VACIO){
    iter->actual++;
  }
  return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
  if (iter->actual == iter->hash->m){
    return false;
  }
  iter->actual++;
  while(iter->actual < iter->hash->m && iter->hash->tabla[iter->actual].estado == VACIO){
    iter->actual++;
  }
  return true;  
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
  if (iter->actual == iter->hash->m) return NULL;
  return iter->hash->tabla[iter->actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
  return iter->actual == iter->hash->m;
}

void hash_iter_destruir(hash_iter_t* iter){
  free(iter);
}
