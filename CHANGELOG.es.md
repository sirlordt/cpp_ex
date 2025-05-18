# Registro de Cambios

## 2025-05-18 09:12 (America/Los_Angeles, UTC-7:00)

### Reorganización de la Estructura del Código

#### Estructura del Proyecto
* Movido main.cpp del directorio raíz al directorio src/
* Sobreescrito el archivo src/main.cpp existente con la versión más completa
* Eliminado el archivo main.cpp de la raíz del proyecto
* Actualizado CMakeLists.txt para usar src/main.cpp en lugar de main.cpp

#### CMakeLists.txt
* Cambiada la línea add_executable para usar src/main.cpp en lugar de main.cpp
* Verificado que todo compila y se ejecuta correctamente

### Detección Mejorada de Dependencias del Contenedor

#### build.dist.sh
* Implementada detección automática de bibliotecas compartidas (.so) usando ldd
* Añadido mapeo de bibliotecas conocidas a sus paquetes correspondientes
* Creada función para añadir paquetes y rastrear sus versiones
* Añadida lógica para determinar qué paquetes Debian proporcionan las bibliotecas detectadas
* Asegurado que el Dockerfile incluya todos los paquetes necesarios
* Añadido manejo especial para bibliotecas como libasan.so.6, libstdc++.so.6 y libubsan.so.1
* Verificado que el contenedor Docker se construye y ejecuta correctamente con todas las dependencias

## 2025-05-16 20:35 (America/Los_Angeles, UTC-7:00)

### Añadida Construcción Opcional de Pruebas

#### CMakeLists.txt
* Añadida opción BUILD_TESTS para controlar la construcción de pruebas
* Modificado enable_testing() para ejecutarse solo cuando BUILD_TESTS está activado
* Condicionada la inclusión del directorio de pruebas a la opción BUILD_TESTS

#### build.sh
* Añadido soporte para el parámetro -test
* Modificado el script para aceptar el parámetro opcional -test
* Añadida configuración condicional de CMake basada en la bandera de prueba
* Actualizados los mensajes de salida para indicar el modo de construcción (con/sin pruebas)
* Simplificado el proceso de construcción utilizando opciones de CMake en lugar de manipulación de archivos

## 2025-05-16 20:23 (America/Los_Angeles, UTC-7:00)

### Añadida Prueba de try_catch_guard en main.cpp

#### main.cpp
* Añadidos bloques _try _catch para probar la funcionalidad de try_catch_guard
* Creada prueba para manejo de excepciones de puntero nulo
* Creada prueba para manejo de excepciones estándar
* Verificada la integración exitosa con la biblioteca try_catch_guard

#### CMakeLists.txt
* Añadido directorio de inclusión para try_catch_guard al ejecutable principal
* Simplificada la ruta de inclusión en main.cpp

## 2025-05-16 20:20 (America/Los_Angeles, UTC-7:00)

### Corregida la Integración de try_catch_guard

#### CMakeLists.txt
* Corregida la integración con try_catch_guard como biblioteca de solo encabezados
* Creado un objetivo de biblioteca INTERFACE para try_catch_guard
* Actualizados los directorios de inclusión para apuntar a la ubicación correcta

#### tests/CMakeLists.txt
* Actualizado para usar la biblioteca de interfaz try_catch_guard
* Añadido objetivo personalizado para construir y ejecutar pruebas de try_catch_guard por separado
* Mejorada la integración de pruebas con el proyecto principal
* Corregida la integración de CTest utilizando ejecución directa de pruebas en lugar de descubrimiento de pruebas
* Eliminado el descubrimiento automático de pruebas que causaba fallos de segmentación con el manejo de señales

#### run_test.sh y build_test.sh
* Eliminado el manejo manual de pruebas de try_catch_guard
* Las pruebas ahora son manejadas por el objetivo personalizado de CMake
* Actualizada la ejecución de CTest para ejecutar solo pruebas directas y evitar fallos de segmentación

## 2025-05-16 20:06 (America/Los_Angeles, UTC-7:00)

### Cambio de Nombre de Biblioteca: memory_guard → try_catch_guard

#### build.sh
* Actualizada la URL del repositorio de https://github.com/sirlordt/memory_guard a https://github.com/sirlordt/try_catch_guard
* Renombradas todas las funciones y variables de memory_guard a try_catch_guard
* Actualizadas las rutas de directorios de external/libs/memory_guard a external/libs/try_catch_guard

#### CMakeLists.txt
* Cambiada la ruta de verificación de la biblioteca de external/libs/memory_guard a external/libs/try_catch_guard
* Renombrada la variable MEMORY_GUARD_AVAILABLE a TRY_CATCH_GUARD_AVAILABLE
* Actualizados los enlaces y definiciones de compilación para usar try_catch_guard

#### tests/CMakeLists.txt
* Actualizado el enlace de la biblioteca de memory_guard a try_catch_guard
* Cambiada la definición de compilación de MEMORY_GUARD_AVAILABLE a TRY_CATCH_GUARD_AVAILABLE
* Actualizadas las referencias al objetivo de prueba de memory_guard_tests a try_catch_guard_tests

#### run_test.sh
* Actualizada la ruta de ejecución de pruebas de external/libs/memory_guard/memory_guard_tests a external/libs/try_catch_guard/try_catch_guard_tests
* Cambiados los mensajes de registro para hacer referencia a try_catch_guard en lugar de memory_guard

#### build_test.sh
* Actualizada la URL del repositorio y el nombre de la función de descarga
* Cambiadas las verificaciones de directorio y referencias al objetivo de prueba
* Actualizado el objetivo de compilación de memory_guard_tests a try_catch_guard_tests

#### Documentación
* Actualizados todos los archivos del banco de memoria (activeContext.md, systemPatterns.md, progress.md, techContext.md)
* Cambiada la descripción del propósito de la biblioteca de "gestión de memoria" a "manejo de excepciones"
* Actualizadas todas las referencias al nombre de la biblioteca y la URL del repositorio