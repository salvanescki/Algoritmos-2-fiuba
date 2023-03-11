#define _POSIX_C_SOURCE 200809L //GETLINE

#include "pila.h"
#include "lista.h"
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TAM_MIN 100

typedef struct usuario {
	size_t id;
	char* coordenadas;
	double saldo;
} usuario_t;

typedef struct vector_usuarios{
	usuario_t** usuarios;
	size_t tam;
	size_t cant;
} vector_usuarios_t;

typedef struct pago {
	size_t id;
	char* codigo;
	double monto;
} pago_t;

/*
###########################################################
PRIMITIVAS DE PAGOS
###########################################################
*/

pago_t* pago_crear(size_t id, double monto, char* codigo){
	pago_t* pago = malloc(sizeof(pago_t));
	if (pago == NULL) return NULL;
	pago->id = id;
	pago->codigo = codigo;
	pago->monto = monto;
	return pago;
}

void pago_destruir(pago_t* pago){
	free(pago);
}

/*
###########################################################
PRIMITIVAS DE USUARIO
###########################################################
*/

usuario_t* usuario_crear(size_t id, double saldo, char* coordenadas){
	usuario_t* usuario = malloc(sizeof(usuario_t));
	if (usuario == NULL) return NULL;
	usuario->id = id;
	usuario->saldo = saldo;
	usuario->coordenadas = coordenadas;
	return usuario;
}

void usuario_destruir(usuario_t* usuario){
	free(usuario);
}

/*
###########################################################
PRIMITIVAS DE VECTOR USUARIO
###########################################################
*/
vector_usuarios_t* vector_usuarios_crear(size_t tam){
	vector_usuarios_t* vector_usuarios = malloc(sizeof(vector_usuarios_t));
	if(vector_usuarios == NULL) return NULL;
	vector_usuarios->usuarios = malloc(sizeof(usuario_t) * tam);
	if(vector_usuarios->usuarios == NULL) return NULL;
	vector_usuarios->tam = tam;
	vector_usuarios->cant = 0;
	return vector_usuarios;
}

bool vector_usuarios_redimensionar(vector_usuarios_t* vector_usuarios, size_t tam_nuevo) {
    usuario_t** datos_nuevo = realloc(vector_usuarios->usuarios, tam_nuevo * sizeof(usuario_t));
    if(datos_nuevo == NULL) return false;
    vector_usuarios->usuarios = datos_nuevo;
    vector_usuarios->tam = tam_nuevo;
    return true;
}

bool vector_usuarios_guardar(vector_usuarios_t* vector_usuarios, usuario_t* usuario){
 	if (vector_usuarios->cant == vector_usuarios->tam){
 		vector_usuarios_redimensionar(vector_usuarios, 2*vector_usuarios->tam);
 	}
    vector_usuarios->usuarios[usuario->id] = usuario;
    vector_usuarios->cant++;
    return true;
}

void vector_usuarios_destruir(vector_usuarios_t* vector_usuarios){
	for(size_t i = 0; i<vector_usuarios->cant; i++){
		usuario_destruir(vector_usuarios->usuarios[i]);
	}
	free(vector_usuarios->usuarios);
	free(vector_usuarios);
}

/*
###########################################################
AUXILIARES
###########################################################
*/

void pago_destruir_wrapper(void* pago){
	pago_destruir((pago_t*)pago);
}

/*
###################################################################################
INICIALIZACION
###################################################################################
*/

bool procesar_archivo_inicial(const char* fnombre, vector_usuarios_t* lista_de_usuarios){
	if(!fnombre) {
		printf("no detecta el archivo\n");
		return false;
	}
	FILE* archivo = fopen(fnombre,"r");
    if (archivo == NULL) {
    	printf("no pudo abrir el archivo\n");
    	return false;
    }
    
    char* linea = NULL; 
    size_t capacidad = 0; 
    ssize_t leidos;
    size_t i = 0;
    while((leidos = getline(&linea,&capacidad,archivo)) > 0){
    	linea[leidos-1] = '\0';
    	char** datos = split(linea,',');
    	if(!datos[0] || !datos[1] || !datos[2]) return false;
    	size_t id = atoi(datos[0]);
    	double monto = atof(datos[1]);
    	char* coordenadas = datos[2];
    	usuario_t* usuario = usuario_crear(id, monto, coordenadas);
    	vector_usuarios_guardar(lista_de_usuarios, usuario);
    	free_strv(datos);
    	i++;
    }
    free(linea);
    fclose(archivo);
    return true;
}

