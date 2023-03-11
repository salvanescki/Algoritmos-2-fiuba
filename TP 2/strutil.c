#include "strutil.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


char* get_palabra(const char* str, size_t pos_i, size_t pos_f){

	char* palabra = malloc(sizeof(char)*(pos_f-pos_i+1));
	for (int i=0; i<(pos_f-pos_i); i++){
		palabra[i] = str[pos_i+i];
	}
	palabra[pos_f-pos_i] = '\0';
	return palabra;	
}

size_t cant_palabras(const char *str, char sep){
	size_t cant_palabras = 1;
	for (int i=0; str[i] != '\0' ; i++){
        if (str[i] == sep) cant_palabras++;
    }
    return cant_palabras;
}

size_t mystrlen(const char *str){

    size_t len = 0;
    
    for (int i=0; str[i] != 0; i++) 
    {
        len++;
    }
    return(len);
}

char** split(const char* str, char sep){

	//Casos especiales
	if (sep == '\0') return NULL;
	//Inicializacion
	char **array = malloc(sizeof(char*)*(cant_palabras(str, sep)+1));
	size_t len = mystrlen(str);
	size_t pos_i = 0;
	size_t pos_f = 0;
	size_t pos_array = 0;
	//Split
	if (!len) array[pos_array] = get_palabra(str,pos_i,pos_f);
	for (int i=0; i<len; i++){
		if (str[i] == sep){
			array[pos_array] = get_palabra(str,pos_i,pos_f);
			pos_array++;
			pos_i = pos_f+1;
		}
		if (i == len-1){
			array[pos_array] = get_palabra(str,pos_i,pos_f+1);
		}
		pos_f++;
	}
	array[pos_array+1] = NULL;
	return array;
}

void free_strv(char* strv[]){
	for (size_t i=0; strv[i] ;i++){
		free(strv[i]);
	}
	free(strv);
}
