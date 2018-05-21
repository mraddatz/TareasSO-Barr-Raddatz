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


typedef struct entrada_directorio entrada_directorio;
struct entrada_directorio{
	unsigned char valid;
	char nombre_archivo[11];
	puntero ubicacion_indice;
};

typedef struct directorio directorio;
struct directorio {
	entrada_directorio entradas_directorio[64];
};

typedef struct bitmap bitmap;
struct bitmap{
	unsigned char bytearray[1024];	
};

typedef struct indice indice;
struct indice {
	unsigned int size;
	unsigned int timestamp_creacion;
	unsigned int timestamp_modificacion;
	puntero punteros_directos[252];
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

typedef struct czFILE czFILE;
struct czFILE {
	char mode;
	int size;
	unsigned char *content;
};