/*
###################################################################################
LENGUAJE DE PILA
###################################################################################
*/
void pagar(size_t emisor, size_t receptor, double monto, vector_usuarios_t* lista_de_usuarios){
	lista_de_usuarios->usuarios[emisor]->saldo = lista_de_usuarios->usuarios[emisor]->saldo - monto;
	lista_de_usuarios->usuarios[receptor]->saldo = lista_de_usuarios->usuarios[receptor]->saldo + monto;
}


bool procesar_codigo(char* codigo, pila_t* pila_pago, vector_usuarios_t* lista_de_usuarios){
	printf("codigo antes de imprimir:%s\n", codigo);
	char** instrucciones = split(codigo, ';');
	size_t i = 0;
	size_t emisor_id = 0;
	char* emisor_coordenadas;
	double emisor_monto = 0.0;
	size_t receptor_id = 0;
	while(instrucciones[i] == NULL){
		if(strcmp(instrucciones[i], "validar_usuario") == 0){
			char* coordenadas = pila_desapilar(pila_pago);
			char* id = pila_desapilar(pila_pago);
			emisor_id = atoi(id);
			emisor_coordenadas = coordenadas;
			if(emisor_id > lista_de_usuarios->cant) return false;
			if(strcmp(lista_de_usuarios->usuarios[emisor_id]->coordenadas, emisor_coordenadas) != 0) return false;
		}else if(strcmp(instrucciones[i], "validar_pago") == 0){
			char* monto = pila_desapilar(pila_pago);
			char* act_id = pila_desapilar(pila_pago);
			emisor_monto = atof(monto);
			size_t id_actual = atoi(act_id);
			if(emisor_id != id_actual) return false;
			if(lista_de_usuarios->usuarios[emisor_id]->saldo < emisor_monto) return false;
		}else if(strcmp(instrucciones[i], "pagar") == 0){
			char* act_emisor_id = pila_desapilar(pila_pago);
			size_t id_actual_emisor = atoi(act_emisor_id);
			if(emisor_id != id_actual_emisor) return false;
			char* act_receptor_id = pila_desapilar(pila_pago);
			receptor_id = atoi(act_receptor_id);
			char* monto = pila_desapilar(pila_pago);
			double act_monto = atof(monto);
			if(emisor_monto != act_monto) return false;
			pagar(emisor_id, receptor_id, emisor_monto, lista_de_usuarios);

		}else{
			pila_apilar(pila_pago, instrucciones[i]);
		}
		i++;
	}
	free_strv(instrucciones);
	return true;
}

/*
########################################################################
FINALIZAR PROGRAMA
########################################################################
*/
void finalizar (vector_usuarios_t* lista_de_usuarios, lista_t* cola_procesamiento, pila_t* pila_pago){
	vector_usuarios_destruir(lista_de_usuarios);
	lista_destruir(cola_procesamiento, pago_destruir_wrapper);
	pila_destruir(pila_pago);
}

/*
###################################################################################
INTERFAZ Y FUNCIONES DE COMANDOS
###################################################################################
*/

bool agregar_pago(size_t id, double monto, char* codigo, lista_t* cola_procesamiento){
	pago_t* pago = pago_crear(id,monto,codigo);
	lista_insertar_ultimo(cola_procesamiento,pago);
	return true;
}

bool pagos_pendientes(lista_t* cola_procesamiento){
	fprintf(stdout, "Cantidad de pagos pendientes: %zu\n", lista_largo(cola_procesamiento));
	if(lista_largo(cola_procesamiento) == 0) return true;
	double acum = 0;
	pago_t* pago_act;
	lista_iter_t* iter_cola = lista_iter_crear(cola_procesamiento);
	while(!lista_iter_al_final(iter_cola)){
		pago_act = lista_iter_ver_actual(iter_cola);
		acum += pago_act->monto;
		lista_iter_avanzar(iter_cola);
	}
	lista_iter_destruir(iter_cola);
	fprintf(stdout, "Montos sumados: %.3f\n", acum);
	return true;
}

