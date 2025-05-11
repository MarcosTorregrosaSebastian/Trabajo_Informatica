#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define MAX_EMBALSES 500 // Número máximo de registros que podemos manejar

struct Embalse {
    char cuenca[100];
    char nombre[100];
    int mes;
    float valores[10]; // 2012-2021
};

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
        // En este caso, asumimos que no hay comas dentro de los campos

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
        printf("Año no válido. Debe estar entre 2012 y 2021\n");
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

int main() {
    setlocale(LC_ALL, ""); // Para caracteres especiales ñ y tildes
    
    Embalse embalses[MAX_EMBALSES];
    int numEmbalses = cargarDatos("dataset.csv", embalses);
    
    if (numEmbalses == 0) {
        printf("No se pudieron cargar datos. Verifique que el archivo dataset.csv existe.\n");
        return 1;
    }
    
    printf("Datos cargados correctamente. Total de registros: %d\n", numEmbalses);
    mostrarDatos(embalses, numEmbalses);
    
    // Menú de opciones
    int opcion;
    do {
        printf("\n--- MENÚ DE OPCIONES ---\n");
        printf("1. Calcular media por mes\n");
        printf("2. Calcular media por año\n");
        printf("3. Calcular media por cuenca\n");
        printf("4. Mostrar primeros registros\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
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
                printf("Ingrese el año (2012-2021): ");
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
                mostrarDatos(embalses, numEmbalses);
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida\n");
        }
    } while (opcion != 0);
    
    return 0;
}