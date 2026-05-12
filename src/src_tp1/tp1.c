#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "utils.h"
#include "constantes.h"

const char *TIPOS_CSV[] = { "ELEC", "FUEG", "PLAN", "AGUA",
			    "NORM", "FANT", "PSI",  "LUCH" };

struct tp1 {
	struct pokemon **pokemones_nombre;
	struct pokemon **pokemones_tipo[TIPOS_CANT];
	size_t cantidad_total;
	size_t cant_tipos[TIPOS_CANT];
};

/*
 * PRE: pokemon debe ser un puntero válido y archivo debe ser un puntero a un FILE abierto en modo escritura.
 * POST: Escribe los datos del pokemon en el archivo siguiendo el formato especificado en FORMATO_ESCRITURA.
 * Devuelve true si los parámetros son válidos, false en caso contrario.
 */
bool escribir_pokemon(struct pokemon *pokemon, void *archivo)
{
	if (!pokemon || !archivo || !(pokemon->nombre))
		return false;

	fprintf(archivo, FORMATO_ESCRITURA, pokemon->nombre,
		TIPOS_CSV[pokemon->tipo], pokemon->ataque, pokemon->defensa,
		pokemon->velocidad);

	return true;
}

/*
 * PRE: tp1 debe ser un tp1 válido y archivo debe ser un puntero a un FILE abierto en modo escritura.
 * POST: Recorre todos los pokemones del TDA y los escribe en el archivo.
 */
void escribir_pokemones(tp1_t *tp1, FILE *archivo)
{
	if (!tp1 || !archivo || tp1->cantidad_total == 0)
		return;

	tp1_con_cada_pokemon(tp1, escribir_pokemon, archivo);
}

/*
 * PRE: pokemones no debe ser NULL y error_memoria debe ser false. Los sub-arreglos [pos_inicio..pos_mitad] 
 * y [pos_mitad+1..pos_fin] deben estar ordenados alfabéticamente.
 * POST: Mezcla ambos sub-arreglos de forma ordenada en el arreglo original. 
 * En caso de fallo de memoria, actualiza *error_memoria a true.
 */
void merge_alfabetico(struct pokemon **pokemones, size_t pos_inicio,
		      size_t pos_mitad, size_t pos_fin, bool *error_memoria)
{
	if (!pokemones || !error_memoria || *error_memoria)
		return;

	size_t i = pos_inicio;
	size_t j = pos_mitad + 1;
	size_t k = 0;

	size_t tamanio = pos_fin - pos_inicio + 1;

	struct pokemon **pokemones_aux =
		malloc((tamanio) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return;
	}

	while (i <= pos_mitad && j <= pos_fin) {
		if (strcasecmp(pokemones[i]->nombre, pokemones[j]->nombre) <=
		    0) {
			pokemones_aux[k] = pokemones[i];
			i++;
		} else {
			pokemones_aux[k] = pokemones[j];
			j++;
		}

		k++;
	}

	while (i <= pos_mitad) {
		pokemones_aux[k] = pokemones[i];
		i++;
		k++;
	}

	while (j <= pos_fin) {
		pokemones_aux[k] = pokemones[j];
		j++;
		k++;
	}

	for (k = 0; k < tamanio; k++)
		pokemones[pos_inicio + k] = pokemones_aux[k];

	free(pokemones_aux);
}

/*
 * PRE: pokemones no debe ser NULL y error_memoria debe ser false.
 * POST: Ordena recursivamente el sub-arreglo delimitado por pos_inicio y 'pos_fin'. 
 * Si *error_memoria es true, se aborta.
 */
void merge_sort_alfabetico(struct pokemon **pokemones, size_t pos_inicio,
			   size_t pos_fin, bool *error_memoria)
{
	if (!pokemones || !error_memoria)
		return;

	if (pos_fin > pos_inicio) {
		size_t pos_mitad = pos_inicio + (pos_fin - pos_inicio) / 2;
		merge_sort_alfabetico(pokemones, pos_inicio, pos_mitad,
				      error_memoria);
		merge_sort_alfabetico(pokemones, pos_mitad + 1, pos_fin,
				      error_memoria);

		if (!(*error_memoria))
			merge_alfabetico(pokemones, pos_inicio, pos_mitad,
					 pos_fin, error_memoria);
	}
}

/*
 * PRE: pokemones no deber NULL y error_memoria debe ser false. cantidad >= 2.
 * POST: Ordena el arreglo de punteros alfabéticamente (de menor a mayor). 
 * Si ocurre un error, actualiza *error_memoria a true.
 */
