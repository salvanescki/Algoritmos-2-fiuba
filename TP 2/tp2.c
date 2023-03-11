#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809L //GETLINE

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cola.h"
#include "lista.h"
#include "heap.h"
#include "abb.h"
#include "hash.h"
#include "strutil.h"
#include "timeutils.h"

#define LONG_IP 4
#define TAM_INICIAL 4
#define RANGODOS 2
#define CANTIDAD_DENTRO_DE_RANGO 5

/*
###########################################################
ESTRUCTURAS
###########################################################
*/
typedef struct entrada {
	char* ip;
	char* datetime;
	char* metodo;
	char* url_recurso;
} entrada_t;

typedef struct visitado{
	const int* numero;
	const char* recurso;
} visitado_t;

/*
###########################################################
PRIMITIVAS DE ENTRADA
###########################################################
*/
entrada_t* entrada_crear(char* ip, char* datetime, char* metodo, char* url_recurso){
	entrada_t* entrada = malloc(sizeof(entrada_t));
	if (entrada == NULL) return NULL;
	entrada->ip = ip;
	entrada->datetime = datetime;
	entrada->metodo = metodo;
	entrada->url_recurso = url_recurso;
	return entrada;
}

void entrada_destruir(entrada_t* entrada){
	free(entrada->ip);
	free(entrada->datetime);
	free(entrada->metodo);
	free(entrada->url_recurso);
	free(entrada);
}

visitado_t* crear_visitado(const int* numero, const char* recurso){
	visitado_t* visitado = malloc(sizeof(visitado_t));
	if(!visitado) return NULL;
	visitado->numero = numero;
	visitado->recurso = recurso;
	return visitado;
}

void visitado_destruir(visitado_t* visitado){
	free(visitado);
}

/*
###########################################################
STRCMP_IP
###########################################################
*/
int strcmp_ip (const char* ip_1, const char* ip_2){
	char** datos_ip1 = split(ip_1, '.');
	char** datos_ip2 = split(ip_2, '.');
	int* vector_ip1 = malloc(sizeof(int) * LONG_IP);
	int* vector_ip2 = malloc(sizeof(int) * LONG_IP);
	for(size_t i = 0; i < LONG_IP; i++){
		vector_ip1[i] = atoi(datos_ip1[i]);
	}
	for(size_t i = 0; i < LONG_IP; i++){
		vector_ip2[i] = atoi(datos_ip2[i]);
	}
	for (int i = 0 ; i < LONG_IP ; i++){
		if(vector_ip1[i]>vector_ip2[i]){
			free_strv(datos_ip1);
			free_strv(datos_ip2);
			free(vector_ip1);
			free(vector_ip2);
			return 1;
		}
		if(vector_ip1[i]<vector_ip2[i]){
			free_strv(datos_ip1);
			free_strv(datos_ip2);
			free(vector_ip1);
			free(vector_ip2);
			return -1;
		}
	}
	free_strv(datos_ip1);
	free_strv(datos_ip2);
	free(vector_ip1);
	free(vector_ip2);
	return 0;
}

/*
###########################################################
AUXILIARES
###########################################################
*/


void entrada_destruir_wrapper(void* entrada){
	entrada_destruir(entrada);
}

void abb_destruir_wrapper(void* abb){
	abb_destruir(abb);
}

void lista_destruir_wrapper(void* lista){
	lista_destruir(lista, entrada_destruir_wrapper);
}

int strcmp_ip_wrapper(const void* ip_1, const void* ip_2){
	return strcmp_ip(ip_2, ip_1);
}

bool print_ips(const char* ip){
	printf("\t%s\n", ip);
	return true;
}

bool print_ips_wrapper(const char* ip, void* ip_dato, void* extra){
	return print_ips(ip);
}

bool print_recursos(const char* recurso, size_t cantidad){
	printf("\t%s - %zu\n", recurso, cantidad);
	return true;
}

int comparar_numeros(int a, int b){
	return b-a;
}

int comparar_numeros_wrapper(const char* a, const char* b){
	char* a_str = (char*) a;
	char* b_str = (char*) b;
	int a_num = atoi(a_str);
	int b_num = atoi(b_str);
	return comparar_numeros(a_num, b_num);
}

int comparar_numeros_wrapper2(const int* a, const int* b){
	return comparar_numeros(*a, *b);
}

int comparar_tiempos_entradas(const char* entrada1, const char* entrada2){
	time_t time_1 = iso8601_to_time(entrada1);
	time_t time_2 = iso8601_to_time(entrada2);
	double time_dif = difftime(time_1, time_2);
	if(time_dif>0){
		return 1;
	} else if(time_dif<0){
		return -1;
	}
	return 0;
}

int comparar_visitado(const visitado_t* a, const visitado_t* b){
	return comparar_numeros_wrapper2(a->numero, b->numero);
}

int comparar_visitado_wrapper(const void* a, const void* b){
	return comparar_visitado(b, a);
}


/*
###################################################################################
INICIALIZACION
###################################################################################
*/
void finalizar_estructuras(hash_t* hash_ips, abb_t* abb_ips, hash_t* hash_entradas){
	hash_destruir(hash_ips);
	abb_destruir(abb_ips);
	hash_destruir(hash_entradas);
}

