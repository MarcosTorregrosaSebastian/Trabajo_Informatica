#include <stdio.h>
#include<string.h>
#define n 45

float BuscarDatoEmbalseFecha(FILE *f, char embalse[n],int mes,int anio);
float BuscarDatoConFilaColumna(FILE *f,int fila,int columna);

int main(){
	
	FILE *p;
	p = fopen("dataset.csv","r");
	if (p == NULL){	
		return -1;
	}else{
		printf("todo bien\n");
	}
	
	
	printf("\nbusca dato con nobre de embalse,mes y ano: \n");
	printf("dato1:  %f\n\n",BuscarDatoEmbalseFecha(p,"Aguilar",10,2015));
	
	printf("\nbusca dato con fila y columna: \n");
	printf("dato2: %f\n",BuscarDatoConFilaColumna(p,423,5));
	
	BuscarDatoConFilaColumna(p,353,13);
}
	



//FUNCIONES HERRAMIENTAS

float BuscarDatoEmbalseFecha(FILE *f, char embalse[n],int mes,int anio){
	
	int columnasTotal = 13;
	int datoColumna;
	int datoFila;
	char x[n];
	int i = 0;
	float dato;
	//Colocomas puntero en la segunada posicion en "embalse_nombre"
	fseek(f, 0, SEEK_SET);
	fscanf(f,"%[^,\n]",x);
	fgetc(f);
	//recorremos el fichero hasta encontrar el nombre del embalse buscado
	while(strcmp(x,embalse) != 0 /*&& fscanf(f,"%c",comprobarFin)!= EOF*/){
		
		if(fscanf(f,"%[^,\n]",x) == EOF){
			printf("nombre de embalse no encontrado\n");
			return 0;
		}
		fgetc(f);
		i++;
	}
	//podemos calcular la columna que nos interesa
	datoFila = i/columnasTotal + mes;	
	//La columna del dato solo depende del ano
	datoColumna = anio-2012 + 4;
	//printf("el dato esta en la columna %i y fila %i\n",datoColumna,datoFila);
	
	dato = BuscarDatoConFilaColumna(f,datoFila,datoColumna);
	
	return dato;
}

float BuscarDatoConFilaColumna(FILE *f,int fila,int columna){
	char dato[256];
	int i;
	
	fseek(f, 0, SEEK_SET);
    // Avanza hasta la fila deseada
    for (i = 0; i < fila-1; i++) {
        fscanf(f, "%[^\n]", dato); //avanza hasta el salto de linea
        fgetc(f);            		//salta el salto de línea
        
    }
    //printf("\n%s\n",dato);
    
	for (i = 0; i < columna; i++) {
        fscanf(f, "%[^,]", dato);   //avanza hasta la coma
        fgetc(f);					//salta la coma
    }
    
    // Convierte 'dato' a un número flotante e imprime
    if (sscanf(dato, "%f", &numero) != 1) {
    	
		printf("El dato leido no es un número valido: %s\n", dato); //avisa en caso de que sea un numeo
	
    } 
    return numero;
}