void ordenar_alfabeticamente(struct pokemon **pokemones, bool *error_memoria,
			     size_t cantidad)
{
	if (!pokemones || !error_memoria || cantidad < 2 || *error_memoria)
		return;

	merge_sort_alfabetico(pokemones, 0, cantidad - 1, error_memoria);
}

/*
 * PRE: 'datos_separados' debe ser un struct vector válido creado por la función split. 'tipo' y 'metricas' deben ser punteros válidos.
 * POST: Valida que el vector contenga datos convertibles (un tipo válido y 3 métricas enteras).
 * Si el formato es correcto, guarda los valores convertidos en '*tipo' y '*metricas' y devuelve true.
 * Si el formato es inválido, devuelve false.
 */
bool formato_es_correcto(struct vector *datos_separados, int *tipo,
			 int *metricas)
{
	if (!datos_separados || !tipo || !metricas)
		return false;

	bool son_correctos_nums = true;
	int valor_leido = 0;

	for (unsigned int i = 2;
	     son_correctos_nums && i < datos_separados->cantidad; i++)
		if (es_numero_valido(datos_separados->palabras[i],
				     &valor_leido))
			metricas[i - 2] = valor_leido;
		else
			son_correctos_nums = false;

	for (int i = 0; *tipo == -1 && i < TIPOS_CANT; i++) {
		if (strcmp(datos_separados->palabras[1], TIPOS_CSV[i]) == 0)
			*tipo = i;
	}

	return (son_correctos_nums && *tipo != -1);
}

/*
 * PRE: linea debe ser un string con formato CSV. error_memoria debe apuntar a un bool inicializado en false.
 * POST: Analiza la línea, extrae los datos y devuelve un puntero a una nueva estructura pokemon alojada dinámicamente.
 * Si la línea tiene un formato inválido o si ocurre un fallo de memoria, devuelve NULL.
 * En caso de fallo de memoria, actualiza *error_memoria a true.
 */
struct pokemon *parsear_linea(char *linea, bool *error_memoria)
{
	if (!linea || !error_memoria || *error_memoria)
		return NULL;

	struct pokemon *pokemon_aux = malloc(sizeof(struct pokemon));
	if (!pokemon_aux) {
		*error_memoria = true;
		return NULL;
	}

	pokemon_aux->nombre = NULL;

	int tipo = -1;
	int metricas[CANT_METRICAS_POKEMON] = { -1, -1, -1 };

	struct vector *datos_separados = split(linea, SEPARADOR);

	if (!datos_separados) {
		free(pokemon_aux);
		*error_memoria = true;
		return NULL;
	}

	if (datos_separados->cantidad != CANT_DATOS ||
	    !formato_es_correcto(datos_separados, &tipo, metricas)) {
		free(pokemon_aux);
		vector_destruir(datos_separados);
		return NULL;
	}

	char *nombre = malloc(strlen(datos_separados->palabras[0]) + 1);
	if (!nombre) {
		free(pokemon_aux);
		vector_destruir(datos_separados);
		*error_memoria = true;
		return NULL;
	}

	strcpy(nombre, datos_separados->palabras[0]);

	pokemon_aux->nombre = nombre;
	pokemon_aux->tipo = tipo;
	pokemon_aux->ataque = metricas[0];
	pokemon_aux->defensa = metricas[1];
	pokemon_aux->velocidad = metricas[2];

	vector_destruir(datos_separados);
	return pokemon_aux;
}

/*
 * PRE: 'pokemones' debe ser un puntero a un arreglo dinámico válido. 'pokemon_aux' debe ser un pokemon válido.
 * 'cantidad' y 'tam_buffer' deben apuntar a las variables de control del arreglo. 'error_memoria' debe estar inicializado en false.
 * POST: Inserta 'pokemon_aux' al final del arreglo. Si se alcanza la capacidad, redimensiona de forma amortizada.
 * Devuelve true si la inserción fue exitosa, o false si ocurrió un error de memoria (y actualiza '*error_memoria' a true).
 */
bool agregar_pokemon(struct pokemon ***pokemones, struct pokemon *pokemon_aux,
		     bool *error_memoria, size_t *cantidad, size_t *tam_buffer)
{
	if (!pokemones || !pokemon_aux || !error_memoria || *error_memoria ||
	    !cantidad || !tam_buffer)
		return false;

	size_t nuevo_tam = 0;

	if (*tam_buffer == *cantidad) {
		nuevo_tam = (*tam_buffer == 0) ? 2 : *tam_buffer * 2;
		struct pokemon **pokemones_aux = realloc(
			pokemones[0], nuevo_tam * sizeof(struct pokemon *));

		if (!pokemones_aux) {
			*error_memoria = true;
			return false;
		}

		pokemones[0] = pokemones_aux;
		*tam_buffer = nuevo_tam;
	}

	pokemones[0][*cantidad] = pokemon_aux;
	(*cantidad)++;

	return true;
}

