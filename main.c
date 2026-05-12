#include <stdlib.h>
#include <stdbool.h>
#include <../src/diccionario.h>
#include <../src/src_tp1/tp1.h>

int main(int argc, char const *argv[])
{
	return 0;
}

bool agregar_al_diccionario (struct pokemon *pokemon, void *diccionario)
{
        if (!pokemon || !diccionario)
                return false;

        diccionario_t *diccionario_recuperado = diccionario_insertar(diccionario, pokemon->nombre, pokemon, NULL);
        return (diccionario_recuperado != NULL);
}
