#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"

typedef struct{
    char* id;
    char* Nombre;
    int PC;
    int PS; 
    char Sexo;
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

Pokemon* CreatePokemon(char* id, char* Nombre, int PC, int PS, char Sexo){
    Pokemon* pokemon;
    pokemon = (Pokemon*) malloc(sizeof(Pokemon));
    pokemon->id = id;
    strcpy(pokemon->Nombre, Nombre);
    pokemon->PC = PC;
    pokemon->PS = PS;
    pokemon->Sexo = Sexo;
    pokemon->Estado = 1; // Estado 1 refiere a que esta activo y 0 a inactivo
    return pokemon;
}

Pokedex* CreatePokedex(char* Nombre, char** tipos, char* EPrevia, char* EPosterior, int idPokedex, char * region, int cantidadTipos){
    Pokedex* pokedex;
    pokedex = (Pokedex*)malloc(sizeof(Pokedex));
    strcpy(pokedex->Nombre, Nombre);
    pokedex->Cantidad = 1;
    pokedex->tipos = tipos;
    pokedex->cantidadTipos = cantidadTipos;
    strcpy(pokedex->EPrevia, EPrevia);
    strcpy(pokedex->EPosterior, EPosterior);
    pokedex->idPokedex = idPokedex;
    strcpy(pokedex->region, region);
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
    arregloAux = searchMap(map,region->region);
    if(!arregloAux){
        Pokemon** arreglo;
        arreglo = (Pokemon**) malloc(100*sizeof(Pokemon*));
        arreglo[0] = poke;
        insertMap(map,region->region, arreglo);
    }else{
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
    insertMap(map,pokemon->id,pokemon);
}

void InsertarPokemon(HashMap** maps, Pokemon* pokemon, Pokedex* pokedex){
    AgregarPokemonid(maps[0],pokemon);
    AgregarPokeNombre(maps[1],pokemon);
    AgregarPokeRegion(maps[2], pokemon, pokedex);
    AgregarPokePC(maps[3],pokemon);
    AgregarPokeTipos(maps[4], pokemon, pokedex);
    AgregarPokedexNombre(maps[5],pokedex);
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
            char* auxtoken;
            char* nombre;
            char* id;
            char* PC;
            char* PS; 
            char Sexo;
            char** tipos; 
            char* EPrevia;
            char* EPosterior;
            char* idPokedex;
            char* region;
            int cantidadTipos;
            tipos = (char**)malloc(10*sizeof(char*));
            token = strtok(line,",");
            id = token;
            token = strtok(NULL,",");
            nombre = token;
            token = strtok(NULL,"\"");
            token = strtok(NULL,"\"");
            if(token!=NULL){
                cantidadTipos = 0;
                auxtoken = token;
                auxtoken = strtok(NULL,",");
                for(int i=0; auxtoken!=NULL; i++){
                    tipos[i] = auxtoken;
                    auxtoken = strtok(NULL,",");
                    cantidadTipos=i;
                }
                cantidadTipos++;
                token =  strtok(NULL,",");
            }else{
                token = strtok(NULL,",");
                tipos[0] = token;
                cantidadTipos = 1; 
            }
            token = strtok(NULL,",");
            PC = token;
            token = strtok(NULL,",");
            PS = token;
            token = strtok(NULL,",");
             printf("aweonao1\n");
            Sexo = token[0];
             printf("aweona2\n");
            token = strtok(NULL,",");
            EPrevia = token;
             printf("aweona3\n");
            token = strtok(NULL,",");
            EPosterior = token;
             printf("aweona4\n");
            token = strtok(NULL,",");
            idPokedex = token;
             printf("aweona5\n");
            token = strtok(NULL,",");
            region = token;
             printf("aweona6\n");
            region = strtok(region,"\n");
            Pokemon* pokemon;
            Pokedex* pokedex;
            printf("aweonao\n");
            pokemon = CreatePokemon(id, nombre,atoi(PC),atoi(PS),Sexo);
            printf("aweonao2\n");
            pokedex = CreatePokedex(nombre,tipos,EPrevia,EPosterior,atoi(idPokedex),region,cantidadTipos);
            InsertarPokemon(maps, pokemon, pokedex);
        }
        fclose(archivo);
        printf("----------------------------------\n");
        printf("%10s\n","Archivo leido con exito!");
        printf("----------------------------------\n");
    }
}
