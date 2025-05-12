
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MAX_LINEA 512
#define PRIMER_ANO 2012
#define ULTIMO_ANO 2021
#define n 45
#define MAX_EMBALSES 500 // Número máximo de registros que podemos manejar

typedef struct {
    char cuenca[100];
    char nombre[100];
    int mes;
    float valores[10]; // 2012-2021
}Embalse;

//DECLARACION de FUNCIONES(prototipo)
//Inicio de Sesion
void inicioSesion();
void pedirCredenciales(char usuario[], char contrasena[]);
void guardarCredenciales(char usuario[],char contrasena[]);
int validarCredenciales(char usuario[], char contrasena[]);

//funciones busqueda de datos
float BuscarDatoEmbalseFecha(FILE *f, char embalse[n],int mes,int anio);
float BuscarDatoConFilaColumna(FILE *f,int fila,int columna);

//menu
void MostrarMenu();

//Calculo de medias
int cargarDatos(const char* filename, Embalse embalses[]);
void mediaPorMes(Embalse datos[], int numRegistros, int mes);
void mediaPorAnio(Embalse datos[], int numRegistros, int anio);
void mediaPorCuenca(Embalse datos[], int numRegistros, const char* cuenca);

//Visualizar Datos
void VisualizarDatoConcreto();
void VisualizarDatoEmbalse();
void VisualizarDatoEmbalseMes();
void VisualizaDatos();

//Maximos y Minimos
void MaximosMinimos();

//Comparacion de datos
void ComparacionDatos();
float buscarValor(char *nombre, int mesBuscado, int ano, const char *archivoCSV);
int obtenerIndiceAno(int anoBuscado);

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
	PreguntarOperacion();
    
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
        	ComparacionDatos();
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

//Calculo de Medias
int cargarDatos(const char* filename, Embalse embalses[]) {
    FILE* archivo = fopen("dataset.csv", "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    char linea[1024];
    int contador = 0;

    // Leer y descartar la primera línea (cabecera)
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) && contador < MAX_EMBALSES) {
        // Reemplazar comas dentro de campos (si existen) por otro carácter
        // Esto es necesario porque algunos nombres tienen guiones bajos que podrían confundirse con delimitadores
        // En este caso asumimos que no hay comas dentro de los campos

        char* token;
        int campo = 0;
        
        token = strtok(linea, ",");//Divide cada línea por comas
        while (token != NULL) {
            // Eliminar saltos de línea del final
            char* nl = strchr(token, '\n');//buscamos los saltos de linea con strchr
            if (nl) *nl = '\0';
            nl = strchr(token, '\r');
            if (nl) *nl = '\0';

            switch (campo) {
                case 0: // cuenca_hidrografica
                    strcpy(embalses[contador].cuenca, token);
                    break;
                case 1: // embalse_nombre
                    strcpy(embalses[contador].nombre, token);
                    break;
                case 2: // mes
                    embalses[contador].mes = atoi(token);
                    break;
                default: // años 2012-2021 (campos 3-12)
                    if (campo-3 >= 0 && campo-3 < 10) {
                        embalses[contador].valores[campo-3] = atof(token);
                    }
                    break;
            }

            token = strtok(NULL, ",");
            campo++;
        }

        contador++;
    }

    fclose(archivo);
    return contador;
}    
void MostrarMenu() {
    Embalse embalses[MAX_EMBALSES];
    int numEmbalses = cargarDatos("dataset.csv", embalses);
    
    if (numEmbalses == 0) {
        printf("No se pudieron cargar datos. Verifique que el archivo dataset.csv existe.\n");
    }
    
    printf("Datos cargados correctamente. Total de registros: %d\n", numEmbalses);
    //mostrarDatos(embalses, numEmbalses);
    
    // Menú de opciones y menejo de flujo
    int opcion;
    do {
        printf("\n--- MENÚ DE OPCIONES ---\n");
        printf("1. Calcular media por mes\n");
        printf("2. Calcular media por ano\n");
        printf("3. Calcular media por cuenca\n");
        printf("4. Mostrar primeros registros\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1: {
                int mes;
                printf("Ingrese el mes (1-12): ");
                scanf("%d", &mes);
                mediaPorMes(embalses, numEmbalses, mes);
                break;
            }
            case 2: {
                int anio;
                printf("Ingrese el ano (2012-2021): ");
                scanf("%d", &anio);
                mediaPorAnio(embalses, numEmbalses, anio);
                break;
            }
            case 3: {
                char cuenca[100];
                printf("Ingrese el nombre de la cuenca: ");
                scanf(" %[^\n]", cuenca); // Leer hasta fin de línea
                mediaPorCuenca(embalses, numEmbalses, cuenca);
                break;
            }
            case 4:
                //mostrarDatos(embalses, numEmbalses);
                break;
            case 0:
            	PreguntarOperacion();
            break;
            default:
                printf("Opcion no valida\n");
        }
    } while (opcion != 0);
}
void mediaPorMes(Embalse datos[], int numRegistros, int mes) {
    float suma = 0;
    int contador = 0;
    
    for (int i = 0; i < numRegistros; i++) {
        if (datos[i].mes == mes) {//buscamos que el mes coincida con el añadido por el usuario
            for (int j = 0; j < 10; j++) {
                suma += datos[i].valores[j];//hacemos un sumatorio de todos los datos
            }
            contador += 10;
        }
    }
    
    if (contador > 0) {
        printf("Media del mes %d: %.2f\n", mes, suma / contador);
    } else {
        printf("No hay datos para el mes %d\n", mes);
    }
}
void mediaPorAnio(Embalse datos[], int numRegistros, int anio) {
    float suma = 0;
    int contador = 0;
    int indiceAnio = anio - 2012;//hacemos esto para que se acople a la columna que deseamos
    
    if (indiceAnio < 0 || indiceAnio >= 10) {
        printf("Ano no valido. Debe estar entre 2012 y 2021\n");
        return;
    }
    
    for (int i = 0; i < numRegistros; i++) {
        suma += datos[i].valores[indiceAnio];
        contador++;
    }
    
    if (contador > 0) {
        printf("Media del año %d: %.2f\n", anio, suma / contador);
    } else {
        printf("No hay datos para el año %d\n", anio);
    }
}
void mediaPorCuenca(Embalse datos[], int numRegistros, const char* cuenca) {
    float suma = 0;
    int contador = 0;
    
    for (int i = 0; i < numRegistros; i++) {
        if (strcmp(datos[i].cuenca, cuenca) == 0) {
            for (int j = 0; j < 10; j++) {
                suma += datos[i].valores[j];
            }
            contador += 10;
        }
    }
    
    if (contador > 0) {
        printf("Media de la cuenca %s: %.2f\n", cuenca, suma / contador);
    } else {
        printf("No hay datos para la cuenca %s\n", cuenca);
    }
}
void mostrarDatos(Embalse datos[], int numRegistros) {
    for (int i = 0; i < numRegistros && i < 5; i++) { // Mostrar solo los primeros 5 para ejemplo
        printf("Cuenca: %s, Embalse: %s, Mes: %d, Valores: ", 
               datos[i].cuenca, datos[i].nombre, datos[i].mes);
        for (int j = 0; j < 10; j++) {
            printf("%.2f ", datos[i].valores[j]);
        }
        printf("\n");
    }
    if (numRegistros > 5) {
        printf("... (mostrando 5 de %d registros)\n", numRegistros);
    }
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
    strcpy(usuario, buffer);//copiamos el valor de buffer en usuario

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
            if (strcmp(u, usuario) == 0 && strcmp(c, contrasena) == 0) {//comparamos los dos caracteres
                fclose(archivo);
                return 1;
            }
        }
    }
    
    fclose(archivo);
    return 0;
}	


