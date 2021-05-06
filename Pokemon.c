#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "HashMap.h"

typedef struct{
    char* id;
    char* Nombre;
    int PC;
    int PS; 
    char* Sexo;
    int Estado; // se agrega estado para eliminado logico
}Pokemon;

typedef struct{
    char* Nombre;
    int Cantidad;
    char** tipos; 
    char* EPrevia;
    char* EPosterior;
    int idPokedex;
    char* region;
    int cantidadTipos;
}Pokedex;

HashMap** initializeMapsPokemon(){
    HashMap ** aux;
    aux = (HashMap**)malloc(6*sizeof(HashMap*));
    for(int i=0;i<6;i++){
        aux[i] = createMap(191);
    }
    return aux;
}

Pokemon* CreatePokemon(char* id, char* Nombre, int PC, int PS, char* Sexo){
    Pokemon* pokemon;
    pokemon = (Pokemon*) malloc(sizeof(Pokemon));
    pokemon->id = id;
    pokemon->Nombre = Nombre;
    pokemon->PC = PC;
    pokemon->PS = PS;
    pokemon->Sexo=Sexo;
    pokemon->Estado = 1; // Estado 1 refiere a que esta activo y 0 a inactivo
    return pokemon;
}

Pokedex* CreatePokedex(char* Nombre, char** tipos, char* EPrevia, char* EPosterior, int idPokedex, char * region, int cantidadTipos){
    Pokedex* pokedex;
    pokedex = (Pokedex*)malloc(sizeof(Pokedex));
    pokedex->Nombre=Nombre;
    pokedex->Cantidad = 1;
    pokedex->tipos = tipos;
    pokedex->cantidadTipos = cantidadTipos;
    pokedex->EPrevia=EPrevia;
    pokedex->EPosterior=EPosterior;
    pokedex->idPokedex = idPokedex;
    pokedex->region= region;
    return pokedex;
}

void DesactivarPoke(HashMap* pokedex, HashMap* map, int id){
    Pokemon* pokemon;
    Pokedex* poke;
    pokemon = searchMap(map, (char *)id);
    poke = searchMap(map,pokemon->Nombre);
    pokemon->Estado = 0;
    poke->Cantidad--;
}

void AgregarPokedexNombre(HashMap* pokedex, Pokedex* poked){
    Pokedex* poke;
    poke = searchMap(pokedex,poked->Nombre);
    if(!poke){
        insertMap(pokedex, poked->Nombre, poked);
    }else{
        poke->Cantidad++;
    }
}

void AgregarPokeNombre(HashMap* map, Pokemon* poke){
    Pokemon** arregloAux;
    arregloAux = searchMap(map,poke->Nombre);
    if(!arregloAux){
        Pokemon** arreglo;
        arreglo = (Pokemon**) malloc(100*sizeof(Pokemon*));
        arreglo[0] = poke;
        insertMap(map,poke->Nombre, arreglo);
    }else{
        for(int j=0;j<100;j++){
            if(arregloAux[j] == NULL){
                arregloAux[j] = poke;
                j=100;
            }
        }
    }
}

void AgregarPokePC(HashMap* map, Pokemon* poke){
    Pokemon** arregloAux;
    arregloAux = searchMap(map,"Puntos");
    if(!arregloAux){
        Pokemon** arreglo;
        arreglo = (Pokemon**) malloc(100*sizeof(Pokemon*));
        arreglo[0] = poke;
        insertMap(map,"Puntos", arreglo);
    }else{
        for(int j=0;j<100;j++){
            if(arregloAux[j] == NULL){
                arregloAux[j] = poke;
                j=100;
            }
        }
    }
}

void AgregarPokeRegion(HashMap* map, Pokemon* poke,Pokedex* region){
    Pokemon** arregloAux;
    printf("loooooooool");
    if(!searchMap(map,region->region)){
        Pokemon** arreglo;
        arreglo = (Pokemon**) malloc(100*sizeof(Pokemon*));
        arreglo[0] = poke;
        insertMap(map,region->region, arreglo);
    }else{
        arregloAux = searchMap(map,region->region);
        for(int j=0;j<100;j++){
            if(arregloAux[j] == NULL){
                arregloAux[j] = poke;
                j=100;
            }
        }

    }
}

