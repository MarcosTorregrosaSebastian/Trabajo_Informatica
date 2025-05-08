#include <stdio.h>
#include<string.h>
#define n 45
//DECLARACION de FUNCIONES
//funciones busqueda de datos
float BuscarDatoEmbalseFecha(FILE *f, char embalse[n],int mes,int anio);
float BuscarDatoConFilaColumna(FILE *f,int fila,int columna);
//Calculo de medias
void MostrarMenu();
void MediaUnaCuenca(); 
void MediaAnual();
void MediaMensual();
//Visualizar Datos
void VisualizarDatoConcreto();
void VisualizarDatoEmbalse();
void VisualizarDatoEmbalseMes();
void VisualizaDatos();
//Manejo de flujo
void PreguntarOperacion(FILE *f);

int main(){

	FILE *p;
	p = fopen("dataset.csv","r");
	if (p == NULL){	
		return -1;
	}else{
		printf("todo bien\n");
	}
	
	PreguntarOperacion(p);
	
	return 0;
}


//Manejo de Flujo
void PreguntarOperacion(FILE *f){
    int operacion;
    
    printf("\n¿Que operacion quiere realizar?\n");
    printf("1.Calcular Media\n");
    printf("2.Comparacion de datos\n");
    printf("3.Maximos y Minimos\n");
    printf("4.Visualizar datos\n");
    printf("5.Finalizar programa\n");
    printf("Intorduzca el numero de su operacion: ");
    scanf("%i",&operacion);
    
    switch(operacion){
        case 1:
        	//Llama la funcion de Medias
            MostrarMenu();
        break;
        
        case 2:
            CompararDatos();
        break;
        
        case 3:
        	
            MaximosMinimos();
            
        break;
        
        case 4:
            void VisualizaDatos();
            
        break;
        
        case 5:
            printf("\nPrograma Finalizado");
            return;
        break;
        
        default:
        	printf("NUMERO INCORRECTO\n");
        	printf("Asegurese de que introduce un numero entero del 1 al 5\n");
        	PreguntarOperacion(f);
        break;
        
    }
}

//Calculo de medias
char cuencas[n][25];
int anios[n];
int meses[n];
float valores[n];
int totalRegistros = 0;

void MediaMensual() {
    int mesElegido;
    printf("Introduce el número del mes (1 a 12) del que deseas calcular la media: ");
    scanf("%d", &mesElegido);

    if (mesElegido < 1 || mesElegido > 12) {
        printf("Mes no válido. Debe estar entre 1 y 12.\n");
        return;
    }

    double suma = 0;
    int conteo = 0;

    for (int i = 0; i < totalRegistros; i++) {
    	
        if (meses[i] == mesElegido) {
            suma += valores[i];
            conteo++;
        }
    }

    if (conteo > 0)
        printf("Media del mes %d entre todas las cuencas: %.2f\n", mesElegido, suma / conteo);
    else
        printf("No se encontraron datos para el mes %d.\n", mesElegido);
}
void MediaAnual() {
    int aniosUnicos[n];
    int conteoAnios = 0;
    double suma[n] = {0};
    int conteo[n] = {0};

    for (int i = 0; i < totalRegistros; i++) {
        int encontrado = 0;
        int j;
        for (j = 0; j < conteoAnios; j++) {
            if (anios[i] == aniosUnicos[j]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            aniosUnicos[conteoAnios] = anios[i];
            j = conteoAnios;
            conteoAnios++;
        }
        suma[j] += valores[i];
        conteo[j]++;
    }

    printf("\nMedia anual:\n");
    for (int i = 0; i < conteoAnios; i++) {
        printf("Año %d: %.2f\n", aniosUnicos[i], suma[i] / conteo[i]);
    }
}
void MediaUnaCuenca() {
    char cuencaBuscada[50];
    int opcion;
    printf("Introduce el nombre de la cuenca: ");
    scanf("%s", cuencaBuscada);

    printf("¿Qué media deseas calcular para la cuenca '%s'?\n", cuencaBuscada);
    printf("1. Media anual\n");
    printf("2. Media de un mes específico\n");
    printf("Selecciona una opción (1 o 2): ");
    scanf("%d", &opcion);

    double suma = 0;
    int conteo = 0;

    if (opcion == 1) {
        for (int i = 0; i < totalRegistros; i++) {
            if (strcmp(cuencas[i], cuencaBuscada) == 0) {
                suma += valores[i];
                conteo++;
            }
        }
        if (conteo > 0)
            printf("Media anual de la cuenca '%s': %.2f\n", cuencaBuscada, suma / conteo);
        else
            printf("No se encontraron datos para la cuenca '%s'.\n", cuencaBuscada);

    } else if (opcion == 2) {
        int mesDeseado;
        printf("Introduce el número del mes (1 a 12): ");
        scanf("%d", &mesDeseado);

        if (mesDeseado < 1 || mesDeseado > 12) {
            printf("Mes no válido.\n");
            return;
        }

        for (int i = 0; i < totalRegistros; i++) {
            if (strcmp(cuencas[i], cuencaBuscada) == 0 && meses[i] == mesDeseado) {
                suma += valores[i];
                conteo++;
            }
        }

        if (conteo > 0)
            printf("Media del mes %d para la cuenca '%s': %.2f\n", mesDeseado, cuencaBuscada, suma / conteo);
        else
            printf("No se encontraron datos para la cuenca '%s' en el mes %d.\n", cuencaBuscada, mesDeseado);
    } else {
        printf("Opción no válida.\n");
    }
}
void MostrarMenu() {
    int opcion;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Calcular media mensual\n");
        printf("2. Calcular media anual\n");
        printf("3. Calcular media de una cuenca\n");
        printf("0. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                MediaMensual();
                break;
            case 2:
                MediaAnual();
                break;
            case 3:
                MediaUnaCuenca();
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intenta de nuevo.\n");
        }
    } while(opcion != 0);
}

//Visualizacion de Datos
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

//Funciones Auxiliares (Herramienta)
//Busqueda de Datos en el Fichero
float BuscarDatoEmbalseFecha(FILE *f,char embalse[n],int mes,int anio){
	
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