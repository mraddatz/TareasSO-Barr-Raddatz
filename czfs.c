#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <dos.h>

//prueba2
//prueba2
//prueba2
//prueba2
//prueba2
//prueba2
//prueba2

czFILE* cz\_open(char* filename, char mode){

}

int cz_exists(char* filename){

}

int cz_read(czFILE* file_desc, void* buffer, int nbytes){

}

int cz_write(czFILE* file_desc, void* buffer, int nbytes){

}

int cz_close(czFILE* file_desc){

}

int cz_mv(char* orig, char *dest){

}

int cz_cp(char* orig, char* dest){

}

int cz_rm(char* filename){

}

void cz_ls(){

}

typedef struct czFILE czFILE;
struct czFILE {
	bloque[65536] arreglo_bloques;
};

typedef struct bloque bloque;
struct bloque {
	
};

typedef struct directorio directorio;
struct directorio {
};

typedef struct bitmap bitmap;
struct bitmap{
};

typedef struct czfs czfs;
struct czfs {
};

typedef struct indice indice;
struct indice {
};

typedef struct datos datos;
struct datos {
};

typedef struct indirecto indirecto;
struct indirecto {
};




int main(int argc, char *argv[]) {



}