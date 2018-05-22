#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "estructuras.c"

char leer_validez( FILE *fp, int position){
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

directorio *generar_directorio(char* filename){
	directorio *directorio_generado = calloc(1, sizeof(directorio));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(directorio_generado, sizeof(*directorio_generado), 1, fp);
    return directorio_generado;
}


directorio *abrir_directorio(char* filename){
	directorio *directorio_generado = calloc(1, sizeof(directorio));
	FILE * fp;
	fp = fopen(filename, "rb");
	printf("%i\n", fp);
	int i = 0;
	int disk_byte_position=0;
	for (i=0;i<64;i++){
		printf("FP %i es %i\n", i, fp);
		entrada_directorio* entrada=calloc(1,sizeof(entrada_directorio));
		entrada->valid = leer_validez(fp, disk_byte_position);
		disk_byte_position += 1;
		entrada->nombre_archivo = leer_nombre(fp, disk_byte_position, 11);
		disk_byte_position += 11;
		entrada->ubicacion_indice = leer_puntero(fp, disk_byte_position);
		disk_byte_position += 3;
		directorio_generado->entradas_directorio[i] = *entrada;
		printf("Entreda %i\n", i);
	}

	return directorio_generado;
}

int encontrar_bloque_disponible(FILE *fp){
	char* bits_map = calloc(8192, sizeof(unsigned char));
	int i;
	unsigned char read;
	for (i=0; i<8192; i++){
		read = leer_validez(fp, 1024);
		showbits(read);
	}
}

unsigne

int ingresar_entrada_directorio(directorio* dir, entrada_directorio entrada){
	int i;
	for (i=0; i<64; i++){
		if (dir->entradas_directorio[i].valid == 0){
			printf("Entrada vacia\n");
			dir->entradas_directorio[i] = entrada;
			return 0;
		}
		else{
			printf("Entrada Ocupada\n");
		}
	}
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





czFILE* cz_open(char* filename, char mode){
	const char read = 'r';
	const char write = 'w';
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
		bits_map = abrir_bitsmap(filename)
		int i;
		for(i=0; i<64; i++){
			 entrada_directorio* entrada = &bloque_directorio->entradas_directorio[i];
			if(strcmp(entrada->nombre_archivo, filename)==0 && entrada->valid){
				printf("%s\n", "Existe este nombre");
				return NULL;
			}
			else{
				printf("no existe este nombre\n");
			}
		}
		FILE * fp;
		fp=fopen(filename, "rb");
		entrada_directorio nueva_entrada;
		nueva_entrada.nombre_archivo = "texto.txt";
		nueva_entrada.valid = 1;
		nueva_entrada.ubicacion_indice=100;
		printf("%i\n", fp);
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);
		escribir_int(fp, 1026, 255);
		encontrar_bloque_disponible(fp);


		//iterar de 0 a 63 entreadas en directorio, and



	}
	printf("Sale del write\n");
	czFILE *hola = calloc(1, sizeof(czFILE));
	return hola;
}