/*
 * PRE: 'pokemones' debe estar ordenado alfabéticamente, y 'nombre' debe ser un string válido.
 * POST: Busca un pokemon por su nombre.
 * Si lo encuentra, devuelve un puntero a dicho pokemon. Si no lo encuentra, devuelve NULL.
 */
struct pokemon *busqueda(struct pokemon **pokemones, int pos_inicio,
			 int pos_fin, const char *nombre)
{
	if (pos_fin < pos_inicio || !pokemones || !nombre)
		return NULL;

	int centro = pos_inicio + ((pos_fin - pos_inicio) / 2);

	int comparacion = strcasecmp(nombre, pokemones[centro]->nombre);

	if (comparacion == 0)
		return pokemones[centro];

	if (comparacion < 0)
		return busqueda(pokemones, pos_inicio, centro - 1, nombre);
	else
		return busqueda(pokemones, centro + 1, pos_fin, nombre);
}

/*
 * PRE: -
 * POST: Reserva memoria y devuelve una instancia de tp1_t inicializado en cero. Si falla la memoria, devuelve NULL.
 */
tp1_t *tp1_crear()
{
	tp1_t *tp1_aux = calloc(1, sizeof(tp1_t));
	if (!tp1_aux)
		return NULL;

	return tp1_aux;
}

/**
 * Libera toda la memoria asociada al tp1
 */
void tp1_destruir(tp1_t *tp1)
{
	if (!tp1)
		return;

	size_t cant = tp1->cantidad_total;

	for (size_t i = 0; i < cant; i++) {
		free(tp1->pokemones_nombre[i]->nombre);
		free(tp1->pokemones_nombre[i]);
	}

	for (size_t j = 0; j < TIPOS_CANT; j++)
		free(tp1->pokemones_tipo[j]);

	free(tp1->pokemones_nombre);
	free(tp1);
}

/*
 * PRE: 'tp' debe estar ordenado alfabéticamente, y 'error_memoria' en false.
 * POST: Elimina duplicados (conservando el primero en dicho caso).
 * Cuenta la cantidad de pokemones por tipo (guardando los datos en el campo cant_tipos del tp1) y ajusta el tamaño del arreglo principal (y su tope).
 * Si ocurre un error de memoria, actualiza '*error_memoria' a true.
 */
void limpiar_y_contar(tp1_t *tp, bool *error_memoria)
{
	if (!tp || !error_memoria || *error_memoria || tp->cantidad_total == 0)
		return;

	tp->cant_tipos[tp->pokemones_nombre[0]->tipo]++;

	size_t pos_lectura = 1;
	size_t pos_escritura = 0;

	size_t cant_original = tp->cantidad_total;

	while (pos_lectura < cant_original &&
	       pos_escritura < cant_original - 1) {
		struct pokemon *pokemon_aux = tp->pokemones_nombre[pos_lectura];

		if (strcasecmp(pokemon_aux->nombre,
			       tp->pokemones_nombre[pos_escritura]->nombre) ==
		    0) {
			free(pokemon_aux->nombre);
			free(pokemon_aux);
		} else {
			tp->pokemones_nombre[pos_escritura + 1] =
				tp->pokemones_nombre[pos_lectura];
			tp->cant_tipos[tp->pokemones_nombre[pos_lectura]->tipo]++;
			pos_escritura++;
		}
		pos_lectura++;
	}

	tp->cantidad_total = (pos_escritura + 1);

	struct pokemon **pokemones_nombre_aux =
		realloc(tp->pokemones_nombre,
			(pos_escritura + 1) * sizeof(struct pokemon *));

	if (!pokemones_nombre_aux) {
		*error_memoria = true;
		return;
	}

	tp->pokemones_nombre = pokemones_nombre_aux;
}

/*
 * PRE: 'tp' no debe tener duplicados y su arreglo 'cant_tipos' debe estar inicializado. 'error_memoria' debe estar inicializado en false.
 * POST: Reserva la memoria exacta para los sub-arreglos y distribuye los punteros del arreglo principal según su tipo.
 * Si ocurre un error de memoria, actualiza '*error_memoria' a true.
 */
