#include "strutil.h"
#include <stdlib.h>
#include <string.h>

size_t cant_palabras(const char* str, char sep){
	size_t cont = 1;
	size_t i = 0;
	while(str[i] != '\0'){
		if(str[i] == sep){
			cont++;
		}
		i++;
	}
	return cont;
}

char* str_obtener(const char* str, size_t pos_i, size_t pos_f){
	size_t longitud = pos_f - pos_i;
	char* string = malloc(sizeof(char)* longitud+1);
	for (size_t i = 0; i < longitud; i++){
		string[i] = str[pos_i + i];
	}
	string[longitud] = '\0';
	return string;
}

char** split(const char* str, char sep){
	if (str[0]=='\0') return NULL;
	if (sep == '\0') return NULL;
	size_t j = 0;
	size_t pos_i = 0;
	char** strv = malloc((sizeof(char*) * cant_palabras(str, sep)) + sizeof(void*));
	if(strv == NULL) return NULL;
	for(size_t i = 0; i <= strlen(str) ; i++){
		if(str[i] == sep){
			strv[j] = str_obtener(str, pos_i, i);
			pos_i = i + 1;
			j++;
		}
		if(i == strlen(str)){
			if(str[i] == '\0'){
				strv[j] = str_obtener(str, pos_i, i);
			}else{
				strv[j] = str_obtener(str, pos_i, i + 1);
			}
		}
	}
	strv[j+1] = NULL;
	return strv;
}

void free_strv(char* strv[]){
	size_t i = 0;
	while(strv[i] != NULL){
		free(strv[i]);
		i++;
	}
	free(strv);
}
