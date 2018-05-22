#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"

int main(int argc, char *argv[]) {
	printf("size puntero: %lu\n", sizeof(puntero));
	printf("size entrada_directorio: %lu\n", sizeof(entrada_directorio));
	printf("size directorio: %lu\n", sizeof(directorio));
	printf("size czfile: %lu\n", sizeof(czFILE));
	printf("size bloque: %lu\n", sizeof(bloque));
	printf("size bitmap: %lu\n", sizeof(bitmap));
	printf("size indice: %lu\n", sizeof(indice));
	printf("size datos: %lu\n", sizeof(datos));
	printf("size indirecto: %lu\n", sizeof(indirecto));

	char a;
	cz_open("simdiskformat.bin", 'w');
	printf("Done\n");
	//scanf("%c", &a);
	//scanf("%c", &a);
	return 0;
}