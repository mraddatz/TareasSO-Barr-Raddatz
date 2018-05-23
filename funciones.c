#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "estructuras.c"

char leer_validez(FILE *fp, int position){
  unsigned char vality[1];
  //fseek recives file position and given offset and places fp to where we are looking (fp + offset)
  fseek(fp, position, SEEK_SET);
  //Leemos 1 byte
  fread(vality, 1, 1, fp);
  return vality[0];
  }

//son de 11 bytes en caso de nombre archivo
char* leer_nombre(FILE *fp, int position, int string_size){
  char* string=calloc(string_size,sizeof(char));
  fseek(fp, position, SEEK_SET);
  fread(string, 1, string_size, fp);
  return string;
}

//Los punteros son siempre de tamaño 4
int leer_puntero(FILE *fp, int position){
	unsigned int puntero[1];
	fseek(fp, position, SEEK_SET);
	fread(puntero, 4, 1, fp); //Colocar 4 y 1 al revez
	return puntero[0];
}

void escribir_int(FILE *fp, int pos, int numero){
unsigned int buffer[1];
  buffer[0]=numero;
  fseek(fp, pos, SEEK_SET);
  fwrite(buffer,4,1,fp);

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

directorio_estructura *generar_estructura_directorio(char* filename){
	directorio_estructura *directorio_generado = calloc(1, sizeof(directorio_estructura));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(directorio_generado, sizeof(*directorio_generado), 1, fp);
    return directorio_generado;
}

indice_estructura *generar_estructura_indice(char* filename, int ubicacion){
	indice_estructura *indice_generado = calloc(1, sizeof(indice_estructura));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(indice_generado, sizeof(*indice_generado), 1, fp);
    return indice_generado;
}

directorio *directorio_completo;


directorio *inicializar(char* filename){
	directorio *directorio_completo = calloc(1, sizeof(directorio));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(&directorio_completo->estructura, 1024, 1, fp);
	for (int i=0;i<8;i++){
		fread(&(directorio_completo->bitmaps[i]), 1024, 1, fp);
	}
	for (int i=0;i<64;i++){
		//if valid
		fseek(fp, directorio_completo->estructura.entradas_directorio_estructura[i].ubicacion_indice * 1024, SEEK_SET);
	    fread(&(directorio_completo->indices[i].estructura), sizeof(indice), 1, fp);
		for(int n_bloque_datos=0; n_bloque_datos < 252; n_bloque_datos++){
			//fseek coloca en 
			fseek(fp, directorio_completo->indices[i].estructura.ubicaciones_directos[n_bloque_datos] * sizeof(indice_estructura), SEEK_SET);
		    fread(&(directorio_completo->indices[i].datos[n_bloque_datos]), sizeof(datos), 1, fp);
		}
	}
    return directorio_completo;
}


int cz_exists(char* filename){
	int archivo_existe = -1;
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, filename) == 0) & 
			(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			printf(" %s, %d", (directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo), i);
			archivo_existe = 1;
		}
	}
	return archivo_existe;
}

int cz_read(czFILE* file_desc, void* buffer, int nbytes){
	printf("%d--\n", file_desc->indice->datos[(file_desc->ubicacion_rw / 1024)].data[file_desc->ubicacion_rw % 1024]);
	memcpy(&buffer, &file_desc->indice->datos[(file_desc->ubicacion_rw / 1024)].data[file_desc->ubicacion_rw % 1024], nbytes);
	if (nbytes <= file_desc->indice->estructura.size - (file_desc->ubicacion_rw)){
		return nbytes;
	}else{
		return nbytes - (file_desc->indice->estructura.size - (file_desc->ubicacion_rw));
	};
	return file_desc->indice->estructura.size - (file_desc->ubicacion_rw);
}

//ver bloques
int cz_write(czFILE* file_desc, void* buffer, int nbytes){
	for (int i=0; i<nbytes; i++){
		memcpy(&file_desc->indice->datos[(file_desc->indice->estructura.size) / 1024].data[(file_desc->indice->estructura.size) % 1024], &buffer, i);
	}
	return 0;
}

void cz_ls(){
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if (directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0){
			printf("%s\n", directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo);
		}
	}
}

int cz_mv(char* orig, char *dest){
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, orig) == 0) & 
		(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			strcpy(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, dest);
			return 0;
		}
	}
	return -1;
}

int cz_cp(char* orig, char* dest){
	return 0;
}

int cz_rm(char* filename){
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, filename) == 0) & 
		(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			unsigned char valid = 0;
			memcpy(&directorio_completo->estructura.entradas_directorio_estructura[i].valid, &valid, 1);
			return 0;
		}
	}
	return -1;
}


unsigned char modifybitto0(unsigned char *byte, int n_bit)
{
    *byte = *byte&(~1u<<n_bit);
    return *byte;
}

czFILE* cz_open(char* filename, char mode){
	const char read = 'r';
	const char write = 'w';

	if (read == mode) {
   		//ptr = fopen(filename,"rb"); // r for read, b for binary		}

		int n_archivo = -1;
		//Encontramos el archivo dentro del directorio
		for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
			if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, filename) == 0) & 
				(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
				n_archivo = i;
			}
		}
		printf("numero archivo %s: %d\n", filename, n_archivo);
		if (n_archivo == -1){
			return NULL;
		}

		//rertornar czfile archivo indice
		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->indice = &(directorio_completo->indices[n_archivo]);
		return ret;

	}
	else if (write == mode){

	}

/*	if (read == mode) {
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

	    //copiar la información del archivo a ret, el cual es un czfile.
		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->mode = mode;
		//+2 para probar aunque el disco esté vacio
		ret->size = indice_archivo->size + 2;
		ret->content = calloc(ret->size, sizeof(char));
		for(int n_bloque_datos=0; n_bloque_datos < 252; n_bloque_datos++){
			//fseek coloca en 
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
	}
	else if (write == mode){
		printf("Entro a write con nombre %s\n", filename);
		int n_archivo = -1;
		directorio* bloque_directorio;
		bloque_directorio = abrir_directorio(filename);
		//bits_map = abrir_bitsmap(filename)
		int i;
		for(i=0; i<64; i++){
			 entrada_directorio* entrada = &bloque_directorio->entradas_directorio[i];
			if(strcmp(entrada->nombre_archivo, filename)==0 && entrada->valid){
				printf("%s\n", "Existe este nombre");
				return NULL;
			}
			else{
				//printf("no existe este nombre\n");
			}
		}
		FILE * fp;
		fp=fopen(filename, "rb");
		entrada_directorio nueva_entrada;
		strcpy(nueva_entrada.nombre_archivo, "texto.txt");
		nueva_entrada.valid = 1;
		nueva_entrada.ubicacion_indice=100;
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);
		escribir_int(fp, 1026, 255);
		//encontrar_bloque_disponible(fp);


		//iterar de 0 a 63 entreadas en directorio, and



	}
*/	printf("Sale del write\n");
	czFILE *hola = calloc(1, sizeof(czFILE));
	return hola;
}
