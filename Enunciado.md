# TDA Diccionario

Se pide implementar un diccionario basado en una **Tabla de Hash abierta** (direccionamiento cerrado) en C. Para ello se brindan las firmas de las funciones públicas a implementar y **se deja a criterio del alumno la creación de las estructuras y funciones privadas del TDA** para el correcto funcionamiento de la **Tabla de Hash** cumpliendo con las buenas prácticas de programación. La función de **Hash** a utilizar será interna de la implementación y también debe ser decidida por el alumno. Para esta implementación las claves admitidas por la tabla serán solamente strings (no nulos). Adicionalmente se pide la creación de un iterador interno que sea capaz de recorrer las claves almacenadas en la tabla.


En el archivo **main.c** se debe implementar un programa que por línea de comandos reciba el nombre de un archivo y utilizando las primitivas del **TP1** cargue un archivo de pokemones en un diccionario. El programa debe implementar las funcionalidades de mostrar pokemon por nombre y por tipo **utilizando las primitivas del diccionario**  si es posible. Cuando no sea posible implementarlo de otra forma y explicar en el informe por qué. No se permite modificar los archivos de cabecera.

Se pide implementar los dos tipos de invocaciones mostrados a continuación:

  - **./diccionario pokedex.csv buscar nombre**: Buscar un pokemon por nombre y mostrarlo.
  - **./diccionario pokedex.csv buscar tipo**: Buscar todos los pokemon por tipo y mostrarlos.
  
Se permite utiliar solamente las primitivas del TP1 **tp1_leer_archivo**, **tp1_con_cada_pokemon** y **tp1_destruir**. Explique la complejidad de buscar un pokemon por nombre y de buscar todos los pokemon de un tipo.

El TDA entregado deberá compilar y pasar las pruebas dispuestas por la cátedra sin errores, adicionalmente estas pruebas deberán ser ejecutadas **sin pérdida de memoria**. Por último (y como siempre), se deben entregar las pruebas elaboradas por el alumno que verifiquen el correcto funcionamiento de la implementación realizada.

## Parte teórica

Explicá teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Qué es un diccionario
    - Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1 sola implementación posible)
   - Qué es una función de hash y qué características debe tener para nuestro problema en particular
   - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)
   - Explique por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)
     - Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?
   - Mas te vale que expliques con dibujos
