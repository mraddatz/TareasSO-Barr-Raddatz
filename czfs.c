#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include "funciones.c"

int main(int argc, char *argv[]) {
	printf("size puntero: %lu\n", sizeof(puntero));
	printf("size entrada_directorio: %lu\n", sizeof(entrada_directorio));
	printf("size directorio: %lu\n", sizeof(directorio));
	printf("size czfile: %lu\n", sizeof(czFILE));
	printf("size bitmap: %lu\n", sizeof(bitmap));
	printf("size indice: %lu\n", sizeof(indice));
	printf("size datos: %lu\n", sizeof(datos));
	printf("size indirecto: %lu\n", sizeof(indirecto));

	char a;
	cz_open("", 'r');

	scanf("%c", &a);
	scanf("%c", &a);
}