#include <stdio.h>

#define MAX_NOMBRE 100
#define MAX_BARRA 50

typedef struct {
    char nombre[MAX_NOMBRE];
    float *volumenes;
    int num_datos;
} Cuenca;

// Función auxiliar para encontrar el máximo volumen en un arreglo
float encontrar_maximo(const float *valores, int n) {
    float max = valores[0];
    int i;
    for (i = 1; i < n; i++) {
        if (valores[i] > max)
            max = valores[i];
    }
    return max;
}

// Función para graficar en ASCII la evolución del volumen por mes para cada cuenca
void graficar_evolucion_ascii(const Cuenca *cuencas, int total_cuencas) {
    int i, j;
    for (i = 0; i < total_cuencas; i++) {
        printf("Cuenca: %s\n", cuencas[i].nombre);
        float max = encontrar_maximo(cuencas[i].volumenes, cuencas[i].num_datos);

        for (j = 0; j < cuencas[i].num_datos; j++) {
            float valor = cuencas[i].volumenes[j];
            int longitud = (int)((valor / max) * MAX_BARRA);
            printf("Mes %2d [%6.1f hm3]: ", j + 1, valor);
            int k;
            for (k = 0; k < longitud; k++) printf("|");
            printf("\n");
        }
        printf("\n");
    }
}

