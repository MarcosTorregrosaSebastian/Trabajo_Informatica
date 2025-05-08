#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

void inicioSesion(char usuario[], char contrasena[], char respuesta);

void pedirCredenciales(char usuario[], char contrasena[]);

void guardarCredenciales(char usuario[],char contrasena[]);

int validarCredenciales(char usuario[], char contrasena[]);
	
	
int main(){
	
	
	setlocale(LC_CTYPE,"spanish");

    char usuario[20], contrasena[20],respuesta;

    printf("¿Ya tienes una cuenta? (s/n): ");
    
    scanf(" %c", &respuesta);
    getchar(); // limpiar el buffer del salto de línea

    inicioSesion(usuario,contrasena,respuesta);
    
    return 0;
    
    
    
};
  
void inicioSesion(char usuario[], char contrasena[],char respuesta){
	
	
	if (respuesta == 's' || respuesta == 'S') {
        pedirCredenciales(usuario, contrasena);
        if (validarCredenciales(usuario, contrasena)) {
            printf("¡Inicio de sesión exitoso! Bienvenido %s\n", usuario);
        } else {
            printf("Usuario o contraseña incorrectos.\n");
			}
    } else {
        pedirCredenciales(usuario, contrasena);
        guardarCredenciales(usuario, contrasena);
        printf("Usuario registrado con éxito.\n");
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
		
		
		
		
		