bool procesar_pagos(lista_t* cola_procesamiento, pila_t* pila_pago, vector_usuarios_t* lista_de_usuarios, size_t cant_pagos){
	if(cant_pagos > lista_largo(cola_procesamiento)){
		fprintf(stderr, "Error en comando procesar, numero max de pagos a procesar superado\n");
		return false;
	}
	for(size_t i = 1; i <= cant_pagos; i++){
		pago_t* pago_act = lista_borrar_primero(cola_procesamiento);
		char* codigo = pago_act->codigo;
		if(!procesar_codigo(codigo, pila_pago, lista_de_usuarios)){ 
			pago_destruir(pago_act);
			fprintf(stderr, "Error en pago %zu\n", pago_act->id);
			return false;
		}
		pago_destruir(pago_act);
	}
	return true;

}

bool guardar_cuentas(vector_usuarios_t* lista_de_usuarios, char* fnombre){
	FILE* f_cuentas_out = fopen(fnombre,"w");
	if(f_cuentas_out == NULL) return false;
	for(size_t i = 0; i < lista_de_usuarios->cant;i++){
		fprintf(f_cuentas_out, "usuario:%zu saldo:%.3f\n", lista_de_usuarios->usuarios[i]->id, lista_de_usuarios->usuarios[i]->saldo);
	}
	fclose(f_cuentas_out);
	return true;
}

int ejecutar_interfaz(vector_usuarios_t* lista_de_usuarios, lista_t* cola_procesamiento, pila_t* pila_pago){
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
		if(strcmp(datos[0], "agregar_pago") == 0){
			if(!datos[1] || !datos[2] || !datos[3]){
				fprintf(stderr, "Error en comando %s, faltan parametros\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			if(datos[4]){
				fprintf(stderr, "Error en comando %s, sobran parametros\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			size_t id_pago = atoi(datos[1]);
			double monto = atof(datos[2]);
			char* codigo_pago = datos[3];
			if(!agregar_pago(id_pago, monto, codigo_pago,cola_procesamiento)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "pagos_pendientes") == 0){
			if(!pagos_pendientes(cola_procesamiento)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "procesar") == 0){
			if(!datos[1]){
				fprintf(stderr, "Error en comando %s, faltan parametros\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			size_t cant_pagos = atoi(datos[1]);
			if(!procesar_pagos(cola_procesamiento, pila_pago, lista_de_usuarios, cant_pagos)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "guardar_cuentas") == 0){
			if(!datos[1]){
				fprintf(stderr, "Error en comando %s, faltan parametros\n",datos[0]);
				free_strv(datos);
				free(linea);
				return 1;
			}
			char* fnombre = datos[1];
			if(!guardar_cuentas(lista_de_usuarios,fnombre)){
				free_strv(datos);
				free(linea);
				return 1;
			}
			free_strv(datos);
			free(linea);
			fprintf(stdout, "OK\n");
			return 0;
		}
		if(strcmp(datos[0], "finalizar") == 0){
			fprintf(stdout, "OK\n");
			free(linea);
			free_strv(datos);
			return 1;
		}else{
			fprintf(stderr, "Error en comando %s, no existe\n",datos[0]);
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
	vector_usuarios_t* lista_de_usuarios = vector_usuarios_crear(TAM_MIN);
	const char* nombre_del_archivo = argv[1];
	if(!procesar_archivo_inicial(nombre_del_archivo, lista_de_usuarios)){
		fprintf(stderr,"Error con el archivo stdin\n");
		vector_usuarios_destruir(lista_de_usuarios);
		return 0;
	}
	lista_t* cola_procesamiento = lista_crear();
	pila_t* pila_pago = pila_crear();
	while(ejecutar_interfaz(lista_de_usuarios, cola_procesamiento, pila_pago) == 0);
	finalizar(lista_de_usuarios, cola_procesamiento, pila_pago);
	return 0;
}