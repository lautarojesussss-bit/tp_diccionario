#include "diccionario.h"
#include <stdlib.h>
#include <string.h>
#define SEMILLA_HASH 5381
#define FACTOR_CARGA 0.7

struct nodo_diccionario{
        struct diccionario_par par;
        struct nodo_diccionario *siguiente;
};

struct diccionario{
        struct nodo_diccionario **tabla;
        size_t capacidad;
        size_t cantidad;
};



/**
 * PRE: 'clave' debe ser un string no nulo
 * POST: devuelve un hash para la clave pasada
 */
static size_t hash_djb2(const char *clave)
{
        size_t hash = SEMILLA_HASH;
        int c;

        while((c = *clave++)) 
                hash = ((hash << 5) + hash) + c;

        return hash;
}

static void destruir_lista_hash(struct nodo_diccionario *primer_nodo,  void (*destructor)(void *))
{
        if(!primer_nodo)
                return;

        struct nodo_diccionario *nodo_actual = primer_nodo;

        while (nodo_actual != NULL){
                struct nodo_diccionario *nodo_siguiente = nodo_actual->siguiente;

                if (destructor != NULL)
                        destructor(nodo_actual->par.valor);

                free(nodo_actual->par.clave);
                free(nodo_actual);
                nodo_actual = nodo_siguiente;
        }
}

/**
 * PRE: 'clave' debe ser un string no nulo, y 'd' debe ser un diccionario válido
 * POST: devuelve el puntero al nodo que tiene la clave pasada, NULL si no se encuentra esa clave en el diccionario.
 */
static struct nodo_diccionario *obtener_nodo(const char *clave, diccionario_t *d)
{
        if(!clave || !d)
                return NULL;

        size_t posicion = hash_djb2(clave) % d->capacidad;
        struct nodo_diccionario* nodo_aux = d->tabla[posicion];

        while (nodo_aux != NULL 
                && strcmp(nodo_aux->par.clave, clave) != 0)
                nodo_aux = nodo_aux->siguiente;
        
        return nodo_aux;
}

/**
 * PRE: 'd' debe ser un diccionario válido, si es nulo se devuelve false
 * POST: Se duplica el tamaño de la tabla de 'd' y se reubican todos los elementos en la nueva tabla agrandada, si no hay errores devuelve true, si hay errores devuelve false
 */
static bool rehashear(diccionario_t *d)
{
        if(!d)
                return false;

        struct nodo_diccionario **tabla_expandida = calloc(d->capacidad * 2, sizeof(struct nodo_diccionario*));
        if (!tabla_expandida)
                return false;
        
        size_t capacidad_original = d->capacidad;

        for(size_t i; i < capacidad_original; i++)
                reubicar_nodo(d->tabla[i], tabla_expandida);

        free(d->tabla);
        d->tabla = tabla_expandida;
        return true;
}
/*
* Crea un diccionario vacío con una capacidad inicial dada. La capacidad nunca puede ser menor a 3.
*/
diccionario_t *diccionario_crear(size_t capacidad_inicial)
{
        diccionario_t *d = calloc(1, sizeof(struct diccionario));
        
        if (!d)
                return NULL;

        d->tabla = calloc(capacidad_inicial, sizeof(struct nodo_diccionario*));

        if (!d->tabla){
                free(d);
                return NULL;
        }

        d->capacidad = capacidad_inicial;
        return d;
}

/**
 * Devuelve la cantidad de claves almacenadas en el diccionario
 */
size_t diccionario_cantidad(diccionario_t *d)
{
        if(!d)
                return 0;

        return d->cantidad;
}





void *diccionario_obtener(diccionario_t *d, const char *clave)
{
        if(!d || !clave)
                return NULL;

        struct nodo_diccionario *nodo_aux = obtener_nodo(clave, d);
        
        if (!nodo_aux)
                return NULL;

        return nodo_aux->par.valor;
}

bool diccionario_existe(diccionario_t *d, const char *clave)
{
        if(!d || !clave)
                return NULL;

        struct nodo_diccionario *nodo_aux = obtener_nodo(clave, d);

        return (nodo_aux != NULL);
}


void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *))
{
        if(!d)
                return;

        size_t capacidad = d->capacidad;

        for (size_t i = 0; i < capacidad; i++)
                destruir_lista_hash(d->tabla[i], destructor);

        free(d->tabla);
        free(d);
}

void diccionario_destruir(diccionario_t *d)
{
        if(!d)
                return;

        diccionario_destruir_todo(d, NULL);
}


/**
 * Inserta un valor asociado a una clave. A efectos de este trabajo, se espera que la clave guardada por el diccionario sea una copia.
 * 
 * Si valor_anterior es un puntero válido, se almacena el valor previamente almacenado para la clave dada (o NULL en caso de que no existiera la clave).
 * 
 * Devuelve el diccionario o NULL en caso de error.
 */
diccionario_t *diccionario_insertar(diccionario_t *d, const char *clave,
				    void *valor, void **valor_anterior)
{
        if(!d || !clave)
                return NULL;

        struct nodo_diccionario *nodo_buscado = obtener_nodo(clave, d);

        if (nodo_buscado){
                if (nodo_buscado->par.valor)
                        *valor_anterior = nodo_buscado->par.valor;
                nodo_buscado->par.valor = valor;
                return d;
        }

        if ((d->cantidad / d->capacidad) >= FACTOR_CARGA){
                bool rehasheo_exitoso = rehashear(d);
                if (!rehasheo_exitoso)
                        return NULL;
        }

        struct nodo_diccionario *nuevo_nodo = calloc(1, sizeof(struct nodo_diccionario));
        if (!nuevo_nodo)
                return NULL;

        const char *clave_copia = malloc(strlen(clave) + 1);
        if (!clave_copia){
                free(nuevo_nodo);
                return NULL;
        }

        strcpy(clave_copia, clave);
        nuevo_nodo->par.valor = valor;
        
        size_t posicion = hash_djb2(clave) % d->capacidad;

        nuevo_nodo->siguiente = d->tabla[posicion];
        d->tabla[posicion] = nuevo_nodo;
        *valor_anterior = NULL; 
        return d;
}

size_t diccionario_con_cada_elemento(diccionario_t *d,
				     bool (*f)(struct diccionario_par *,
					       void *),
				     void *extra)
{
        if (!d || !f || d->cantidad == 0)
                return 0;

        size_t cant_evaluados = 0;
        size_t capacidad = d->capacidad;
        bool seguimos = true;

        for (size_t i; i < capacidad && seguimos; i++)
        {
                struct nodo_diccionario *nodo_actual = d->tabla[i];

                while (nodo_actual != NULL && seguimos){
                        seguimos = f(&(nodo_actual->par), extra);
                        cant_evaluados++;
                        nodo_actual = nodo_actual->siguiente;
                }          
        }

        return cant_evaluados;
}