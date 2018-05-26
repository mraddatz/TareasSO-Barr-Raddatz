#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "estructuras.c"

FILE* fp;
directorio *directorio_completo;


void showbits(unsigned char x){
    int i; 
    for(i=(sizeof(x)*8)-1; i>=0; i--)
            (x&(1u<<i))?putchar('1'):putchar('0');
    
    printf("\n");
}
//Solo para byte
void use_block(unsigned char* byte, int numero){

	if (numero == 0) *byte += 128;
	if (numero == 1) *byte += 64;
	if (numero == 2) *byte += 32;
	if (numero == 3) *byte += 16;
	if (numero == 4) *byte += 8;
	if (numero == 5) *byte += 4;
	if (numero == 6) *byte += 2;
	if (numero == 7) *byte += 1;
}

int isBigEndian() {
    int test = 1;
    char *p = (char*)&test;

    return p[0] == 0;
}

int packInt(char toBigEndian, int x){
    int myInt;
    char *packed = (char*)&myInt;
    char *ptrToX = (char*)&x;

    // If we want to store big and the architecture is already big or
    // if we want to store little and the architecture is already little
    // then we don't need to do anything special
    char needsFix = !(
        ( toBigEndian &&  isBigEndian()) ||
        (!toBigEndian && !isBigEndian())
    );

    if( needsFix ){
        packed[0] = ptrToX[3];
        packed[1] = ptrToX[2];
        packed[2] = ptrToX[1];
        packed[3] = ptrToX[0];
    }else{
        packed[0] = ptrToX[0];
        packed[1] = ptrToX[1];
        packed[2] = ptrToX[2];
        packed[3] = ptrToX[3];
    }
    return myInt;
}



//Solo para byte
void release_block(unsigned char* byte, int numero){
	if (numero == 0 ) *byte -= 128;
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
	for (int bitmap_number=0; bitmap_number<8; bitmap_number++){
		if (numero_bloque<8192*(bitmap_number+1)){
			int bloque_en_bitmap = numero_bloque-(8192*bitmap_number);
			for (int byte=0; byte<1024; byte++){
				if ((byte*8 < bloque_en_bitmap) & (byte*8+8 > bloque_en_bitmap)){
					int bloque_en_byte = bloque_en_bitmap%8;
					printf("bitmap: %i  byte en bitmap: %i , bloque en byte: %i\n", bitmap_number, byte, bloque_en_byte);
					use_block(&dir->bitmaps[bitmap_number].bytearray[byte], bloque_en_byte);
					return 1;
				}
			}
		}
	}
	return -1;
}