/*
###################################################################################
INTERFAZ Y FUNCIONES DE COMANDOS
###################################################################################
*/


bool tiempo_enRango(entrada_t* entrada1, entrada_t* entrada2){
	time_t time_1 = iso8601_to_time(entrada1->datetime);
	time_t time_2 = iso8601_to_time(entrada2->datetime);
	double time_dif = difftime(time_1, time_2);
	if (time_dif < RANGODOS){
		return true;
	} return false;
}

bool procesar_archivo(hash_t* hash_ips){
	heap_t* heap_dos = heap_crear(strcmp_ip_wrapper);
	hash_iter_t* iter = hash_iter_crear(hash_ips);
	//Proceso de agregar al heap:
	while(!hash_iter_al_final(iter)){
		cola_t* cola_entradas = cola_crear();
		abb_t* abb_act = hash_obtener(hash_ips, hash_iter_ver_actual(iter));
		abb_iter_t* iter_a = abb_iter_in_crear(abb_act);
		while(!abb_iter_in_al_final(iter_a)){
			entrada_t* entrada_act = abb_obtener(abb_act, abb_iter_in_ver_actual(iter_a));
			if(cola_esta_vacia(cola_entradas)){

			}else if(tiempo_enRango(entrada_act, cola_ver_primero(cola_entradas))){

				if(cola_cantidad(cola_entradas) >= (CANTIDAD_DENTRO_DE_RANGO - 1)){
					heap_encolar(heap_dos, strdup(hash_iter_ver_actual(iter)));
					break;
				}
			} else{
				while((!cola_esta_vacia(cola_entradas)) && (!tiempo_enRango(entrada_act, cola_ver_primero(cola_entradas)))){
					cola_desencolar(cola_entradas);
				}
			}
			cola_encolar(cola_entradas, entrada_act);
			abb_iter_in_avanzar(iter_a);
		}
		cola_destruir(cola_entradas, NULL);
		abb_iter_in_destruir(iter_a);
		hash_iter_avanzar(iter);
	}
	void* desencolado;
	while((desencolado = heap_desencolar(heap_dos))){
		fprintf(stdout, "DoS: %s\n", (char*) desencolado);
		free(desencolado);
	}
	
	heap_destruir(heap_dos, NULL);
	hash_iter_destruir(iter);
	return true;
}



bool agregar_archivo(const char* fnombre, hash_t* hash_ips, hash_t* hash_entradas){
	if(!fnombre) return false;
	FILE* archivo = fopen(fnombre,"r");
	if(!archivo) return false;
	//LIMPIAR HASH
	if(hash_cantidad(hash_ips)){
		hash_limpiar(hash_ips);
	}
	char* linea = NULL;
    size_t capacidad = 0;
    ssize_t leidos;
    while((leidos = getline(&linea,&capacidad,archivo)) > 0){
    	linea[leidos-1] = '\0';
    	if(linea[0] != '\0'){
    		char** datos = split(linea,'\t');
	    	if(!datos[0] || !datos[1] || !datos[2] || !datos[3]) return false; 
	    	char* ip = strdup(datos[0]);
			char* datetime = strdup(datos[1]);
			char* metodo = strdup(datos[2]);
			char* url_recurso = strdup(datos[3]);
			//---Parte del hash_ips----
			entrada_t* entrada = entrada_crear(ip, datetime, metodo, url_recurso);
			if(hash_pertenece(hash_ips, ip)){
				abb_t* abb_ipAct = hash_obtener(hash_ips, ip);
				abb_guardar(abb_ipAct, entrada->datetime, entrada);
			}else{
				abb_t* abb_ipAct = abb_crear(comparar_tiempos_entradas, entrada_destruir_wrapper);
				abb_guardar(abb_ipAct, entrada->datetime, entrada);
				hash_guardar(hash_ips, ip, abb_ipAct);
			}
			//---Parte del hash_entradas---
			if(!hash_pertenece(hash_entradas, url_recurso)){
				int* veces_pedido = malloc(sizeof(int));
				*veces_pedido = 1;
				hash_guardar(hash_entradas, url_recurso, veces_pedido);
			}else{
				int* veces_pedido = hash_obtener(hash_entradas, url_recurso);
				*veces_pedido = *veces_pedido + 1;
			}

	    	free_strv(datos);
	    }

    }
    free(linea);
    fclose(archivo);
    return true;
}


