Introducción
============

Linguini es un lenguaje de programación sencillo y ligero, diseñado
principalmente para ser fácil de entender y escribir para las personas
hispanohablantes que inician en la programación. La sintaxis evita las
construcciones complejas, y en su lugar, usa muchas palabras clave
explícitas en vez de símbolos.

Linguini es principalmente de paradigma imperativo, pero a la vez está
completamente basado en objetos. Tiene un sistema de tipado fuerte, y
los programas son ejecutados en una máquina virtual.

Linguini es software libre y de código abierto, licenciado bajo la
[Licencia Pública General de GNU, versión
3.0](https://www.gnu.org/licenses/gpl-3.0.html) o posterior.

Historia
========

Inicialmente diseñé Linguini para cumplir con el proyecto final de
primer semestre de la clase de Fundamentos de Programación, en la
carrera de Ingeniería en computación, en la Universidad Nacional
Autónoma de México. La primera versión del lenguaje, 1.0, la entregué en
dicha asignación, pero a pesar de cumplir muy bien su función, su código
tiene varios defectos de diseño que deben ser corregidos.

La segunda versión, 2.0, corrige éstos y otros errores, a la vez que
añade nuevas características, y es la versión que entregaré como
proyecto final de tercer semestre de la clase de Estructuras de Datos y
Algoritmos II.

El nombre del lenguaje, Linguini, viene del apodo que el grupo de la
clase de primer semestre adjudicó al profesor que la impartía, aludiendo
al parecido que tiene con el personaje de [[Linguini
Gusteau]{.underline}]{.underline}, de la película
[[Ratatouille]{.underline}]{.underline}.

Conceptos básicos
=================

Valores
-------

Ámbito
------

En Linguini prácticamente todos los objetos pertenecen a un ámbito
espacial y temporal determinado, es decir, que casi ningún objeto puede
estar accesible siempre y desde cualquier punto: en general todos los
objetos pertenecen a un bloque, sea un módulo, un programa o una
función. Esto quiere decir que si declaramos una variable al inicio de
un programa, esta variable no estará disponible fuera de ese programa,
por ejemplo, en un módulo importado.

La única excepción son las clases incluidas de serie: `Objeto`,
`Entero`, `Decimal`, `Cadena`, `Lista`, `Funcion`, `Logico`, `Modulo` y
`Excepcion`: se pueden declarar objetos de estas clases desde cualquier
ubicación del programa.

Errores
-------

Metamétodos
-----------

  Metamétodo           Descripción
  -------------------- ------------------------------
  \_~suma~             Suma
  \_~resta~            Resta
  \_~multiplicacion~   Multiplicación
  \_~division~         División
  \_~resto~            Resto de una división entera
  \_~potencia~         Potencia
  \_~raiz~             Raíz
  \_~mayorque~         Comparación mayor que
  \_~menorque~         Comparación menor que
  \_~igualdad~         Igualdad
  \_~desigualdad~      Desigualdad
  \_~y~                Y lógico
  \_~o~                O lógico
  \_~cadena~           Conversión a Cadena
  \_~entero~           Conversión a Entero
  \_~flotante~         Conversión a Flotante
  \_~booleano~         Conversión a Booleano
  \_~lista~            Conversión a Lista
  \_~asignacion~       Asignación
  \_~creacion~         Inicialización
  \_~destruccion~      Destrucción
  \_~obtener~          Indizado
  \_~elementos~        Lista de elementos
  \_~tamano~           Tamaño
  \_~lectmetodo~       Lectura de un método
  \_~modmetodo~        Modificación de un método
  \_~elimmetodo~       Borrado de un método
  \_~copia~            Copia de un valor
  \_~iteracion~        Iteración

Sintaxis
========

Nomenclatura
------------

Variables
---------

Declaraciones
-------------

### Bloques

1.  Blo

### Instrucciones

Expresiones
-----------

### Operadores

Un operador \[agrupacion\] :() \^\_ \*/%

+- \<\> =\~ &\|

Tipo de archivo
---------------

Un archivo de código fuente en Linguini puede ser de dos tipos: un
programa o un módulo: los programas están diseñados para ejecutarse
directamente, mientras que los módulos no, sino que son para ser
importados para ser usados en otros programas.

### Programa

La estructura de un programa es como sigue:

``` {.bnf}
programa = "Programa " identifier newline+ sentencia*
```

### Módulo

``` {.bnf}
modulo = "Modulo " identifier newline+ (usos|newline)* "Expone" newline+ (declaraciones|newline)* "Implementa" newline+ (definiciones|asignara|newline)*
```

Lista de palabras reservadas
----------------------------

Declarar AsignarA Usa Como Fin Funcion Acepta Modifica Devuelve Clase
Extiende Expone Redefine Implementa Si PeroSi SiNo Mientras PorCada En
Escoger Caso Predeterminado Regresar Continuar Eliminar Intentar
Excepcion Finalizacion

ABNF
----

Compilador
==========

Avisos
------

Clave: W: permite, con advertencia; A: permite; E: no permite

redef
:   \(E\) Permite redefinir variables ya definidas

unusedobj
:   \(A\) Permite no usar objetos declarados

unusedparam
:   \(A\) Permite no usar parámetros declarados en una función

limits
:   \(A\) Permite usar valores que se salen de sus rangos

impconv
:   \(A\) Permite usar conversiones implícitas mediante metamétodos

impbool
:   \(E\) Permite usar cualquier valor como booleano en comparaciones

comparefloats
:   \(A\) Permite comparar valores flotantes

privmmethod
:   \(A\) Permite que los metamétodos sean privados

switchdef
:   \(A\) Permite usar un bloque Escoger sin un bloque Predeterminado

switchbool
:   \(A\) Permite usar un bloque Escoger comparando un valor lógico

all
:   refiere a todos los avisos

Opciones
--------

debug
:   Incluye información del código fuente original

mpath
:   Indica una o más rutas en dónde buscar módulos

Bytecode
--------

  Tamaño     Valor            Descripción
  ---------- ---------------- -------------------------------------------------------
  3          0x17 0x94 0x00   Número mágico (0x179400)
  1          0x01 o 0x02      Tipo de ejecutable (programa o módulo)
  1          \-               Tamaño del nombre del ejecutable (máx 256 caracteres)
  Variable   \-               Nombre del ejecutable
                              

k
-

``` {.plain}
Funcion [identificador]
  Recibe ([clase] [identificador][\n,])+
  Modifica ([clase] [identificador][\n,])+
  Devuelve [clase]
  [sentencias]
Fin

Clase [identificador] (Extiende [clase])?
  Expone ([clase] [identificador][\n,])+
  Implementa
    Funcion [identificador]
      [sentencias]
    Fin
  Redefine
    Funcion [identificador]
      [sentencias]
    Fin
Fin


Programa a
Usa "lista"
Usa "instagram" Como i

ES:imprimir("hola")

# En Linguini todas las variables se inicializan automáticamente al ser declaradas
# De ese modo, en Linguini no hay variables con valores basura
# Hay azúcar sintáctico para inicializar las clases básicas
# "a" -> cadena
# 3 -> entero
# 1.6 -> flotante
# Falso -> booleano
# [x] -> :obtener(x)
# Por ejemplo, un entero vale 0 inicialmente, un booleano vale False, y una cadena, ""

Declarar ES:Archivo f # Se inicializa, a partir de aquí es seguro usar f
f:abrir("datos.txt")
f:cerrar()


Declarar Cadena i # i = ""
Declarar Cadena j # j = ""
AsignarA j = "b"

# La asignación substituye el valor de una variable por el valor dado
# El valor que tenía la variable es destruido, y el valor dado es copiado a la memoria de la variable
AsignarA i "hola" # El valor "hola" es copiado en i
AsignarA i j # El valor de j es copiado en i

ES:imprimir(i:nombre)


Escoger [variable]
  Caso 1
    [sentencias]
  Fin
  Caso 2
    [sentencias]
  Fin
  Predeterminado
    [sentencias]
  Fin
Fin

PorCada x En i
  [sentencias]
Fin

Si [expresión]
  [sentencias]
PeroSi [expresión]
  [sentencias]
SiNo
  [sentencias]
Fin

Mientras [expresión]
  [sentencias]
Fin

Programa Chocolate

Intentar
Excepcion ArchivoNoEncontrado
  ES:imprimir("No se encontró el archivo")
Excepcion SinPermisos
  Lanzar
Finalizacion
  ES:imprimir("Esto siempre se imprime")
SiNo
  ES:imprimir("Todo salió bien")
Fin
```

Biblioteca estándar
===================

Clases incluidas en el sistema
------------------------------

### Objeto

### Entero

### Decimal

### Cadena

### Lista

### Funcion

### Logico

### Modulo

### Excepcion

Bibliotecas disponibles
-----------------------

### Otros tipos

1.  Complejo

2.  Tupla

3.  Conjunto

### Tiempo

Fecha, hora, zonas horarias, calendario

### Matemática

1.  abs()

2.  nan()

3.  exp()

4.  ln()

5.  log2()

6.  log(10)

7.  hipotenusa()

8.  sin()

9.  cos()

10. tan()

11. asin()

12. acos()

13. atan()

14. sinh()

15. cosh()

16. tanh()

17. asinh()

18. acosh()

19. atanh()

20. piso()

21. techo()

22. redondeo()

23. aleatorio()

### Sistema

Archivos, rutas

### E/S

1.  ImprimirF

2.  Persistencia

3.  Contrasen̄a

4.  Red

### \[\#C\] Compresión

1.  zlib

2.  gzip

3.  bz2

4.  lzma

5.  zip

6.  tar

### Formatos

1.  CSV

2.  TOML

3.  XML

4.  JSON

5.  HTML

6.  Gettext

7.  Dislines

### Criptográfico

1.  Hash

2.  Base64

### \[\#C\] Internet

1.  Email

2.  Mailbox

3.  Mimetypes

4.  CGI

5.  URL

6.  CGI

7.  Clientes

    1.  FTP

    2.  POP

    3.  IMAP

    4.  SMTP

    5.  HTTP

    6.  Telnet

### Internacionalización

1.  Locale

### \[\#C\] Tk

### Metalenguaje

1.  Parser

PC
==

homepage
