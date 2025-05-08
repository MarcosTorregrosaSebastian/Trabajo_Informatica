#include <stdio.h>
#include<string.h>
#define n 45

void PreguntarOperacion(FILE *f);

void VisualizarDatoConcreto();
void VisualizarDatoEmbalse();
void VisualizarDatoEmbalseMes();
void VisualizaDatos();

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
	
	VisualizaDatos();
	
}
	


void VisualizaDatos(){
	
	int operacion;
	char embalse[45];
	int mes;
	int ano;
	printf("\n Inicio Visualizar Datos\n");
	printf("Que datos quiere buscar?\n");
	printf("1.Dato concreto\n");
	printf("2.Por embalse\n");
	printf("3.Por embalse y mes\n");
	printf("Seleccione operacion:");
	scanf("%i",&operacion);
	printf("\n");
	
	switch(operacion){
		
		case 1:
			VisualizarDatoConcreto();
			
		break;
		
		case 2:
			VisualizarDatoEmbalse();
		break;
		
		case 3:
			VisualizarDatoEmbalseMes();
		break;
		
		default:
			printf("error");
		break;
	}
	
}

void VisualizarDatoEmbalseMes(){
	char embalse[45];
	int mes;
	int numeroAnos = 10;
	int i;
	FILE *p;
	p = fopen("dataset.csv","r");
	
	printf("\nIntroduce el embalse: ");
	scanf("%s",embalse);
	printf("\nIntroduce el mes(en numero): ");
	scanf("%i",&mes);
	
	for(i=0;i<numeroAnos;i++){
		
		printf("%i\t",i+2012);
		
	}
	
	printf("\n");
	
	for(i=0;i<numeroAnos;i++){
		
		printf("%.2f\t",BuscarDatoEmbalseFecha(p,embalse,mes,i+2012));
		
	}
	fclose(p);
	printf("\n");
	VisualizaDatos();
}
void VisualizarDatoEmbalse(){

	char embalse[45];
	int numeroAnos = 10;
	int i,j;
	FILE *f;
	f = fopen("dataset.csv","r");
	if (f == NULL){	
		printf("error al abrir el fichero");
	}
	
	printf("\nIntroduzca el nombre del embalse:");
	scanf("%s",embalse);
	printf("\n");
	printf("Anos\t");
	for(j=1;j<=12;j++){
		
		printf(" %i \t",j);
	}
	printf("\n");
	
	for(i=0;i<numeroAnos;i++){
		
		printf("%i\t",i+2012);
		for(j=1;j<=12;j++){
		
			printf("%.2f\t",BuscarDatoEmbalseFecha(f,embalse,j,i+2012));
		
		}
		printf("\n");
	}
	fclose(f);
	printf("\n");
	VisualizaDatos();
}

void VisualizarDatoConcreto(){
	
	char embalse[45];
	int mes;
	int ano;
	FILE *f;
	f = fopen("dataset.csv","r");
	if (f == NULL){	
		printf("error al abrir el fichero");
	}else{
		printf("todo bien\n");
	}
	
	printf("\nIntroduzca el nombre del embalse:");
	scanf("%s",embalse);
	printf("\nIntroduzca el ano:");
	scanf("%i",&ano);
	printf("\nIntroduzca el mes(en numero):");
	scanf("%i",&mes);
			
	printf("\n El dato es: %f",BuscarDatoEmbalseFecha(f,embalse,mes,ano));
	
	fclose(f);
	printf("\n");
	VisualizaDatos();
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
	//La columna del dato solo depende del ao
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
        fscanf(f, "%[^\n]", dato); // Lee hasta el salto de línea
        fgetc(f);                  // Consume el salto de línea
        
    }
    //printf("\n%s\n",dato);
    
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
