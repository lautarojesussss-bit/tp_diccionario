#include "pa2m.h"
#include <stdio.h>
#include <../src/diccionario.h>

static bool siempre_true(struct diccionario_par *, void *)
{
        return true;
}

//debería devolver 3 como capacidad
void prueba_diccionario_crear()
{
        diccionario_t *nuevo_diccionario = diccionario_crear(3);
        pa2m_afirmar(nuevo_diccionario != NULL & diccionario_cantidad(nuevo_diccionario) == 0, "Crear un nuevo diccionario y que esté vacío.");
        diccionario_destruir(nuevo_diccionario);
}

void prueba_diccionario_manejar_NULL()
{
        diccionario_insertar(NULL, NULL, NULL, NULL);
        diccionario_eliminar(NULL, NULL);
        diccionario_obtener(NULL, NULL);
        diccionario_existe(NULL, NULL);
        diccionario_cantidad(NULL);
        diccionario_con_cada_elemento(NULL, NULL, NULL);
        diccionario_destruir_todo(NULL, NULL);
        diccionario_destruir(NULL);
}

void prueba_diccionario_insertar()
{
        diccionario_t *nuevo_diccionario = diccionario_crear(3);
        const char *CLAVE = "Primero";
        int valor = 0;
        diccionario_insertar(nuevo_diccionario, CLAVE, &valor, NULL);
        bool existe = diccionario_existe(nuevo_diccionario, CLAVE);
        size_t cantidad = diccionario_cantidad(nuevo_diccionario);
        int *valor_recuperado = diccionario_obtener(nuevo_diccionario, CLAVE);
        pa2m_afirmar(existe && cantidad == 1 && valor_recuperado == &valor, "Cargar un par clave-valor sin errores." );
        diccionario_destruir(nuevo_diccionario);
}

void prueba_diccionario_eliminar()
{
        diccionario_t *nuevo_diccionario = diccionario_crear(3);
        const char *CLAVE = "Primero";
        int valor = 0;
        diccionario_insertar(nuevo_diccionario, CLAVE, &valor, NULL);
        int *valor_recuperado = diccionario_eliminar(nuevo_diccionario, CLAVE);
        pa2m_afirmar(valor_recuperado == &valor && diccionario_cantidad(nuevo_diccionario) == 0, "Eliminar un clave éxitosamente.");
        diccionario_destruir(nuevo_diccionario);
}

void prueba_diccionario_con_cada_elemento()
{
        diccionario_t *nuevo_diccionario = diccionario_crear(3);
        const char *CLAVE1 = "Primero";
        const char *CLAVE2 = "Segundo";
        const char *CLAVE3 = "Tercero";
        int valores[3] = {1,2,3};
        diccionario_insertar(nuevo_diccionario, CLAVE1, &(valores[0]), NULL);
        diccionario_insertar(nuevo_diccionario, CLAVE2, &(valores[1]), NULL);
        diccionario_insertar(nuevo_diccionario, CLAVE3, &(valores[2]), NULL);
        size_t cant_iterados = diccionario_con_cada_elemento(nuevo_diccionario, siempre_true, NULL);
        pa2m_afirmar(cant_iterados == diccionario_cantidad(nuevo_diccionario), "Iterar todos los pares de un diccionario al user una callback que siempre devuelve true.");
        diccionario_destruir(nuevo_diccionario);
}

void prueba_diccionario_insertar_editar_valor()
{
        diccionario_t *nuevo_diccionario = diccionario_crear(3);
        const char *CLAVE = "Primero";
        int valor_original = 1;
        diccionario_insertar(nuevo_diccionario, CLAVE, &valor_original, NULL);
        int valor_nuevo = 2;
        int *valor_original_recuperado;
        diccionario_insertar(nuevo_diccionario, CLAVE, &valor_nuevo, &valor_original_recuperado);
        int *nuevo_valor_recuperado = diccionario_obtener(nuevo_diccionario, CLAVE);
        pa2m_afirmar(valor_original_recuperado == &valor_original && nuevo_valor_recuperado == &valor_nuevo && diccionario_cantidad(nuevo_diccionario)== 1, "Editar el valor de una clave y recuperar el original.");
        diccionario_destruir(nuevo_diccionario);
}


int main() {
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP X ===============");

	pa2m_nuevo_grupo("Pruebas de Suma del TDA X");
	pruebas_unitarias_sumar();

	return pa2m_mostrar_reporte();
}
