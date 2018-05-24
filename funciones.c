#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "estructuras.c"


void showbits(unsigned char x){
    int i; 
    for(i=(sizeof(x)*8)-1; i>=0; i--)
            (x&(1u<<i))?putchar('1'):putchar('0');
    
    printf("\n");
}
//Solo para byte
int use_block(unsigned char* byte, int numero){
	printf("Entered USe block\n");
	if (numero>7) return -1;
	int resto;
	resto = *byte / 128;
	if (numero == 0) *byte += 128;
	if (numero == 1) *byte += 64;
	if (numero == 2) *byte += 32;
	if (numero == 3) *byte += 16;
	if (numero == 4) *byte += 8;
	if (numero == 5) *byte += 4;
	if (numero == 6) *byte += 2;
	if (numero == 7) *byte += 1;
}

//Solo para byte
int release_block(unsigned char* byte, int numero){
	if (numero>7) return -1;
	int resto;
	resto = *byte / 128;
	if (numero == 0 && (*byte/128)) *byte -= 128;
	if (numero == 1 ) *byte -= 64;
	if (numero == 2) *byte -= 32;
	if (numero == 3) *byte -= 16;
	if (numero == 4) *byte -= 8;
	if (numero == 5) *byte -= 4;
	if (numero == 6) *byte -= 2;
	if (numero == 7) *byte -= 1;
}




char leer_validez(FILE *fp, int position){
  unsigned char vality[1];
  //fseek recives file position and given offset and places fp to where we are looking (fp + offset)
  fseek(fp, position, SEEK_SET);
  //Leemos 1 byte
  fread(vality, 1, 1, fp);
  return vality[0];
  }

//Encuentra dentro de 1 bitmap
int encontrar_bloque_disponible(bitmap* bit_map){
	int bloque=0;
	for (int byte=0; byte<1024; byte++){
		if (bit_map->bytearray[byte]<255){
			if (bit_map->bytearray[byte]<128) return bloque;
			else if (bit_map->bytearray[byte]<192) return bloque + 1; 
			else if (bit_map->bytearray[byte]<224) return bloque + 2;
			else if (bit_map->bytearray[byte]<240) return bloque + 3;
			else if (bit_map->bytearray[byte]<248) return bloque + 4;
			else if (bit_map->bytearray[byte]<252) return bloque + 5;
			else if (bit_map->bytearray[byte]<254) return bloque + 6;
			else return bloque + 7;
		}
		else{
			bloque += 8;
		}
	
	}
	return -1;
}

int utilizar_bloque(directorio* dir, int numero_bloque){
	int bloque=0;
	for (int bitmap_number=0; bitmap_number<8; bitmap_number++){
		if (numero_bloque<8192*(bitmap_number+1)){
			int bloque_en_bitmap = numero_bloque-(8192*bitmap_number);
			for (int byte=0; byte<1024; byte++){
				if (byte*8 < bloque_en_bitmap & byte*8+8 > bloque_en_bitmap){
					int bloque_en_byte = bloque_en_bitmap%8;
					printf("bitmap: %i  byte en bitmap: %i , bloque en byte: %i\n", bitmap_number, byte, bloque_en_byte);

					int a = use_block(&dir->bitmaps[bitmap_number].bytearray[byte], bloque_en_byte);
					return 1;
				}
			}

		}
	
	}
	return -1;
}

//Encuentra dentro de todos los bitmap
int bloque_disponible(directorio* directorio_completo){
	int bloque=0;
	int result;
	for(int i=0; i<8; i++){
		result = encontrar_bloque_disponible(&directorio_completo->bitmaps[i]);
		if (result>-1){
			return bloque+result;
		}
		else{
			bloque += 1024 * 8;
		}

	}
	return -1;
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


char leer_char_disco( FILE *fp, int pos){
  unsigned char buffer[1];
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 1, 1, fp);
  return buffer[0];
  }

void escribir_int(FILE *fp, int pos, int numero){
unsigned int buffer[1];
  buffer[0]=numero;
  fseek(fp, pos, SEEK_SET);
  fwrite(buffer,4,1,fp);

}



unsigned char modifybitto1(unsigned char *byte, int n_bit)
{
    *byte = *byte|(1u<<n_bit);
    return *byte;
}


unsigned char byte_de_1(unsigned char *byte, int n_bit)
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

indice_estructura *generar_estructura_indice(char* filename, int ubicacion){
	indice_estructura *indice_generado = calloc(1, sizeof(indice_estructura));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(indice_generado, sizeof(*indice_generado), 1, fp);
    return indice_generado;
}


int encontrar_bloque_disponible(bitmap bit_map){
	printf("Leyendo Bloque\n");
	for (int i=0; i<1024; i++){
		if(bit_map.bytearray[i]<255){
			printf("Hay espacio\n");
		}
		else{
			printf("No hay espacio\n");
		}
	}
	return 0;
}


directorio *inicializar(char* filename){
	directorio *directorio_completo = calloc(1, sizeof(directorio));
	FILE * fp;
	fp=fopen(filename, "rb");
    fread(&directorio_completo->estructura, 1024, 1, fp);

	for (int i=0;i<8;i++){
		fread(&(directorio_completo->bitmaps[i]), 1024, 1, fp);
		printf("FP: %i\n", fp);
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
		int n_archivo = -1;

		for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
			if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, filename) == 0) & 
				(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
				n_archivo = i;
			}
		}
		//si existe el archivo retorno null
		if (n_archivo > -1){
			return NULL;
		}

		//Buscamos primera entrada libre
		for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
			if (directorio_completo->estructura.entradas_directorio_estructura[i].valid == 0){
				n_archivo = i;
				break;
			}
		}


		int bloque_libre = bloque_disponible(directorio_completo);
		if (bloque_libre==-1){
			return NULL;
		}
		printf("Vamos a crear la entrada\n");
		//Creamos la entrada
		entrada_directorio_estructura* entrada_dir = calloc(1, sizeof(entrada_directorio_estructura));
		
		entrada_dir->valid = 1;
		//abort trap en este strcpy
		strcpy(entrada_dir->nombre_archivo, filename);
		entrada_dir->ubicacion_indice = bloque_libre;
		directorio_completo->estructura.entradas_directorio_estructura[n_archivo] = *entrada_dir;
		printf("Se creo la entrada\n");
		utilizar_bloque(directorio_completo, bloque_libre); //Ucupamos el bloque en bitmap

		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->indice = &(directorio_completo->indices[n_archivo]);
		return ret;
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
		fp=fopen(filename, "r+b");
		entrada_directorio nueva_entrada;
		strcpy(nueva_entrada.nombre_archivo, "texto.txt");
		nueva_entrada.valid = 1;
		nueva_entrada.ubicacion_indice=100;
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);
		ingresar_entrada_directorio(bloque_directorio, nueva_entrada);

		bitmap *bit_map_block = calloc(1, sizeof(bitmap));
		for (int i=0; i<1024; i++){
			showbits(bit_map_block->bytearray[i]);
		}
		//for (int i=0; i<1024; i++){
		//	bit_map_block->bytearray[i]= ~0u;
		//}

		//for (int i=0; i<1024; i++){
		//	showbits(bit_map_block->bytearray[i]);
		//}

		encontrar_bloque_disponible(*bit_map_block);



		//iterar de 0 a 63 entreadas en directorio, and



	}
*/	printf("Sale del write\n");
	czFILE *hola = calloc(1, sizeof(czFILE));
	return hola;
}
