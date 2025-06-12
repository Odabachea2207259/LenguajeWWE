# LenguajeWWE
Un lenguaje que cree de prueba para probar mis habilidades en la creación de traductores-compiladores
Este lenguaje sencillo fue hecho como proyecto para la materia de Traductores, se creó usando C y realiza
todas las fases de un compilador, pero en vez de darnos el archivo en lenguaje máquina, o en assembly, nos
lo retorna en lenguaje C para proceder con su compilación.

## Como usar
El lenguaje es muy sencillo de escribir, está muy inspirado en pseudocódigo, por lo que tenemos que señalar
el inicio y fin de las cosas. Por ejemplo, para el inicio y fin del programa se utiliza:

```
raw
  Enunciados
smackdown
```

### Variables
Tenemos dos tipos de variables en este lenguaje, *flotantes* y *listas de flotantes*, esto con la finalidad
de reducir la complejidad del compilador. A pesar que todas las variables sean iguales, debemos declararlas.

Para las variables normales usamos:
```
  nxt variable1 variable2 variable3
```

Para las listas es casi igual, pero usamos **nexus**:
```
  nexus lista1 lista2
```

> Como ven, en ninguno de los dos casos es necesario usar comas, con delimitarlos con espacio el compilador
sabrá que son distintas.

### Expresiones
El lenguaje admite expresiones tanto sencillas como complejas, ya sea de un solo elemento o más. Podemos realizar
operaciones como **12 + 21** o **variable + variable2 / 12**.

### Asignación
Para asignarle un valor a una variable lo hacemos de la manera:
```
  variable = valor
```
Donde valor puede ser, otra variable, un número o una expresión.
> Notese que no es necesario tampoco terminar ninguna línea con un delimitador, ya que este delimitador es el salto de línea.

### Leer desde consola
Para leer un valor desde una consola lo hacemos de diferentes formas para una lista o para una variable sencilla,
pero comparten la misma base, el uso de la palabra reservada **promo**.

*Variable sencilla*:
```
  variable = promo
```
*Lista*:
```
  lista <- promo
```

### Listas
Las listas tienen una característica particular, y es que no es necesario declararle un tamaño, conforme vayamos agregando cosas 
a la lista, ira incrementando de manera automática.
Como ya vimos, se puede realizar la asignación de una variable sencilla, pero para una lista es importante conocer
el tamaño de nuestra lista para no acceder a un valor que no existe.

Tenemos una manera de conocer tres datos importantes de nuestra lista.
- El tamaño
```
 lista.s
```
- El primer elemento
```
  lista.f
```
- El último elemento
```
  lista.l
```

Estos tres datos no pueden ser cambiados por el usuario, pero pueden usarse para inicializar otra variable:
```
  variable = lista.l + lista.s
```

Por último, tenemos una manera de añadir un valor al último lugar de nuestra lista, y es tan sencillo cómo:
```
  lista <- valor
```
Esto añadira el valor a la última posición de nuestra lista.

### Imprimir en consola
Para imprimir en consola debemos usar la palabra reservada **anunciar**, acompañado de los delimitadores
**->** y **<-**. El compilador admite imprimir palabras, variables y expresiones, pero no juntos, deben estar separados.
```
  anunciar -> "Hola mundo\n" <-
  anunciar -> variable <-
  anunciar -> 12 + 32 <-
```
> Como el lenguaje aun esta en una fase muy joven, el compilador detectará un error al querer imprimir una expresión,
> ya que no encontrará el primer número como una variable declarada. Todo compilará bien pero marcará el error.

### Condicionales
Al igual que en cualquier otro lenguaje, podemos utilizar la condicional *if*. En este caso, usamos las palabras
reservadas **jeff**, **matt** y **hardy**, donde *jeff* es el inicio del if que irá seguido de la comparación,
*matt* funciona como el else y *hardy* es el delimitador del final de la condicional:
```
  jeff variable < 134
    anunciar -> "Es menor\n" <-
  matt
    anunciar -> "Es mayor\n" <-
  hardy
```
> Las comparaciones permitidas no pueden ser de más de un valor por lado de la comparación por ahora.

### Ciclos
En el lenguaje WWE contamos con un ciclo que hace la función de un while. Para esto, utilizamos
**randy** para el inicio del while y **orton** para el final.
```
  randy i < 10
    anunciar -> i <-
    i = i + 1
  orton
```
> Las comparaciones permitidas no pueden ser de más de un valor por lado de la comparación por ahora.

## Ejemplos
En el apartado de ejemplos agregue varios programas creados con el lenguaje, para demostrar la capacidad
de este. Se puede realizar un bubble sort o un selection sort, también se pudo realizar el fibonacci, y
el programa de fizzbuzz.

## Uso del compilador
Para usar el compilador es necesario tener los archivos **Listas.c** y **Listas.h**, ya que estas funcionan
como libreria del lenguaje, como el caso de *stdio.h* o *stdlib.h* de C. 
Para compilar solo es:
```
.\wwe.exe nombre_programa.wwe -o nombre_salida
```
Todo programa debe ser terminado con *.wwe* para que el compilador sepa de cual estamos hablando. No es necesario
ligar los archivos de listas antes mencionados, ya que el compilador se encargará de eso.
