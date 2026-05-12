#include "utils.h"
#include "constantes.h"

bool es_numero_valido(const char *cadena, int *valor)
{
	if (!cadena || cadena[0] == '\0')
		return false;

	char *endptr;
	long valor_convertido = 0;

	valor_convertido = strtol(cadena, &endptr, 10);

	if (cadena == endptr || *endptr != '\0')
		return false;

	*valor = (int)valor_convertido;

	return true;
}

void vector_destruir(struct vector *vector)
{
	if (!vector)
		return;

	size_t cant_palabras = vector->cantidad;

	for (size_t i = 0; i < cant_palabras; i++)
		free(vector->palabras[i]);

	free(vector->palabras);
	free(vector);
}

/*
 * PRE: 'vector_destino' y 'fuente' deben ser válidos (no NULL). 'pos_final' debe ser mayor o igual a 'pos_inicio'.
 * POST: Se extrae el substring de 'fuente' delimitado por pos_inicio y pos_final, se reserva memoria para él,
 * y lo agrega al final de 'vector_destino'. Devuelve true en caso de éxito, o false si falla la memoria.
 */
bool cargar_subpalabra(struct vector *vector_destino, char *fuente,
		       size_t pos_inicio, size_t pos_final)
{
	if (!vector_destino || !(vector_destino->palabras) || !fuente ||
	    pos_final < pos_inicio)
		return false;

	size_t largo_subpalabra = pos_final - pos_inicio;

	char *subpalabra_aux = malloc((largo_subpalabra + 1) * sizeof(char));

	if (!subpalabra_aux)
		return false;

	memcpy(subpalabra_aux, fuente + pos_inicio, largo_subpalabra);

	subpalabra_aux[largo_subpalabra] = '\0';

	vector_destino->palabras[vector_destino->cantidad] = subpalabra_aux;
	vector_destino->cantidad++;

	return true;
}

/*
 * PRE: 'n_palabras_iniciales' indica la capacidad inicial del arreglo interno de palabras.
 * POST: Reserva memoria dinámica para una estructura vector y para su arreglo de punteros a char. 
 * Devuelve el puntero al vector inicializado. Si ocurre algún error de memoria en cualquiera de los pasos, devuelve NULL.
 */
struct vector *crear_vector_para_n_palabras(size_t n_palabras_iniciales)
{
	struct vector *nuevo_vector = calloc(1, sizeof(struct vector));

	if (!nuevo_vector)
		return NULL;

	char **palabras_aux = calloc(n_palabras_iniciales, sizeof(char *));

	if (!palabras_aux) {
		free(nuevo_vector);
		return NULL;
	}

	nuevo_vector->palabras = palabras_aux;

	return nuevo_vector;
}

struct vector *split(char *texto, char caracter_separador)
{
	if (!texto)
		return NULL;

	bool salir = false;
	size_t largo_texto = strlen(texto);
	size_t pos_inicio_palabra = 0;
	size_t pos_final_palabra = 0;

	struct vector *vector_resultado =
		crear_vector_para_n_palabras(largo_texto + 1);

	if (!vector_resultado)
		return NULL;

	for (size_t i = 0; !salir && i <= largo_texto; i++) {
		if (texto[i] == caracter_separador || i == largo_texto) {
			pos_final_palabra = i;

			if (!cargar_subpalabra(vector_resultado, texto,
					       pos_inicio_palabra,
					       pos_final_palabra))
				salir = true;

			pos_inicio_palabra = pos_final_palabra + 1;
		}
	}

	if (salir) {
		vector_destruir(vector_resultado);
		return NULL;
	}

	char **buffer_ajustado =
		realloc(vector_resultado->palabras,
			vector_resultado->cantidad * sizeof(char *));

	if (!buffer_ajustado) {
		vector_destruir(vector_resultado);
		return NULL;
	}

	vector_resultado->palabras = buffer_ajustado;
	return vector_resultado;
}

estado_lectura_t leer_linea(FILE *archivo, char **linea)
{
	if (!archivo) {
		return ERROR;
	}

	size_t tamanio_buffer = TAMANIO_INICIAL;

	char *buffer = malloc(tamanio_buffer * sizeof(char));
	if (!buffer) 
		return ERROR_MEMORIA;

	bool error_memoria = false;
	size_t ocupado = 0;
	bool encontramos_salto_de_linea = false;

	while (!(error_memoria) && !encontramos_salto_de_linea &&
	       fgets(buffer + ocupado, (int)(tamanio_buffer - ocupado),
		     archivo) != NULL) {
		ocupado += strlen(buffer + ocupado);

		if (buffer[ocupado - 1] == '\n') {
			encontramos_salto_de_linea = true;
			buffer[ocupado - 1] = '\0';
			ocupado--;
		} else {
			char *buffer_aux = realloc(
				buffer, tamanio_buffer * 2 * sizeof(char));
			if (!buffer_aux) {
				error_memoria = true;
			} else {
				buffer = buffer_aux;
				tamanio_buffer = tamanio_buffer * 2;
			}
		}
	}

	if (!encontramos_salto_de_linea && ocupado == 0) {
		free(buffer);
		return FIN_DE_ARCHIVO;
	}

	if (error_memoria) {
		free(buffer);
		return ERROR_MEMORIA;
	}

	char *buffer_ajustado = realloc(buffer, (ocupado + 1) * sizeof(char));

	if (!buffer_ajustado) {
		free(buffer);
		return ERROR_MEMORIA;
	}

	*linea = buffer_ajustado;
	return LECTURA_OK;
}