#ifndef CONSTANTES_H_
#define CONSTANTES_H_

//tp1.c
#define CANT_DATOS 5
#define SEPARADOR ','
#define TIPOS_CANT 8
#define CANT_METRICAS_POKEMON 3



//utils.c
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"
#define TAMANIO_INICIAL 101

//main.c
#define ERROR -1
#define CANT_ARGUMENTOS 4
#define ACCION_1 "mostrar"
#define ACCION_2 "buscar"
#define ACCION_1_FORMA_1 "nombre"
#define ACCION_1_FORMA_2 "tipo"
#define CANT_TIPOS 8
#define FORMATO_IMPRESION \
	"Nombre: %-15s | Tipo: %-10s | Ataque: %-3d | Defensa: %-3d | Velocidad: %-3d\n"
#define COLOR_BLANCO "\x1b[37;1m"
#define COLOR_VERDE "\x1b[32;1m"
#define COLOR_ROJO "\x1b[31;1m"
#define COLOR_AMARILLO "\x1b[33;1m"
#define COLOR_NORMAL "\x1b[0m"

//pruebas_alumno.c
#define CANT_POKEMONES_PRUEBA 5
#define VELOCIDAD_MINIMA 20

#endif //CONSTANTES_H_