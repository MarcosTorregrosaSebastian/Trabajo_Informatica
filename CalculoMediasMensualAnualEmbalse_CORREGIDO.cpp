
#include <stdio.h>
#include <string.h>
#include<locale.h>
#define N 45

// Variables globales simuladas (deberían llenarse al leer el CSV en tu código original)
int totalRegistros = 1000;
int anios[1000];
int meses[1000];
char cuencas[1000][N];
double valores[1000];

// Función para calcular la media mensual de un mes dado entre todas las cuencas
void MediaMensual() {
    int mesDeseado;
    printf("Introduce el numero del mes del que deseas calcular la media (1-12): ");
    scanf("%d", &mesDeseado);

    double suma = 0;
    int conteo = 0;

    for (int i = 0; i < totalRegistros; i++) {
        if (meses[i] == mesDeseado) {
            suma += valores[i];
            conteo++;
        }
    }

    if (conteo > 0)
        printf("Media mensual para el mes %d en todas las cuencas: %.2f\n", mesDeseado, suma / conteo);
    else
        printf("No se encontraron datos para el mes %d.\n", mesDeseado);
}

// Función para calcular la media anual de un año dado entre todas las cuencas
void MediaAnualPorAnio() {
    int anioDeseado;
    printf("Introduce el año del que deseas calcular la media anual (todas las cuencas): ");
    scanf("%d", &anioDeseado);

    double suma = 0;
    int conteo = 0;

    for (int i = 0; i < totalRegistros; i++) {
        if (anios[i] == anioDeseado) {
            suma += valores[i];
            conteo++;
        }
    }

    if (conteo > 0)
        printf("Media anual para el año %d en todas las cuencas: %.2f\n", anioDeseado, suma / conteo);
    else
        printf("No se encontraron datos para el año %d.\n", anioDeseado);
}

// Función para calcular la media de una cuenca específica
void MediaUnaCuenca() {
    char cuencaDeseada[N];
    printf("Introduce el nombre de la cuenca: ");
    scanf("%s", cuencaDeseada);

    double suma = 0;
    int conteo = 0;

    for (int i = 0; i < totalRegistros; i++) {
        if (strcmp(cuencas[i], cuencaDeseada) == 0) {
            suma += valores[i];
            conteo++;
        }
    }

    if (conteo > 0)
        printf("Media para la cuenca %s: %.2f\n", cuencaDeseada, suma / conteo);
    else
        printf("No se encontraron datos para la cuenca %s.\n", cuencaDeseada);
}

// Menú principal
void MostrarMenu() {
    int opcion;
    do {
        printf("\nSeleccione una opcion:\n");
        printf("1. Calcular media mensual (todas las cuencas)\n");
        printf("2. Calcular media anual por año (todas las cuencas)\n");
        printf("3. Calcular media de una cuenca\n");
        printf("4. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                MediaMensual();
                break;
            case 2:
                MediaAnualPorAnio();
                break;
            case 3:
                MediaUnaCuenca();
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);
}

// Función principal
int main() {
	setlocale(LC_CTYPE,"spanish");
    MostrarMenu();
    
    return 0;
}
