#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

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
 int posicion = hash(key,map->capacity);
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){
    if(is_equal(map->buckets[posicion]->key, key))
      break;
    posicion = (posicion + 1)% map->capacity ;
  }
if((float) ( (map->size) / (map->capacity) )>= 0.7)
    enlarge(map);
  map->buckets[posicion] = createPair(key,value);
  map->size ++;

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** old_buckets = map -> buckets;
  map->capacity*=2;
  Pair ** nuevo = calloc(sizeof(Pair *) , map->capacity);
  map->buckets = nuevo;
  for (int i = 0; i <map->capacity/2 ; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }
  

}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  map->capacity = capacity;
  map->size = 0;
  map->current=-1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  Pair* pair=searchMap(map,key);
  if (pair != NULL) {
        pair->key = NULL;
        map->size--;
    }

}

Pair * searchMap(HashMap * map,  char * key) {   
  int posicion = hash(key,map->capacity);
  while (map->buckets[posicion] != NULL) {
        if (is_equal(map->buckets[posicion]->key, key)) {
            map->current = posicion;
            return map->buckets[posicion];
        }
        posicion = (posicion + 1) % map->capacity;
        if (posicion == hash(key, map->capacity))
            break;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
  for (int i = 0; i < map->capacity; i++){
    if(map->buckets[i]!= NULL && map->buckets[i]->key != NULL){
       map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for (int i = map->current + 1; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
  return NULL;
}
