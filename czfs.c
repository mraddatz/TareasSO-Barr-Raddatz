#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"

int main(int argc, char *argv[]) {
	directorio_completo = inicializar("simdiskfilled.bin");
	printf("size puntero: %lu\n", sizeof(puntero));
	printf("size entrada_directorio: %lu\n", sizeof(entrada_directorio_estructura));
	printf("size directorio: %lu\n", sizeof(directorio_estructura));
	printf("size czfile: %lu\n", sizeof(czFILE));
	printf("size bitmap: %lu\n", sizeof(bitmap));
	printf("size indice: %lu\n", sizeof(indice_estructura));
	printf("size datos: %lu\n", sizeof(datos));
	printf("size indirecto: %lu\n", sizeof(indirecto_estructura));
	printf("nombre archivo vacio: %s\n", directorio_completo->estructura.entradas_directorio_estructura[10].nombre_archivo);
	printf("%d\n", cz_exists("texto.txt"));
	printf("size archivo texto.txt: %d\n", directorio_completo->indices[1].estructura.size);
	printf("%d\n", cz_exists("a"));
	char buff[20] = "12345678901234567890";
	czFILE *czfile = cz_open("texto.txt", 'w');
	cz_write(czfile, buff, 10);
	printf(" Leyendo: %d\n", cz_read(czfile, buff, 3));


	//cz_rm("smile.png");
	//cz_close(czfile);
	//cz_mv("", "123");
	//cz_ls();
	cz_cp("texto.txt", "copy.txt");
	

	//cz_open("nuevo.txt", 'w');
	char a;
	scanf("%c", &a);
	//scanf("%c", &a);
	return 0;
}