bool abb_ips_llenar(hash_t* hash_ips, abb_t* abb_ips){
	hash_iter_t* iter = hash_iter_crear(hash_ips);
	while(!hash_iter_al_final(iter)){
		if(abb_pertenece(abb_ips, hash_iter_ver_actual(iter))){
			hash_iter_avanzar(iter);
			continue;
		}
		if(!abb_guardar(abb_ips, hash_iter_ver_actual(iter), NULL)) return false;
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return true;
}

bool ver_visitantes(char* ip_inicial, char* ip_final, abb_t* abb_ips){
	printf("Visitantes:\n");
	abb_iter_t* iter = abb_iter_in_crear(abb_ips);
	while(!abb_iter_in_al_final(iter) && !(strcmp_ip(abb_iter_in_ver_actual(iter), ip_inicial)>=0)){
		abb_iter_in_avanzar(iter);
	}
	while(!abb_iter_in_al_final(iter) && !(strcmp_ip(abb_iter_in_ver_actual(iter), ip_final)>0)){
		printf("\t%s\n", abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	return true;
}

char* obtener_recurso(hash_t* hash_ips, hash_iter_t* iter){
	char* clave = (char*) hash_iter_ver_actual(iter);
	abb_t* abb_ipAct = hash_obtener(hash_ips, (const char*)clave);
	entrada_t* entrada_ipAct = abb_obtener(abb_ipAct, (const char*)clave);
	char* recurso = entrada_ipAct->url_recurso;
	return recurso;
}
/*
bool abb_recursos_llenar(hash_t* hash_ips, abb_t* abb_recursos, size_t cantidad){
	if(!hash_ips || hash_cantidad(hash_ips) == 0) return false;
	if(cantidad > hash_cantidad(hash_ips)){
		cantidad = hash_cantidad(hash_ips);
	}
	char* recurso_ant;
	char* recurso_act;
	size_t cont;
	for(size_t i = 0; i < cantidad; i++){
		hash_iter_t* iter = hash_iter_crear(hash_ips);
		recurso_ant = obtener_recurso(hash_ips, iter);
		cont = 0;
		while(!hash_iter_al_final(iter)){
			recurso_act = obtener_recurso(hash_ips, iter);
			if(strcmp(recurso_act, recurso_ant) == 0){
				cont++;
			}
			hash_iter_avanzar(iter);
		}
		if(!abb_guardar(abb_recursos, (const char*)recurso_ant, &cont)) return false;
		hash_iter_destruir(iter);
	}	
	return true;
}
*/
bool ver_mas_visitados(hash_t* hash_entradas, size_t cant){
	printf("Sitios más visitados:\n");
	hash_iter_t* iter = hash_iter_crear(hash_entradas);
	heap_t* heap = heap_crear(comparar_visitado_wrapper);
	while(!hash_iter_al_final(iter)){
		visitado_t* visitado = crear_visitado(hash_obtener(hash_entradas, hash_iter_ver_actual(iter)), hash_iter_ver_actual(iter));
		heap_encolar(heap, visitado);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	for(int i = 0; i<cant; i++){
		visitado_t* visitado_act = heap_desencolar(heap);
		if(visitado_act){
			printf("\t%s - %d\n", visitado_act->recurso,  *visitado_act->numero);
			visitado_destruir(visitado_act);
		}
	}
	heap_destruir(heap, free);
	return true;
}

int ejecutar_interfaz(hash_t* hash_ips, abb_t* abb_ips, hash_t* hash_entradas){
	char* linea = NULL;
	size_t capacidad = 0;
	ssize_t leidos;
	while((leidos = getline (&linea, &capacidad, stdin)) > 0){
		if(strcmp(linea,"\n") == 0){
			free(linea);
			return 1;
		}
		linea[leidos-1] = '\0';
		char** datos = split(linea,' ');
		if(strcmp(datos[0], "agregar_archivo") == 0){
			if(!datos[1]){
				fprintf(stderr, "Error en comando agregar_archivo\n");
				free_strv(datos);
				free(linea);
				return 1;
			}
			char* fnombre = datos[1];
			if(!agregar_archivo(fnombre, hash_ips, hash_entradas)){
				fprintf(stderr, "Error en comando agregar_archivo\n");
				free_strv(datos);
				free(linea);
				return 1;
			}
			if(!procesar_archivo(hash_ips)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "ver_visitantes") == 0){
			if(!datos[1] || !datos[2]){
				fprintf(stderr, "Error en comando %s\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			char* ip_inicial = datos[1];
			char* ip_final = datos[2];
			if(!abb_ips_llenar(hash_ips, abb_ips)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			if(!ver_visitantes(ip_inicial, ip_final, abb_ips)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "ver_mas_visitados") == 0){
			if(!datos[1]){
				fprintf(stderr, "Error en comando %s\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			size_t cantidad = atoi(datos[1]);
			if(!ver_mas_visitados(hash_entradas, cantidad)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}else{
			fprintf(stderr, "Error en comando %s\n",datos[0]);
			free_strv(datos);
			free(linea);
			return 1;
		}
	}
	free(linea);
	return 1;
}

/*
#######################################################################
MAIN
#######################################################################
*/

int main (int argc, char* argv[]){
	hash_t* hash_ips = hash_crear(abb_destruir_wrapper);
	hash_t* hash_entradas = hash_crear(free);
	abb_t* abb_ips = abb_crear(strcmp_ip, NULL);
	/*abb_t* abb_recursos = abb_crear(comparar_numeros_wrapper, entrada_destruir_wrapper);*/
	while(ejecutar_interfaz(hash_ips, abb_ips, hash_entradas) == 0);
	finalizar_estructuras(hash_ips, abb_ips, hash_entradas);
	return 0;
}