void AgregarPokeTipos(HashMap* map, Pokemon* poke,Pokedex* tipos){
    Pokemon** arregloAux;
    char** tipo;
    tipo= tipos->tipos;
    for(int i=0;i<tipos->cantidadTipos;i++){
        arregloAux = searchMap(map,tipo[i]);
        if(!arregloAux){
            Pokemon** arreglo;
            arreglo = (Pokemon**) malloc(100*sizeof(Pokemon*));
            arreglo[0] = poke;
            insertMap(map,tipo[i], arreglo);
        }else{
            for(int j=0;j<100;j++){
                if(arregloAux[j] == NULL || arregloAux[j]->Estado == 0){
                    arregloAux[j] = poke;
                    j=100;
                }
            }
        }
    }
}

int LastID(HashMap* map){
    int id;
    id=0;
    Pokemon** arregloAux;
    arregloAux = searchMap(map,"Puntos");
    for(int i=0;i<100;i++){
        if(arregloAux[i] != NULL && id<atoi(arregloAux[i]->id)){
           id = atoi(arregloAux[i]->id); 
        }
    }
    return id++;
}
void AgregarPokemonid(HashMap* map, Pokemon * pokemon){
    if(pokemon->id ==NULL){
        pokemon->id = (char*)LastID(map);
    }
    printf("%s\n",pokemon->id);
    insertMap(map,pokemon->id,pokemon);
}

void InsertarPokemon(HashMap** maps, Pokemon* pokemon, Pokedex* pokedex){
     printf("0");
    AgregarPokemonid(maps[0],pokemon);
     printf("1");
    AgregarPokeNombre(maps[1],pokemon);
     printf("2");
    AgregarPokeRegion(maps[2], pokemon, pokedex);
     printf("3");
    AgregarPokePC(maps[3],pokemon);
     printf("4");
    AgregarPokedexNombre(maps[5],pokedex);
     printf("5");
    AgregarPokeTipos(maps[4], pokemon, pokedex);
    printf("mia5");
}

void LiberarPokemon(HashMap** maps, char* id){
    Pokemon* pokemonLiberar;
    pokemonLiberar = searchMap(maps[0], id);
    if(pokemonLiberar!= NULL){
        pokemonLiberar->Estado = 0;
        Pokedex* pokedex;
        pokedex = searchMap(maps[5],pokemonLiberar->Nombre);
        pokedex->Cantidad--;
    }
}

void LeerArchivo(HashMap** maps, char* nombreArchivo){
    FILE * archivo;
    archivo = fopen(nombreArchivo, "r");
    if(!archivo){
        printf("----------------------------------\n");
        printf("%10s\n","No se pudo encontrar el archivo");
        printf("----------------------------------\n");
    }
    else{
        char line[100];
        fgets(line,100,archivo);
        while(fgets(line,100,archivo)){
            char* token;
            char*token2;
            char token3[40];
            char* nombre;
            char* id;
            char* PC;
            char* PS; 
            char* Sexo;
            char** tipos; 
            char* EPrevia;
            char* EPosterior;
            char* idPokedex;
            char* region;
            int cantidadTipos;
            bool uno;
            uno=false;
            token2 = (char*)malloc(30*sizeof(char));
            tipos = (char**)malloc(10*sizeof(char*));
            token = strtok(line,"\n");
            token = strtok(line,",");
            id = token;
            token = strtok(NULL,",");
            nombre = token;
            token = strtok(NULL,"\"");
            token2 = token;
            printf("%d",strlen(token2));
            if(strlen(token2)>30){
                token = strtok(token,",");
                tipos[0] = token;
                uno = true;
                cantidadTipos =1;
            }
            token = strtok(NULL,",");
            PC = token;
            token = strtok(NULL,",");
            PS = token;
            token = strtok(NULL,",");
            Sexo = token;
            token = strtok(NULL,",");
            EPrevia = token;
            token = strtok(NULL,",");
            EPosterior = token;
            token = strtok(NULL,",");
            idPokedex = token;
            token = strtok(NULL,",");
            region = token;
            if(uno==false){
                token2 = strtok(token2,",");
                tipos[0] = token2;
                token2 = strtok(NULL," ");
                tipos[1] = token2;
                cantidadTipos =2;
            }
            Pokemon* pokemon;
            Pokedex* pokedex;
            int i;
            int j;
            i =atoi(PC);
            j=atoi(PS);
            int numerito;
            pokemon = CreatePokemon(id, nombre, i,j,Sexo);
            numerito = atoi(idPokedex);
            pokedex = CreatePokedex(nombre,tipos,EPrevia,EPosterior,numerito,region,cantidadTipos);
            printf("aweonao3\n");
            InsertarPokemon(maps, pokemon, pokedex);
            printf("aweonao1\n");
        }
        fclose(archivo);
        printf("----------------------------------\n");
        printf("%10s\n","Archivo leido con exito!");
        printf("----------------------------------\n");
    }
}
