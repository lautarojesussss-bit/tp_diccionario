#include "pa2m.h"
#include <stdio.h>
#include <../src/diccionario.h>

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

int main() {
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP X ===============");

	pa2m_nuevo_grupo("Pruebas de Suma del TDA X");
	pruebas_unitarias_sumar();

	return pa2m_mostrar_reporte();
}
