#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "HashMap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
	if(!key) exit(-1);
	long posicion;
	Pair * bucket;
	bucket = createPair(key, value);
	posicion=hash(key, map->capacity);
	while(map->buckets[posicion]!=NULL){
		posicion++;
		if(posicion == map->capacity) posicion = 0;
	}
	map->buckets[posicion] = bucket;
	map->size++;
	map->current = posicion;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
	Pair ** aux;
	long tam;
	tam = map->capacity*2;
	aux = map->buckets;
	map->buckets = (Pair **) calloc(tam,sizeof(Pair*));
	map->capacity = tam;
	for(int i=0; i<tam/2; i++){
		if(aux[i]!=NULL && aux[i]->key!=NULL){
			insertMap(map, aux[i]->key, aux[i]->value);
		}
	}
}


HashMap * createMap(long capacity) {
	HashMap * map;
	map = (HashMap *) malloc(sizeof(HashMap));
	map->current = -1;
	map->capacity = capacity;
	map->size = 0;
	map->buckets = (Pair **) calloc(capacity,sizeof(Pair*));
	return map;
}

void eraseMap(HashMap * map,  char * key) {  
	int posicion;
	posicion = hash(key, map->capacity);  
	for(int i=posicion; i<map->capacity; i++){
		if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL && strcmp(map->buckets[i]->key, key) == 0){
			map->current = i;
			map->size--;
			map->buckets[i]->key = NULL;
		}
	}
}

void * searchMap(HashMap * map,  char * key) {
	int posicion;
	posicion = hash(key, map->capacity);   
	for(int i=posicion; i<map->capacity; i++){
		if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL && strcmp(map->buckets[i]->key, key) == 0){
			map->current = i;
			return map->buckets[i]->value;
		}
	}
	return NULL;
}

void * firstMap(HashMap * map) {
for(int i=0; i<map->capacity; i++){
		if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
			map->current = i;
			return map->buckets[i]->value;
		}
	}
	return NULL;
}

void * nextMap(HashMap * map) {
for(int i=map->current+1; i<map->capacity; i++){
		if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
			map->current = i;
			return map->buckets[i]->value;
		}
	}
	return NULL;
}

long capacity(HashMap * map){
	return map->capacity;
}
