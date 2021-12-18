set -euo pipefail
IFS=$'\n\t'

#xpath -q -e '//span[@id="yfs_l10_goog"][1]/text()'

### Explicación
# -Wall -Wextra -Wpedantic -Werror → Me evita muchas malas prácticas
# -Wformat=2                       → Me avisa sobre errores en printf
# -Wshadow                         → Para evitar enmascaramientos de ámbito
# -Wwrite-strings                  → Pone const a las cadenas literales para avisar sobre modificaciones
# -Wstrict-prototypes              → Para declarar funciones completas
# -Wredundant-decls                → Me evita declarar dos veces lo mismo
# -Wnested-externs                 → Extern nunca debe ir dentro de una función
# -Wmissing-include-dirs           → Me avisa sobre errores al incluir directorios
# -Wjump-misses-init               → Me avisa de saltos sobre inicializaciones de variables
# -Wlogical-op                     → Me avisa de extrañezas con operadores lógicos
# -Wnormalized                     → Mi mapa de caracteres está modificado y genera caracteres NFD. Esto avisa si hay alguno de ellos en el código.
FLAGS=(-Wall -Wextra -Wpedantic -Werror -Wformat=2 -Wshadow -Wwrite-strings -Wstrict-prototypes -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wjump-misses-init -Wlogical-op -Wnormalized -std=c89)
LIBS=('lista' 'utilidades' 'expresion')
TARGET=('remy' 'linguini')

if [[ ${1:-v} == tests ]]; then
  echo "Estableciendo objetivos…"
  TARGET=('main')
fi

if [[ ${1:-v} == clean ]]; then
  echo "Limpiando…"
  #rm -rvf *.o ${TARGET[@]} *~
  rm -rvf bin/*
  rm -rvf *~
  rm -rvf *.html
  exit
fi

if [[ ${1:-v} == docs ]]; then
  echo "Documentando: Generando md de inicio…"
  command -v pandoc >/dev/null 2>&1
  if [[ $? == 0 ]]; then
    pandoc linguini.org -o linguini.md
  #else
    #echo >&2 "¡Necesito pandoc para generar la página de inicio de Doxygen!"
  fi


  doxygen Doxyfile
  exit
fi


# Compilación
command -v xpath >/dev/null 2>&1
if [[ $? == 0 ]]; then
  echo "libxml-xpath-perl instalado"
else
  echo >&2 "libxml-xpath-perl no está instalado"
  exit 1
fi

command -v xpath >/dev/null 2>&1
if [[ $? == 0 ]]; then
  echo "libxml2-dev instalado"
else
  echo >&2 "libxml2-dev no instalado"
  exit 1
fi

for i in "${LIBS[@]}"; do
  echo "Creando objeto: " $i
  gcc ${FLAGS[@]} -c src/$i.c -o bin/$i.o
done

for i in "${TARGET[@]}"; do
  echo "Creando ejecutable: " $i
  gcc ${FLAGS[@]} -o bin/$i src/$i.c bin/*.o
done
