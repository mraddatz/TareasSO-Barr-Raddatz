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
	unsigned char buffer[67108864];
	FILE *ptr;
	const char read = 'r';

	if (read == mode) {
   		ptr = fopen(filename,"rb");
   		fread(buffer,sizeof(buffer),1,ptr); // r for read, b for binary		}
   	}
	else{
	 printf("%s\n", "doesnt match");}

	 printf("%s\n", 'Hola');


	//fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer

	//for(int i = 0; i<8; i++){
    //	printf("%u ", buffer[i]);
	//}
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
	printf("%s\n", argv[2]);

	cz_open(argv[1], *argv[2]);
}