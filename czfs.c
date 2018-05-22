#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"

int main(int argc, char *argv[]) {
	directorio_completo = inicializar("simdiskformat.bin");
	printf("size puntero: %lu\n", sizeof(puntero));
	printf("size entrada_directorio: %lu\n", sizeof(entrada_directorio_estructura));
	printf("size directorio: %lu\n", sizeof(directorio_estructura));
	printf("size czfile: %lu\n", sizeof(czFILE));
	printf("size bitmap: %lu\n", sizeof(bitmap));
	printf("size indice: %lu\n", sizeof(indice_estructura));
	printf("size datos: %lu\n", sizeof(datos));
	printf("size indirecto: %lu\n", sizeof(indirecto_estructura));
	printf("nombre archivo vacio: %s\n", directorio_completo->estructura.entradas_directorio_estructura[10].nombre_archivo);
	printf("size archivo vacio: %d\n", directorio_completo->indices[10].estructura.size);

	char a;
	cz_open("simdiskformat.bin", 'w');
	printf("Done\n");
	scanf("%c", &a);
	//scanf("%c", &a);
	return 0;
}