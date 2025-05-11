#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 512
#define PRIMER_ANO 2012
#define ULTIMO_ANO 2021

int obtenerIndiceAno(int anoBuscado);
float buscarValor(char *nombre, int mes, int ano, const char *archivoCSV);

int main() {
    char cuenca1[50], cuenca2[50];
    int ano, mes;
    float valor1 = -1, valor2 = -1;

    const char *nombreArchivo = "cuencas.csv";

    while (1) {
        printf("Ingrese el nombre del primer embalse/cuenca: ");
        scanf("%s", cuenca1);

        printf("Ingrese el nombre del segundo embalse/cuenca: ");
        scanf("%s", cuenca2);

        printf("Ingrese el año (2012–2021): ");
        scanf("%d", &ano);

        printf("Ingrese el número de mes (1–12): ");
        scanf("%d", &mes);

        // Validación rápida de año y mes
        if (ano < PRIMER_ANO || ano > ULTIMO_ANO || mes < 1 || mes > 12) {
            printf("Año o mes fuera de rango. Intente de nuevo.\n\n");
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

// Devuelve el índice de columna correspondiente al año buscado
int obtenerIndiceAno(int anoBuscado) {
    if (anoBuscado < PRIMER_ANO || anoBuscado > ULTIMO_ANO) return -1;
    return anoBuscado - PRIMER_ANO + 3; // +3 por ubicación, nombre, mes
}

// Busca el valor de volumen de una cuenca en cierto mes y año
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