//Comparacion de Datos
void ComparacionDatos(){
	char cuenca1[50], cuenca2[50];
    int ano, mes;
    float valor1 = -1, valor2 = -1;

    const char *nombreArchivo = "cuencas.csv";

    while (1) {
        printf("Ingrese el nombre del primer embalse/cuenca: ");
        scanf("%s", cuenca1);

        printf("Ingrese el nombre del segundo embalse/cuenca: ");
        scanf("%s", cuenca2);

        printf("Ingrese el ano (2012–2021): ");
        scanf("%d", &ano);

        printf("Ingrese el numero de mes (1–12): ");
        scanf("%d", &mes);

        // Validación rápida de año y mes
        if (ano < PRIMER_ANO || ano > ULTIMO_ANO || mes < 1 || mes > 12) {
            printf("Ano o mes fuera de rango. Intente de nuevo.\n\n");
            continue;
        }

        // Buscar los valores
        valor1 = buscarValor(cuenca1, mes, ano, nombreArchivo);
        valor2 = buscarValor(cuenca2, mes, ano, nombreArchivo);

        if (valor1 == -1 || valor2 == -1) {
            printf("No se encontraron datos para una o ambas cuencas. Intente de nuevo.\n\n");
            continue;
        }

        break; // Salimos del bucle si todo es válido
    }

    // Mostrar resultados
    printf("\nRESULTADOS:\n");
    printf("- Cuenca: %s | Mes: %d | Año: %d | Volumen: %.2f\n", cuenca1, mes, ano, valor1);
    printf("- Cuenca: %s | Mes: %d | Año: %d | Volumen: %.2f\n", cuenca2, mes, ano, valor2);
    printf("=> Diferencia de volumen: %.2f\n", valor1 - valor2);

    return 0;
}
float buscarValor(char *nombre, int mesBuscado, int ano, const char *archivoCSV) {
    FILE *archivo = fopen(archivoCSV, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo: %s\n", archivoCSV);
        return -1;
    }

    char linea[MAX_LINEA];
    float valor = -1;
    int columnaAno = obtenerIndiceAno(ano);

    if (columnaAno == -1) {
        fclose(archivo);
        return -1;
    }

    fgets(linea, sizeof(linea), archivo); // Saltar encabezado

    while (fgets(linea, sizeof(linea), archivo)) {
        char *copia = strdup(linea);
        char *token = strtok(copia, ","); // ubicación (ignorar)

        token = strtok(NULL, ","); // nombre del embalse
        if (!token) {
            free(copia);
            continue;
        }
        char nombreEmbalse[50];
        strcpy(nombreEmbalse, token);

        token = strtok(NULL, ","); // mes
        if (!token) {
            free(copia);
            continue;
        }
        int mesActual = atoi(token);

        if (strcmp(nombreEmbalse, nombre) == 0 && mesActual == mesBuscado) {
            // Saltar a la columna correspondiente al año
            for (int i = 3; i < columnaAno; i++) {
                token = strtok(NULL, ",");
            }

            token = strtok(NULL, ",");
            if (token) valor = atof(token);
            free(copia);
            break;
        }

        free(copia);
    }

    fclose(archivo);
    return valor;
}
int obtenerIndiceAno(int anoBuscado) {
    if (anoBuscado < PRIMER_ANO || anoBuscado > ULTIMO_ANO) return -1;
    return anoBuscado - PRIMER_ANO + 3; // +3 por ubicación, nombre, mes
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
			printf("El embalse con mas agua en %i tenia %f\n",ano,temp);
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
			//PreguntarOperacion();
		break;
		
		default:
			printf("\nOperacion incorrecta");
			MaximosMinimos();
		break;
	}
	fclose(f);
	
}