void clasificar_por_tipo(tp1_t *tp, bool *error_memoria)
{
	if (!tp || !error_memoria || *error_memoria)
		return;

	for (size_t i = 0; !(*error_memoria) && i < TIPOS_CANT; i++) {
		tp->pokemones_tipo[i] =
			malloc(tp->cant_tipos[i] * sizeof(struct pokemon *));
		tp->cant_tipos[i] = 0;

		if (!(tp->pokemones_tipo[i]))
			*error_memoria = true;
	}

	if (*error_memoria)
		return;

	struct pokemon *pokemon_aux = NULL;
	size_t cant = tp->cantidad_total;

	for (size_t j = 0; j < cant; j++) {
		pokemon_aux = tp->pokemones_nombre[j];
		tp->pokemones_tipo[pokemon_aux->tipo]
				  [tp->cant_tipos[pokemon_aux->tipo]] =
			pokemon_aux;
		tp->cant_tipos[pokemon_aux->tipo]++;
	}
}

/*
 * PRE: 'tp' debe ser válido. 'archivo' debe ser un puntero a un FILE abierto en modo lectura. 'error_memoria' inicializada en false.
 * POST: parsea los pokemones válidos del archivo y los agrega al arreglo principal del tp1 (pokemon_nombre).
 * Si ocurre un error de memoria durante la lectura o inserción, actualiza '*error_memoria' a true.
 */
void cargar_en_bruto(tp1_t *tp, FILE *archivo, bool *error_memoria)
{
	if (!tp || !archivo || !error_memoria || *error_memoria)
		return;

	bool termino_el_archivo = false;
	struct pokemon *pokemon_aux = NULL;
	size_t tam_buffer = 0;

	while ((!*error_memoria) && !termino_el_archivo) {
		char *linea = 

			leer_linea(archivo, &linea);

		if (linea != NULL)
			pokemon_aux = parsear_linea(linea, error_memoria);

		free(linea);

		if (pokemon_aux != NULL) {
			if (!agregar_pokemon(&(tp->pokemones_nombre),
					     pokemon_aux, (error_memoria),
					     &(tp->cantidad_total),
					     &tam_buffer)) {
				free(pokemon_aux->nombre);
				free(pokemon_aux);
			}
		}
		pokemon_aux = NULL;
	}
}

/**
 * Lee el archivo indicado y devuelve la estructura tp1 con los pokemones.
 * En caso de error devuelve NULL.
 * Tengo que abrir un archivo, leerlo con cierto formato ()
*/
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (!nombre)
		return NULL;

	FILE *archivo = fopen(nombre, "r");

	if (!archivo)
		return NULL;

	tp1_t *tp = tp1_crear();

	if (!tp) {
		fclose(archivo);
		return NULL;
	}

	bool error_memoria = false;

	cargar_en_bruto(tp, archivo, &error_memoria);

	fclose(archivo);

	ordenar_alfabeticamente(tp->pokemones_nombre, &error_memoria,
				tp->cantidad_total);

	limpiar_y_contar(tp, &error_memoria);

	clasificar_por_tipo(tp, &error_memoria);

	if (error_memoria) {
		tp1_destruir(tp);
		return NULL;
	}

	return tp;
}

/**
 * Devuevle la cantidad de pokemones leidos correctamente.
 * En caso de error devuelve 0.
*/
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1)
		return 0;

	return tp1->cantidad_total;
}

/**
 * Guarda en el archivo indicado los pokemones contenidos en la estructura tp1 
 * de manera tal que tp1_leer_archivo pueda volver a leerlo correctamente.
 *
 * Devuelve NULL en caso de error o el tp1 pasado por parámetro en caso de exito.
*/
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (!tp1 || !nombre)
		return NULL;

	FILE *archivo = fopen(nombre, "w");
	if (!archivo)
		return NULL;

	escribir_pokemones(tp1, archivo);

	fclose(archivo);

	return tp1;
}

/**
* Devuelve el n-esimo pokemon por orden alfabetico (de menor a mayor).
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_orden(tp1_t *tp, int n)
{
	if (!tp || n >= tp->cantidad_total || n < 0)
		return NULL;

	return tp->pokemones_nombre[n];
}

/*
 * PRE: 'pokemon' debe ser un puntero válido. 'error_memoria' debe estar inicializado en false.
 * POST: Crea y devuelve una copia profunda (nueva memoria) del pokemon recibido.
 * Si ocurre un error de memoria, devuelve NULL y actualiza '*error_memoria' a true.
 */
