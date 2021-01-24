Linguini es un lenguaje de programación muy sencillo que escribí para el proyecto final de primer semestre de la clase de Fundamentos de Programación, en la carrera de Ingeniería en computación. No es necesaria ninguna biblioteca más allá de la biblioteca estándar de C. Está bajo la licencia GNU GPL versión 3 o superior.

# Opciones
* El parámetro `-i`, antes del archivo a ejecutar habilitael modo de depuración, y muestra paso a paso lo que hace cada instrucción del programa

# Compilación
He compilado usando `gcc -Wall -Wextra -Wpedantic -Wconversion -funsigned-char -s -Os -Wno-switch linguini.c -lm`

# Editores
* El archivo `linguini.el` implementa un modo en Emacs para Linguini
* El archivo `linguini-npp.xml` es un [UDL](https://npp-user-manual.org/docs/user-defined-language-system/) para el editor Notepad++
