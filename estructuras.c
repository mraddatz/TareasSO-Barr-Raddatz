#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int puntero;

typedef struct datos datos;
struct datos {
	unsigned char data[1024];
};

typedef struct indirecto indirecto;
struct indirecto {
	puntero punteros_directos[256];
	datos* ubicacion_datos[256];
};

typedef struct indice indice;
struct indice {
	unsigned int size;
	unsigned int timestamp_creacion;
	unsigned int timestamp_modificacion;
	puntero ubicaciones_directos[252];
	datos* punteros_datos[252];
	puntero ubicacion_indirecto;
	indirecto* puntero_indirecto;
};

typedef struct entrada_directorio entrada_directorio;
struct entrada_directorio{
	unsigned char valid;
	char nombre_archivo[11];
	puntero ubicacion_indice;
	indice* puntero_indice;
};

typedef struct directorio directorio;
struct directorio {
	entrada_directorio entradas_directorio[64];
};

typedef struct bitmap bitmap;
struct bitmap{
	unsigned char bytearray[1024];	
};

typedef struct czFILE czFILE;
struct czFILE {
	char mode;
	int size;
	puntero indice;
	puntero dir_indirecto;

	unsigned char *content;
};
