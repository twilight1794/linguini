set -euo pipefail
IFS=$'\n\t'

function getconfig(){
  grep $1 config.xml | sed -e 's/.*">//;s/<\/string>//'
}


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
  exit
fi

if [[ ${1:-v} == prepare ]]; then
  echo "Preparando entorno"
  apt install libxml2-dev pandoc doxygen
fi

if [[ ${1:-v} == clean ]]; then
  echo "Limpiando…"
  rm -rvf *.exe ${TARGET[@]} *~ *.o *.html
  rm -rvf docs/*
  rm -rvf build/*
  rm -rvf src/xml/*.h
  rm -rvf config.h
  exit
fi

if [[ ${1:-v} == docs ]]; then
  echo "Documentando: Generando md de inicio…"
  command -v pandoc >/dev/null 2>&1
  if [[ $? == 0 ]]; then
    pandoc linguini.org -o linguini.md
    doxygen Doxyfile
  else
    echo >&2 "Pandoc no está instalado, omitiendo inicio…"
    sed -e 's/linguini.md//" Doxyfile | doxygen -
  fi
  exit
fi


# Compilación
#command -v xpath >/dev/null 2>&1
#if [[ $? == 0 ]]; then
#  echo "libxml-xpath-perl instalado"
#else
#  echo >&2 "libxml-xpath-perl no está instalado"
#  exit 1
#fi

command -v xpath >/dev/null 2>&1
if [[ $? == 0 ]]; then
  echo "libxml2-dev instalado"
else
  echo >&2 "libxml2-dev no instalado"
  exit 1
fi

# Procesando configuración
echo "Procesando: config.xml"
echo -e "#ifndef CONFIG\n#define CONFIG\n" > src/config.h
grep "LING_PC_" config.xml | sed -e 's/.*<string name="/#define /;s/">/="/;s/<\/string>/"/;s/&lt;/</g;s/&amp;/\&/g' >> src/config.h
grep "LING_OP_" config.xml | sed -e 's/.*<string name="/#define /;s/">/="/;s/<\/string>/"/;s/&lt;/</g;s/&amp;/\&/g' >> src/config.h
grep "LING_VAL_" config.xml | sed -e 's/.*<string name="/#define /;s/">/="/;s/<\/string>/"/;s/&lt;/</g;s/&amp;/\&/g' >> src/config.h
echo "#if defined(__WIN32__) || defined(__CYGWIN__)" >> src/config.h
grep "_WIN" config.xml | sed -e 's/.*<string name="/#define /;s/_WIN">/="/;s/<\/string>/"/;s/&lt;/</g;s/&amp;/\&/g' >> src/config.h
echo "#elif defined(__unix__)" >> src/config.h
grep "_UNIX" config.xml | sed -e 's/.*<string name="/#define /;s/_UNIX">/="/;s/<\/string>/"/;s/&lt;/</g;s/&amp;/\&/g' >> src/config.h
echo -e "#endif\n#endif" >> src/config.h

# Generando código C desde los XMLs
cd src/xml
for i in *.xml; do
  echo "Procesando: ${i%.*}"
  echo -e "#ifndef I18N_${i%.*}\n#define I18N_${i%.*}\n" > "i18n_${i%.*}.h"
  grep "<l:gentext" $i | sed -e 's/.*<l:gentext key="/#define MSG_/;s/" text//;s/ \/>//;s/&lt;/</g' >> "i18n_${i%.*}.h"
  echo "#endif" >> "i18n_${i%.*}.h"
done
cd ../..


# Generando archivos objeto
for i in "${LIBS[@]}"; do
  echo "Creando objeto: " $i
  gcc ${FLAGS[@]} -c src/$i.c -o bin/$i.o
done

## Generando ejecutables
for i in "${TARGET[@]}"; do
  echo "Creando ejecutable: " $i
  gcc ${FLAGS[@]} -o bin/$i src/$i.c bin/*.o
done
