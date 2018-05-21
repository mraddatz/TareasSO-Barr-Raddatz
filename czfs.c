#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <signal.h>
// #include <sys/types.h>
#include <sys/wait.h>

typedef struct czFILE czFILE;
struct czFILE {
	unsigned char arreglo_bloques[65536];
};

czFILE* cz_open(char* filename, char mode){
	unsigned char buffer[1024];
	FILE *ptr;
	const char read = 'r';

	if (read == mode) {
   		ptr = fopen(filename,"rb");
   		fread(buffer,sizeof(buffer),1,ptr); // r for read, b for binary		}
   	}
	else{
	 printf("%s\n", "doesnt match");}

	 printf("%s\n", "Hola");


	//fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer

	for(int i = 0; i<100; i++){
    	printf("%u ", buffer[i]);
	}
}

typedef struct bloque bloque;
struct bloque {
	unsigned char block_size[1024];	
};


typedef int puntero;




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
	printf("%s\n", argv[2]);

	cz_open(argv[1], *argv[2]);

}