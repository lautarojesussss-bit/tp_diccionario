#include "diccionario.h"
#include "stdlib.h"

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
        size_t hash = 5381;
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
 * PRE: 'clave' debe ser un string no nulo, y 'dict' debe ser un diccionario válido
 * POST: devuelve el puntero al nodo que tiene la clave pasada, NULL si no se encuentra esa clave en el diccionario.
 */
static struct nodo_diccionario *obtener_nodo(const char *clave, diccionario_t *dict)
{
        if(!clave || !dict)
                return NULL;

        size_t posicion = hash_djb2(clave) % dict->capacidad;
        struct nodo_diccionario* nodo_aux = dict->tabla[posicion];

        while (nodo_aux != NULL 
                && strcmp(nodo_aux->par.clave, clave) != 0)
                nodo_aux = nodo_aux->siguiente;
        
        return nodo_aux;
}

/*
* Crea un diccionario vacío con una capacidad inicial dada. La capacidad nunca puede ser menor a 3.
*/
diccionario_t *diccionario_crear(size_t capacidad_inicial)
{
        diccionario_t *dict = calloc(1, sizeof(struct diccionario));
        
        if (!dict)
                return NULL;

        dict->tabla = calloc(capacidad_inicial, sizeof(struct nodo_diccionario*));

        if (!dict->tabla){
                free(dict);
                return NULL;
        }

        dict->capacidad = capacidad_inicial;
        return dict;
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

        
                
}