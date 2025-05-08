#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <locale.h>
#define n 45

float BuscarDatoConFilaColumna(FILE *f,int fila,int columna);
void MaximosMinimos();
int main(){
	MaximosMinimos();
	return 0;
}
void MaximosMinimos(){
	FILE *f;
	f = fopen("dataset.csv","r");
	float temp; //aqui iremos guardandi el dato mayor/menor
	int ano;
	int operacion;
	int i=1,maxFila = 4236;
	
	printf("Que quieres calcular\n1.Maximo\n2.Minimo\n3.Volver\n");
	printf("Intrduce operacion(1 o 2): ");
	scanf("%i",&operacion);
	printf("\nElige un ano(2012-2021): ");
	scanf("%i",&ano);
	
	//Averiguamos la columna del año de interes en la tabla
	int numColumna = ano-2012+4; 
	switch(operacion){
		case 1:
			temp = BuscarDatoConFilaColumna(f,i,numColumna);
			while(i<maxFila){
				
				if(temp<BuscarDatoConFilaColumna(f,i,numColumna)  ){
					
					temp = BuscarDatoConFilaColumna(f,i,numColumna);
				}
				i++;
			}
			printf("El embalse con mas agua en %i tenia %f",ano,temp);
		break;
		case 2:
			i = 2,
			temp = BuscarDatoConFilaColumna(f,i,numColumna);
			while(i<maxFila){
				
				if(temp>BuscarDatoConFilaColumna(f,i,numColumna)&& BuscarDatoConFilaColumna(f,i,numColumna) != 0){
					
					temp = BuscarDatoConFilaColumna(f,i,numColumna);
				}
				i++;
			}
			printf("El embalse con menos agua en %i tenia %f",ano,temp);
		break;
		case 3:
			
		break;
		default:
			printf("\nOperacion incorrecta");
			MaximosMinimos();
		break;
	}
	fclose(f);
	return 0;
}
float BuscarDatoConFilaColumna(FILE *f,int fila,int columna){
	
	char dato[256];
	int i;
	
	fseek(f, 0, SEEK_SET);
    // Avanza hasta la fila deseada
    for (i = 0; i < fila-1; i++) {
        fscanf(f, "%[^\n]", dato); // Lee hasta el salto de línea
        fgetc(f);                  // Consume el salto de línea
        
    }
    //printf("\n%s\n",dato);
    
    //Avanza por las columnas hasta el dato
	for (i = 0; i < columna; i++) {
        fscanf(f, "%[^,]", dato); 
        fgetc(f);                  
    }
    //printf("\n%s\n",dato);
    
    // Convierte 'dato' a un número flotante e imprime
    float numero;
    if (sscanf(dato, "%f", &numero) != 1) {
    	
		printf("El dato leido no es un número valido: %s\n", dato);
	
    } 
    return numero;
}