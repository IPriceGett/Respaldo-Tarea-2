#ifndef POKEMON_H
#define POKEMON_H

typedef struct Pokemon Pokemon;

typedef struct Pokedex Pokedex;

Pokemon* CreatePokemon(int id, char* Nombre, int PC, int PS, char* Sexo);

Pokedex* CreatePokedex(char* Nombre, char** tipos, char* EPrevia, char* EPosterior, int idPokedex, char * region, int cantidadTipos);

HashMap** initializeMapsPokemon();

void InsertarPokemon(HashMap** maps, Pokemon* pokemon, Pokedex* pokedex);

void LiberarPokemon(HashMap** maps, char* id);

void LeerArchivo(HashMap** maps, char* nombreArchivo);

#endif /* POKEMON_H */