void liberar_bloque(directorio* dir, int numero_bloque){
	for (int bitmap_number=0; bitmap_number<8; bitmap_number++){
		if (numero_bloque<8192*(bitmap_number+1)){ //identificamos bitmap
			int bloque_en_bitmap = numero_bloque-(8192*bitmap_number);
			for (int byte=0; byte<1024; byte++){
				if ((byte*8 <= bloque_en_bitmap) & (byte*8+8 > bloque_en_bitmap)){ //identificamos byte en bitmap
					int bloque_en_byte = bloque_en_bitmap%8;
					release_block(&dir->bitmaps[bitmap_number].bytearray[byte], bloque_en_byte);
					showbits(dir->bitmaps[bitmap_number].bytearray[byte]);
				}
			}
		}
	}
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

void print_bitsmap(bitmap* bit){
	for (int i=0; i<1024; i++){
		showbits(bit->bytearray[i]);
	}
	}

//Entrego indice y me dice cuantos bloques utiliza
int bloques_utilizados(indice* indice){
	int size_bytes = indice->estructura.size;
	int bloques_enteros = size_bytes / 1024;
	int bloques_incompletos = 0;
	if (size_bytes%1024>0){
		bloques_incompletos =1;
	}

	return bloques_enteros + bloques_incompletos;
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


void escribir_int_disco( FILE *fp, int pos, int numero){
	unsigned char buffer[4];
  buffer[0]=numero>>24;
  buffer[1]=numero>>16;
  buffer[2]=numero>>8;
  buffer[3]=numero;
  fseek(fp, pos, SEEK_SET);
  fwrite(&buffer,1,4,fp);
	}

void escribir_char_disco( FILE *fp, int pos, char caracter){
  unsigned char buffer[1];
    buffer[0]=caracter;
    fseek(fp, pos, SEEK_SET);
    fwrite(buffer,1,1,fp);
  }

void escribir_string_disco( FILE *fp, int pos, char* str, int size_of_str){
      fseek(fp, pos, SEEK_SET);
      fwrite(str,1,size_of_str,fp);
	}

void guardar_todo(FILE* fp, directorio* dir){
	int pos=0;
	for (int i=0; i<64; i++){
		
		escribir_char_disco(fp, pos, dir->estructura.entradas_directorio_estructura[i].valid);
		pos += 1;
		escribir_string_disco(fp, pos, dir->estructura.entradas_directorio_estructura[i].nombre_archivo, 11);
		pos += 11;
		escribir_int_disco(fp, pos, dir->estructura.entradas_directorio_estructura[i].ubicacion_indice);
		pos += 4;
	}
	}

void borrar_todo(FILE* fp, directorio* dir){
	int pos=0;
	for (int i=0; i<64; i++){
		
		int cero = 0;
		escribir_int_disco(fp, pos, cero);
		pos += 4;
		escribir_int_disco(fp, pos, cero);
		pos += 4;
		escribir_int_disco(fp, pos, cero);
		pos += 4;
		escribir_int_disco(fp, pos, cero);
		pos += 4;

	}
	}


unsigned char modifybitto1(unsigned char *byte, int n_bit){
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

directorio *inicializar(char* filename){
	directorio *directorio_completo = calloc(1, sizeof(directorio));
	fp=fopen(filename, "rb+");

    fread(&directorio_completo->estructura, 1024, 1, fp);
    for (int i=0; i<64; i++){
    	directorio_completo->estructura.entradas_directorio_estructura[i].ubicacion_indice = packInt(1, directorio_completo->estructura.entradas_directorio_estructura[i].ubicacion_indice);
    	    }

    printf("Leyendo directorio\n");
    printf("Ubicacion indice: %i\n", directorio_completo->estructura.entradas_directorio_estructura[0].ubicacion_indice);


	for (int i=0;i<8;i++){
		fread(&(directorio_completo->bitmaps[i]), 1024, 1, fp);
	}
	for (int i=0;i<64;i++){ //para cada entrada
		if (directorio_completo->estructura.entradas_directorio_estructura[i].valid >0){ //si es valida
			fseek(fp, directorio_completo->estructura.entradas_directorio_estructura[i].ubicacion_indice * 1024, SEEK_SET);
			fread(&(directorio_completo->indices[i].estructura), sizeof(directorio_completo->indices[0].estructura), 1, fp);
			
			//Cambiar endianess
			directorio_completo->indices[i].estructura.size = packInt(1, directorio_completo->indices[i].estructura.size);
			directorio_completo->indices[i].estructura.timestamp_creacion = packInt(1, directorio_completo->indices[i].estructura.timestamp_creacion);
			directorio_completo->indices[i].estructura.timestamp_modificacion = packInt(1, directorio_completo->indices[i].estructura.timestamp_modificacion);
			for (int d=0; d<252;d++){
				directorio_completo->indices[i].estructura.ubicaciones_directos[d] = packInt(1, directorio_completo->indices[i].estructura.ubicaciones_directos[d]);
			}
			directorio_completo->indices[i].estructura.ubicacion_indirecto = packInt(1, directorio_completo->indices[i].estructura.ubicacion_indirecto);


			//leo ahora el indirecto, el if no es necesario
			if (directorio_completo->indices[i].estructura.ubicacion_indirecto>0){
				fseek(fp, directorio_completo->indices[i].estructura.ubicacion_indirecto * 1024, SEEK_SET);
				fread(&(directorio_completo->indices[i].indirecto.estructura), sizeof(directorio_completo->indices[0].indirecto.estructura), 1, fp);
				for (int d=0; d<256;d++){
					//cambio endianess
					directorio_completo->indices[i].indirecto.estructura.ubicacion_directos[d] = packInt(1, directorio_completo->indices[i].indirecto.estructura.ubicacion_directos[d]);
				}
			}


	    	//tengo leido los indices directos e indirectos, ahora leo la data propiamente tal.
	    	//identifico cuantos bloques utiliza esta entrada
	    	int bloques = bloques_utilizados(&directorio_completo->indices[i]);

			for(int n_bloque_datos=0; n_bloque_datos < bloques; n_bloque_datos++){
				if (n_bloque_datos<252){
					int num_bloque = directorio_completo->indices[i].estructura.ubicaciones_directos[n_bloque_datos];
					fseek(fp, num_bloque * 1024, SEEK_SET);
					fread(&(directorio_completo->indices[i].datos[n_bloque_datos]), sizeof(datos), 1, fp);
				}
				else{
					int num_bloque = directorio_completo->indices[i].indirecto.estructura.ubicacion_directos[n_bloque_datos];
					fseek(fp, num_bloque * 1024, SEEK_SET);
					fread(&(directorio_completo->indices[i].indirecto.datos[n_bloque_datos-252]), sizeof(datos), 1, fp);
				}
				}


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
	int bytes_restantes = nbytes;
	int bytes_escritos = 0;
	int posicion_escritura_actual = file_desc->indice->estructura.size;
	while (bytes_restantes > 0){
		if (bytes_restantes > 1024 - posicion_escritura_actual % 1024){
			memcpy(&file_desc->indice->datos[(posicion_escritura_actual) / 1024].data[(posicion_escritura_actual) % 1024], buffer + bytes_escritos, 1024 - posicion_escritura_actual % 1024);
			bytes_escritos += 1024 - file_desc->indice->estructura.size % 1024;
			posicion_escritura_actual += 1024 - file_desc->indice->estructura.size % 1024;
			bytes_restantes -= 1024 - file_desc->indice->estructura.size % 1024;	
		}else{
			memcpy(&file_desc->indice->datos[(posicion_escritura_actual) / 1024].data[(posicion_escritura_actual) % 1024], buffer + bytes_escritos, nbytes);
			return nbytes;
		}
		//asigno nuevo bloque
		if (posicion_escritura_actual/ 1024 == 0){
			int nuevo_bloque = bloque_disponible(directorio_completo);
			if (nuevo_bloque == -1){
				return bytes_escritos;
			}
			utilizar_bloque(directorio_completo, nuevo_bloque);
			// si estoy en los directos
			if (posicion_escritura_actual < 252 * 1024){
				file_desc->indice->estructura.ubicaciones_directos[posicion_escritura_actual / 1024] = nuevo_bloque;
			}else{
				//si es mi primer indirecto
				if (posicion_escritura_actual == 252 * 1024){
					int bloque_indirecto =bloque_disponible(directorio_completo);
					//si no hay espacio para asignar un indirecto
					if (bloque_indirecto == -1){
						file_desc->indice->estructura.ubicacion_indirecto = nuevo_bloque;
						return bytes_escritos;
					}
					utilizar_bloque(directorio_completo, bloque_indirecto);
					file_desc->indice->estructura.ubicacion_indirecto = bloque_indirecto;
					file_desc->indice->indirecto.estructura.ubicacion_directos[0] = nuevo_bloque;
				}else{
					file_desc->indice->indirecto.estructura.ubicacion_directos[(posicion_escritura_actual - 252 * 1024) / 1024] = nuevo_bloque;
				}
			}
		}
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


//Si no alcanza el espacio se copia hasta donde se pueda
int cz_cp(char* orig, char* dest){
	int dest_existe = 0;
	int n_orig = -1;
	int n_dest = -1;
	if (strcmp(orig, dest) == 0){
		return -1;
	}
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, orig) == 0) & 
		(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			n_orig = i;
		}
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, dest) == 0) & 
		(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			 dest_existe = 1;
		}
	}
	if (n_orig==-1 || dest_existe == 1) return -1;
	//Buscamos primera entrada libre
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if (directorio_completo->estructura.entradas_directorio_estructura[i].valid == 0){
			n_dest = i;
			break;
		}
	}
	int bloque_libre = bloque_disponible(directorio_completo);

	if (bloque_libre==-1){
			dest_existe = 1;
			return -1;
		}
		if (sizeof(dest)>11){
			return -1;
		}
	
	//Creamos la entrada
	entrada_directorio_estructura* entrada_dir = &directorio_completo->estructura.entradas_directorio_estructura[n_dest];
	entrada_dir->valid = 1;
	for (int i=0; i<sizeof(dest); i++){
		strcpy(&entrada_dir->nombre_archivo[i], &dest[i]);
	}		
	entrada_dir->ubicacion_indice = bloque_libre;
	utilizar_bloque(directorio_completo, bloque_libre); //Ocupamos el bloque en bitmap
	//creamos el indice
	indice *indice = &directorio_completo->indices[n_dest];
	indice_estructura *indice_e = &indice->estructura;
	indice_e->timestamp_creacion=(int)time(NULL);
	indice_e->timestamp_modificacion=(int)time(NULL);
	//le asigno altiro un indirecto
	if (indice_e->size > 1024 * 252){
		int bloque_libre_indirecto = bloque_disponible(directorio_completo);
		if (bloque_libre_indirecto == -1) return -1;
		indice_e->ubicacion_indirecto = bloque_libre_indirecto;
		utilizar_bloque(directorio_completo, bloque_libre_indirecto); //Ocupamos el bloque indirecto libre
	}

	//Copiar
	//Bloques a copiar
	int cantidad_bloques_copiar = bloques_utilizados(&directorio_completo->indices[n_orig]);
	int bloque_nuevo;
	//ahora copio data
	for (int i =0; i<cantidad_bloques_copiar;i++){
		bloque_nuevo = bloque_disponible(directorio_completo);
		if (bloque_nuevo == -1) return -1;
		if (i<252){
			indice->estructura.ubicaciones_directos[i]=bloque_nuevo;
			memcpy(indice->datos[i].data, directorio_completo->indices[n_orig].datos[i].data, 1024);
		}
		else{
			indice->indirecto.estructura.ubicacion_directos[i-252] = bloque_nuevo;
			memcpy(indice->indirecto.datos[i-252].data, directorio_completo->indices[n_orig].indirecto.datos[i-252].data, 1024);
		}
		utilizar_bloque(directorio_completo, bloque_nuevo);
	}
	
		

	//Guardo cuando ya este validado
	directorio_completo->estructura.entradas_directorio_estructura[n_dest] = *entrada_dir;
	directorio_completo->indices[n_dest]=*indice;

	return 0;
	}




int cz_rm(char* filename){
	for (int i=0; i < sizeof(directorio_estructura)/sizeof(entrada_directorio_estructura); i++){
		if ((strcmp(directorio_completo->estructura.entradas_directorio_estructura[i].nombre_archivo, filename) == 0) & 
		(directorio_completo->estructura.entradas_directorio_estructura[i].valid != 0)){
			printf("Esta este archivo\n");
			int n_archivo = i;

			int num_bloques = bloques_utilizados(&directorio_completo->indices[n_archivo]);
			int bloques_a_liberar[num_bloques];
			for (int i = 0; i<num_bloques; i++){
				if (i<252){//reviso en el directo
					bloques_a_liberar[i] = directorio_completo->indices[n_archivo].estructura.ubicaciones_directos[i];
				}
				else{//reviso en el indirecto
					bloques_a_liberar[i] = directorio_completo->indices[n_archivo].indirecto.estructura.ubicacion_directos[i-252];
				}
			}
			printf("Bloques a Borrar:\n");
			printf("%d\n", directorio_completo->estructura.entradas_directorio_estructura[n_archivo].ubicacion_indice);
			printf("%d\n", directorio_completo->indices[n_archivo].estructura.ubicacion_indirecto);
			for (int e=0; e<num_bloques; e++){
				printf("%i\n", bloques_a_liberar[e]);
			}

			//liberamos los bloques
			liberar_bloque(directorio_completo, directorio_completo->estructura.entradas_directorio_estructura[n_archivo].ubicacion_indice);
			liberar_bloque(directorio_completo, directorio_completo->indices[n_archivo].estructura.ubicacion_indirecto);
			for (int e=0; e<num_bloques; e++){
				liberar_bloque(directorio_completo, bloques_a_liberar[e]);
			}

			//validez de la entrada a 0
			directorio_completo->estructura.entradas_directorio_estructura[n_archivo].valid = 0;
			directorio_completo->estructura.entradas_directorio_estructura[n_archivo].ubicacion_indice = 0;
			return 0;
		}
	}
	return -1;
	}


unsigned char modifybitto0(unsigned char *byte, int n_bit){
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
		//tenemos numero archivo
		if (n_archivo == -1){
			return NULL;
		}

		//rertornar czfilde archivo inice
		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->indice = &(directorio_completo->indices[n_archivo]);
		ret->mode = 'r';
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
		//si no hay bloques disponibles o filename es mayor a 11 caracteres retorno null
		if (bloque_libre==-1){
			return NULL;
		}
		if (sizeof(filename)>11){
			return NULL;
		}

		//Creamos la entrada
		entrada_directorio_estructura* entrada_dir = &directorio_completo->estructura.entradas_directorio_estructura[n_archivo];
		entrada_dir->valid = 1;
		for (int i=0; i<sizeof(filename); i++){
			strcpy(&entrada_dir->nombre_archivo[i], &filename[i]);
		}
		entrada_dir->ubicacion_indice = bloque_libre;
		directorio_completo->estructura.entradas_directorio_estructura[n_archivo] = *entrada_dir;
		utilizar_bloque(directorio_completo, bloque_libre); //Ocupamos el bloque en bitmap

		//creamos el indice
		indice* indice = &directorio_completo->indices[n_archivo];
		indice_estructura* indice_e;
		indice_e = &indice->estructura;
		int size = 0;
		int time_creacion = (int)time(NULL);
		int time_modificacion = (int)time(NULL);
		indice_e->size = size;
		indice_e->timestamp_creacion = time_creacion;
		indice_e->timestamp_modificacion = time_modificacion;

		//creamos el czfile
		czFILE *ret = calloc(1, sizeof(czFILE));
		ret->indice = &(directorio_completo->indices[n_archivo]);
		ret->mode='w';
		return ret;
	}
		czFILE *ret = calloc(1, sizeof(czFILE));
		return ret;
	}

int cz_close(czFILE* file_desc){
	borrar_todo(fp, directorio_completo);
	return 0;
}