struct pokemon *crear_copia_pokemon(struct pokemon *pokemon,
				    bool *error_memoria)
{
	if (!pokemon || !error_memoria || *error_memoria)
		return NULL;

	size_t len_nombre = strlen(pokemon->nombre);
	char *nombre_aux = malloc(sizeof(char) * (len_nombre + 1));

	if (!nombre_aux) {
		*error_memoria = true;
		return NULL;
	}

	struct pokemon *pokemon_copia_aux = malloc(sizeof(struct pokemon));
	if (!pokemon_copia_aux) {
		*error_memoria = true;
		free(nombre_aux);
		return NULL;
	}

	*pokemon_copia_aux = *pokemon;
	memcpy(nombre_aux, pokemon->nombre, (len_nombre + 1) * sizeof(char));
	pokemon_copia_aux->nombre = nombre_aux;

	return pokemon_copia_aux;
}

/*
 * PRE: 'tp_destino' debe ser un tp1_t válido. 'fuente' debe ser un arreglo de pokemones origen válido. 
 * 'cant' debe ser el tamaño de 'fuente'.
 * POST: Realiza copias profundas de todos los elementos de 'fuente' y los almacena en 'tp_destino'.
 * Devuelve true si la carga fue exitosa, o false si ocurrió un error de memoria.
 */
bool cargar_copias(tp1_t *tp_destino, struct pokemon **fuente, size_t cant,
		   enum tipo_pokemon tipo)
{
	if (!tp_destino || !fuente)
		return false;

	bool error_memoria = false;
	struct pokemon *pokemon_copia = NULL;

	for (size_t i = 0; !error_memoria && i < cant; i++) {
		pokemon_copia = crear_copia_pokemon(fuente[i], &error_memoria);
		if (pokemon_copia != NULL) {
			tp_destino->pokemones_nombre[i] = pokemon_copia;
			tp_destino->pokemones_tipo[tipo][i] = pokemon_copia;
			tp_destino->cantidad_total++;
			tp_destino->cant_tipos[tipo]++;
		}
	}

	return !error_memoria;
}

/*
 * PRE: tp fuente y tp_destino deben ser punteros a tp1_t válidos
 * POST: copia desde tp_fuente y hacia tp_destino los pokemons que tengan el tipo recibido.
 * En caso de error de vuelve false, en caso de éxito devuelve true.
 */
bool copiar_pokemones_filtrados(tp1_t *tp_fuente, tp1_t *tp_destino,
				enum tipo_pokemon tipo)
{
	if (!tp_fuente || !tp_destino)
		return false;

	size_t cant = tp_fuente->cant_tipos[tipo];

	if (cant == 0)
		return true;

	struct pokemon **pokemones_aux = calloc(cant, sizeof(struct pokemon *));

	if (!pokemones_aux)
		return false;

	struct pokemon **pokemones_tipo_aux =
		calloc(cant, sizeof(struct pokemon *));

	if (!pokemones_tipo_aux) {
		free(pokemones_aux);
		return false;
	}

	tp_destino->pokemones_nombre = pokemones_aux;
	tp_destino->pokemones_tipo[tipo] = pokemones_tipo_aux;

	bool carga_exitosa = cargar_copias(
		tp_destino, tp_fuente->pokemones_tipo[tipo], cant, tipo);

	return carga_exitosa;
}

/**
* Dado un tp1 y un tipo, devuelve otro tp1 conteniendo solamente los pokemons de dicho tipo.
*
* En caso de error devuelve NULL.
*/
tp1_t *tp1_filtrar_tipo(tp1_t *un_tp, enum tipo_pokemon tipo)
{
	if (!un_tp)
		return NULL;

	tp1_t *tp_filtrado = tp1_crear();

	if (!tp_filtrado)
		return NULL;

	bool filtrado_exitoso =
		copiar_pokemones_filtrados(un_tp, tp_filtrado, tipo);

	if (!filtrado_exitoso) {
		tp1_destruir(tp_filtrado);
		return NULL;
	}

	return tp_filtrado;
}

/**
* Busca un pokemon por nombre.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (!tp || !nombre || tp->cantidad_total == 0)
		return NULL;

	return busqueda(tp->pokemones_nombre, 0, (int)(tp->cantidad_total - 1),
			nombre);
}

/**
 * Aplica la función f a cada pokemon por orden alfabetico (de menor a mayor).
 * La función deja de aplicarse si f devuelve false o se terminaron los pokemones.
 * 
 * Devuelve la cantidad de pokemones sobre los cuales se aplicó la función f.
 */
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	if (!un_tp || !(un_tp->pokemones_nombre) || !f)
		return 0;

	bool continuar = true;
	size_t i = 0;
	size_t cant = un_tp->cantidad_total;

	for (; continuar && i < cant; i++)
		continuar = f(un_tp->pokemones_nombre[i], extra);

	return i;
}
