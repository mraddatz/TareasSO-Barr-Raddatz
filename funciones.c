#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include "estructuras.c"



directorio *generar_directorio(char* filename){
	directorio *directorio_generado = calloc(1, sizeof(directorio));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(directorio_generado, sizeof(*directorio_generado), 1, fp);
    return directorio_generado;
}

int cz_exists(char* filename){
	directorio *directorio_disco = calloc(1, sizeof(directorio));
	directorio_disco = generar_directorio("simdiskformat.bin");

	int archivo_existe = -1;
	for (int i=0; i < sizeof(directorio)/sizeof(entrada_directorio); i++){
		if ((strcmp(directorio_disco->entradas_directorio[i].nombre_archivo, filename) == 0) & (directorio_disco->entradas_directorio[i].valid != 0)){
			archivo_existe = 1;
		}
	}
	return archivo_existe;
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
	directorio *directorio_disco = calloc(1, sizeof(directorio));
	directorio_disco = generar_directorio("simdiskformat.bin");

	for (int i=0; i < sizeof(directorio)/sizeof(entrada_directorio); i++){
		if (directorio_disco->entradas_directorio[i].valid != 0){
			printf("nombre archivo %d: %s\n", i, directorio_disco->entradas_directorio[i].nombre_archivo);
		}
	}
}

void showbits(unsigned char x){
    int i; 
    for(i=(sizeof(x)*8)-1; i>=0; i--)
            (x&(1u<<i))?putchar('1'):putchar('0');
    
    printf("\n");
}

unsigned char modifybitto1(unsigned char *byte, int n_bit)
{
    *byte = *byte|(1u<<n_bit);
    return *byte;
}

unsigned char modifybitto0(unsigned char *byte, int n_bit)
{
    *byte = *byte&(~1u<<n_bit);
    return *byte;
}

czFILE* cz_open(char* filename, char mode){
	const char read = 'r';
	indice *indice_archivo = calloc(1, sizeof(indice));

	directorio *directorio_disco = calloc(1, sizeof(directorio));
	directorio_disco = generar_directorio("simdiskformat.bin");


	if (read == mode) {
   		//ptr = fopen(filename,"rb"); // r for read, b for binary		}

		int n_archivo = -1;
		//Encontramos el archivo dentro del directorio
		for (int i=0; i < sizeof(directorio)/sizeof(entrada_directorio); i++){
			if (strcmp(directorio_disco->entradas_directorio[i].nombre_archivo, filename) == 0){
				//agregar if validez
				n_archivo = i;
			}
		}
		if (n_archivo == -1){
			return NULL;
		}

		//abrir el archivo indice
		FILE * fp;
		fp=fopen("simdiskformat.bin", "rb");
	    fseek(fp, directorio_disco->entradas_directorio[n_archivo].ubicacion_indice, SEEK_SET);
	    fread(indice_archivo, sizeof(indice), 1, fp);
	    datos *bloque_datos = calloc(1, sizeof(datos));

	    //copiar la información del archivo a ret
		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->mode = mode;
		//+2 para probar aunque el disco esté vacio
		ret->size = indice_archivo->size + 2;
		ret->content = calloc(ret->size, sizeof(char));
		for(int n_bloque_datos=0; n_bloque_datos < 252; n_bloque_datos++){
			fseek(fp, indice_archivo->punteros_directos[n_bloque_datos], SEEK_SET);
		    fread(bloque_datos->data, sizeof(datos), 1, fp);
			for (int n_linea_bloque=0; n_linea_bloque < 1024; n_linea_bloque++){
				if (n_bloque_datos*1024 + n_linea_bloque < indice_archivo->size + 2){
					ret->content[n_bloque_datos*1024 + n_linea_bloque] = bloque_datos->data[n_linea_bloque];
				}
			}
		}

		//mostrar información
		for(int byte_n=0; byte_n < indice_archivo->size + 2; byte_n++){
			showbits(ret->content[byte_n]);
		}
		return ret;
	}else{

	}
	printf("%c\n", read);
	printf("%c\n", mode);
	czFILE *hola = calloc(1, sizeof(czFILE));
	return hola;
}
