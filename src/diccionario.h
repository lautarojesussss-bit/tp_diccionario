#ifndef __DICCIONARIO_H__
#define __DICCIONARIO_H__

#include <stdbool.h>
#include <stddef.h>

typedef struct diccionario diccionario_t;

struct diccionario_par {
	const char *clave;
	const void *valor;
};

/*
* Crea un diccionario vacío con una capacidad inicial dada. La capacidad nunca puede ser menor a 3.
*/
diccionario_t *diccionario_crear(size_t capacidad_inicial);

/**
 * Inserta un valor asociado a una clave. A efectos de este trabajo, se espera que la clave guardada por el diccionario sea una copia.
 * 
 * Si valor_anterior es un puntero válido, se almacena el valor previamente almacenado para la clave dada (o NULL en caso de que no existiera la clave).
 * 
 * Devuelve el diccionario o NULL en caso de error.
 */
diccionario_t *diccionario_insertar(diccionario_t *d, const char *clave,
				    void *valor, void **valor_anterior);

/**
 * Elimina del diccionario una clave. Devuelve el valor asociado a la clave eliminada o NULL en caso de error.
 * 
 */
void *diccionario_eliminar(diccionario_t *d, const char *clave);

/**
 * Devuelve el valor asociado con una clave en el diccionario o NULL en caso de error.
 */
void *diccionario_obtener(diccionario_t *d, const char *clave);

/*
* Devuelve true si la clave existe dentro del diccionario.
*/
bool diccionario_existe(diccionario_t *d, const char *clave);

/**
 * Devuelve la cantidad de claves almacenadas en el diccionario
 */
size_t diccionario_cantidad(diccionario_t *d);

/*
* Invoca la función f con cada par almacenado en el diccionario. Si la función f devuelve false, finaliza la iteración.
*
* Devuelve la cantidad de elementos iterados.
*/
size_t diccionario_con_cada_elemento(diccionario_t *d,
				     bool (*f)(struct diccionario_par *,
					       void *),
				     void *extra);

/**
 * Destruye el diccionario y aplica la función destructora a cada valor almacenado
 */
void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *));

/**
 * Destruye el diccionario
 */
void diccionario_destruir(diccionario_t *d);

#endif /* __DICCIONARIO_H__ */
