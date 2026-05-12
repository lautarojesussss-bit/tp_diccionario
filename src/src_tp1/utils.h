#ifndef UTILS_H_
#define UTILS_H_
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct vector {
	size_t cantidad;
	char **palabras;
};

typedef enum {
	LECTURA_OK = 0,
	FIN_DE_ARCHIVO = 1,
	ERROR_MEMORIA = -1,
	ERROR_DISCO = -2
} estado_lectura_t;

/*
 * PRE: 'cadena' debe ser un string válido terminado en '\0'. 'valor' debe ser un puntero a int válido (no NULL).
 * POST: Evalúa si toda la cadena está compuesta estrictamente por números. Si es así, convierte 
 * el string, lo guarda en '*valor' y devuelve true. Si está vacía o no tiene únicamente números, devuelve false.
 */
bool es_numero_valido(const char *cadena, int *valor);

/*
 * PRE: 'texto' debe ser un string válido (no NULL).
 * POST: Divide el texto dinámicamente utilizando el 'caracter_separador'. Devuelve un puntero a un struct vector 
 * que contiene las palabras ya separadas y su cantidad. Si ocurre un error de memoria o los parámetros son nulos, se devuelve NULL.
 */
struct vector *split(char *texto, char separador);

/*
 * PRE: -
 * POST: Libera toda la memoria dinámica asociada al struct vector.
 * Si el vector es NULL, no hace nada.
 */
void vector_destruir(struct vector *vector);

/*
 * PRE: 'archivo' debe estar abierto en modo lectura. 'error_memoria' y 'termino_el_archivo' deben ser válidos e inicializados en false.
 * POST: Lee dinámicamente una línea completa del archivo hasta encontrar un '\n' (salto de linea) o el EOF. 
 * Si encuentra un salto de linea reemplaza el '\n' por '\0' y devuelve el string leído alojado en el heap. 
 * Si falla la memoria o se llega al final del archivo sin leer nada, devuelve NULL y actualiza las banderas booleanas correspondientes.
 */
estado_lectura_t leer_linea(FILE *archivo, char *linea);

#endif //UTILS_H_