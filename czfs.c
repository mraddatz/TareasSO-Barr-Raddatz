#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>



typedef struct bloque bloque;
struct bloque {
	unsigned char block_size[1024];	
};

typedef int puntero;


typedef struct czFILE czFILE;
struct czFILE {
	bloque arreglo_bloques[65536];
};

typedef struct entrada_directorio entrada_directorio;
struct entrada_directorio{
	unsigned char valid;
	unsigned char nombre_archivo[11];
	puntero indice;
};

typedef struct directorio directorio;
struct directorio {
	entrada_directorio entradas_directorio[64];
};

typedef struct bitmap bitmap;
struct bitmap{
	unsigned char bitmap[1024];	
};

typedef struct indice indice;
struct indice {
	unsigned char size[4];
	unsigned char timestamp_creacion[4];
	unsigned char timestamp_modificacion[4];
	puntero punteros_directos[1008];
	puntero puntero_indirecto;
};

typedef struct datos datos;
struct datos {
	unsigned char data[1024];
};

typedef struct indirecto indirecto;
struct indirecto {
	puntero punteros_directos[256];
};


czFILE* cz_open(char* filename, char mode){
	FILE *ptr;
	const char read = 'r';

	if (read == mode) {
   		ptr = fopen(filename,"rb"); // r for read, b for binary		}
	}else{
	 printf("%s\n", "doesnt match");}

	printf("%c\n", read);
	printf("%c\n", mode);
}

int cz_exists(char* filename){
	return 0;
}

int cz_read(czFILE* file_desc, void* buffer, int nbytes){
	return 0;
}

int cz_write(czFILE* file_desc, void* buffer, int nbytes){
	return 0;
}

int cz_close(czFILE* file_desc){
	return 0;
}

int cz_mv(char* orig, char *dest){
	return 0;
}

int cz_cp(char* orig, char* dest){
	return 0;
}

int cz_rm(char* filename){
	return 0;
}

void cz_ls(){

}


int main(int argc, char *argv[]) {
	printf("size puntero: %lu\n", sizeof(puntero));
	printf("size entrada_directorio: %lu\n", sizeof(entrada_directorio));
	printf("size directorio: %lu\n", sizeof(directorio));
	printf("size czfile: %lu\n", sizeof(czFILE));
	printf("size bloque: %lu\n", sizeof(bloque));
	printf("size bitmap: %lu\n", sizeof(bitmap));
	printf("size datos: %lu\n", sizeof(datos));
	printf("size indirecto: %lu\n", sizeof(indirecto));

	indice *prueba = (indice*) calloc(1, sizeof(indice));
	int *num = (int*) calloc(1, sizeof(int));
	printf("%d, %d --\n", *prueba->size, *num);
	char a;

	scanf("%c", &a);
	printf("algo: %c\n", a);	
	//printf("algo: %s\n", a=='rb');
	scanf("%c", &a);
	char nombre[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
	cz_open(nombre, 'r');
}