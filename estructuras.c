#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int puntero;

typedef struct datos datos;
struct datos {
	unsigned char data[1024];
};

typedef struct indirecto_estructura indirecto_estructura;
struct indirecto_estructura {
	puntero ubicacion_directos[256];
};

typedef struct indirecto indirecto;
struct indirecto {
	indirecto_estructura estructura[256];
	datos datos[256];
};

typedef struct indice_estructura indice_estructura;
struct indice_estructura {
	unsigned int size;
	unsigned int timestamp_creacion;
	unsigned int timestamp_modificacion;
	puntero ubicaciones_directos[252];
	puntero ubicacion_indirecto;
};

typedef struct indice indice;
struct indice {
	indice_estructura estructura;
	datos datos[252];
	indirecto indirecto;
};

typedef struct entrada_directorio_estructura entrada_directorio_estructura;
struct entrada_directorio_estructura{
	unsigned char valid;
	char nombre_archivo[11];
	puntero ubicacion_indice;
};

typedef struct bitmap bitmap;
struct bitmap{
	unsigned char bytearray[1024];	
};

typedef struct directorio_estructura directorio_estructura;
struct directorio_estructura {
	entrada_directorio_estructura entradas_directorio_estructura[64];
};

typedef struct directorio directorio;
struct directorio {
	directorio_estructura estructura;
	indice indices[64];
	bitmap bitmaps[8];
};

typedef struct czFILE czFILE;
struct czFILE {
	char mode;
	int size;
	puntero indice;
	puntero dir_indirecto;

	unsigned char *content;
};
