#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <locale.h>
#define n 45
//DECLARACION de FUNCIONES
//Inicio de Sesion
void inicioSesion();
void pedirCredenciales(char usuario[], char contrasena[]);
void guardarCredenciales(char usuario[],char contrasena[]);
int validarCredenciales(char usuario[], char contrasena[]);

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

//Maximos y Minimos
void MaximosMinimos();

//Manejo de flujo
void PreguntarOperacion();

//ESTRUCTURAS
typedef struct{
	char embalse[n];
	int mes;
	int ano;
}dato;


int main(){
	
	setlocale(LC_CTYPE,"spanish");

    inicioSesion();

	FILE *p;
	p = fopen("dataset.csv","r");
	
	if (p == NULL){	
		return -1;
	}else{
		printf("\nFichero abierto correctamente\n");
	}
	
	PreguntarOperacion(p);
	
	return 0;
}


//Manejo de Flujo
void PreguntarOperacion(){
    
	int *operacion;
    operacion = malloc(sizeof(int));
    if(operacion == NULL){
		printf("Error memoria no disponible");
	}
    printf("\n¿Que operacion quiere realizar?\n");
    printf("1.Calcular Media\n");
    printf("2.Comparacion de datos\n");
    printf("3.Maximos y Minimos\n");
    printf("4.Visualizar datos\n");
    printf("5.Finalizar programa\n");
    printf("Intorduzca el numero de su operacion: ");
    scanf("%i",operacion);
    
    switch(*operacion){
        case 1:
        	//Llama la funcion de Medias
            MostrarMenu();
        break;
        
        case 2:
        	
        break;
        
        case 3:
        	MaximosMinimos();
        break;
        
        case 4:
            VisualizaDatos();
            
        break;
        
        case 5:
            printf("\nPrograma Finalizado");
            return;
        break;
        
        default:
        	printf("NUMERO INCORRECTO\n");
        	printf("Asegurese de que introduce un numero entero del 1 al 5\n");
        	PreguntarOperacion();
        break;
        
    }
    free(operacion);
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
	
	int *operacion;
	char embalse[45];
	int mes;
	int ano;
	operacion = malloc(sizeof(int));
	if(operacion == NULL){
		printf("Error memoria no disponible");
	}
	
	printf("\n Inicio Visualizar Datos\n");
	printf("Que datos quiere buscar?\n");
	printf("1.Dato concreto\n");
	printf("2.Por embalse\n");
	printf("3.Por embalse y mes\n");
	printf("4.Volver al menu\n");
	printf("Seleccione operacion:");
	scanf("%i",operacion);
	printf("\n");
	
	switch(*operacion){
		
		case 1:
			VisualizarDatoConcreto();
			
		break;
		
		case 2:
			VisualizarDatoEmbalse();
		break;
		
		case 3:
			VisualizarDatoEmbalseMes();
		break;
		
		case 4:
			PreguntarOperacion();
		break;
		
		default:
			printf("Operacion no valida");
			VisualizaDatos();
		break;
	}
	free(operacion);
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
	dato busqueda;
	
	FILE *f;
	f = fopen("dataset.csv","r");
	if (f == NULL){	
		printf("error al abrir el fichero");
	}else{
		printf("todo bien\n");
	}
	
	printf("\nIntroduzca el nombre del embalse:");
	scanf("%s",busqueda.embalse);
	printf("\nIntroduzca el ano:");
	scanf("%i",&busqueda.ano);
	printf("\nIntroduzca el mes(en numero):");
	scanf("%i",&busqueda.mes);
			
	printf("\n El dato es: %f",BuscarDatoEmbalseFecha(f,busqueda.embalse,busqueda.mes,busqueda.ano));
	
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

//Inicio de Sesion
void inicioSesion(){
	
	char usuario[20], contrasena[20],respuesta;
	printf("¿Ya tienes una cuenta? (s/n): ");
    scanf(" %c", &respuesta);
    getchar(); // limpiar el buffer del salto de línea

	if (respuesta == 's' || respuesta == 'S') {
        pedirCredenciales(usuario, contrasena);
        if (validarCredenciales(usuario, contrasena)) {
            printf("¡Inicio de sesión exitoso! Bienvenido %s\n", usuario);
        } else {
            printf("Usuario o contraseña incorrectos.\n");
            inicioSesion();
		}
    } else {
        pedirCredenciales(usuario, contrasena);
        guardarCredenciales(usuario, contrasena);
        inicioSesion();
    }
	
}  
void pedirCredenciales(char usuario[], char contrasena[]) {
    char buffer[100];

    printf("Introduce el nombre de usuario: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar salto de línea
    strcpy(usuario, buffer);

    printf("Introduce la contraseña: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; 
    strcpy(contrasena, buffer);

	}
void guardarCredenciales(char usuario[],char contrasena[]) {
    
	int i;
	FILE* archivo = fopen("usuarios.txt", "a"); 
    if (archivo == NULL) {
        printf("Error al abrir el archivo para escritura.\n");
        return;
    }
	fprintf(archivo, "%s;%s\n", usuario, contrasena);
    
    fclose(archivo);
}
int validarCredenciales(char usuario[], char contrasena[]) {
    char linea[100];
    char u[20], c[20];
    
    FILE* archivo = fopen("usuarios.txt", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo. ¿Primera vez usando el sistema?\n");
        return 0;
    }
    
    while (fgets(linea, sizeof(linea), archivo)) {
        if (sscanf(linea, "%19[^;];%19s", u, c) == 2) {
            if (strcmp(u, usuario) == 0 && strcmp(c, contrasena) == 0) {
                fclose(archivo);
                return 1;
            }
        }
    }
    
    fclose(archivo);
    return 0;
}	
	
//Maximos y Minimos
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
			while(i<maxFila){
				
				if(temp<BuscarDatoConFilaColumna(f,i,numColumna)  ){
					
					temp = BuscarDatoConFilaColumna(f,i,numColumna);
				}
				i++;
			}
			printf("El embalse con mas agua en %i tenia %f",ano,temp);
			MaximosMinimos();
		break;
		case 2:
			temp = BuscarDatoConFilaColumna(f,i,numColumna);
			while(i<maxFila){
				
				if(temp>BuscarDatoConFilaColumna(f,i,numColumna)&& BuscarDatoConFilaColumna(f,i,numColumna) != 0){
					
					temp = BuscarDatoConFilaColumna(f,i,numColumna);
				}
				i++;
			}
			printf("El embalse con menos agua en %i tenia %f",ano,temp);
			MaximosMinimos();
		break;
		
		case 3:
			PreguntarOperacion();
		break;
		
		default:
			printf("\nOperacion incorrecta");
			MaximosMinimos();
		break;
	}
	fclose(f);
	return 0;
}