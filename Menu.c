#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HashMap.h"
#include "Pokemon.h"

void case1(HashMap** maps){
    char* nombreArchivo;
    nombreArchivo = (char*)malloc(25*sizeof(char));
    printf("Ingrese el nombre de la wea de archivo: \n");
    fgets(nombreArchivo,25,stdin);
    nombreArchivo = strtok(nombreArchivo, "\n");
    printf("%s",nombreArchivo);
    LeerArchivo(maps, nombreArchivo);
    printf("godines\n");
    int id;
    scanf("%d", id);
}