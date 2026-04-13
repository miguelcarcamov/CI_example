# CI example (C++)

Ejemplo mínimo en **C++17** con **CMake** y **GoogleTest**: clases `Vehicle` / `Car` / `Fleet`, pruebas unitarias e integración, y **CI en GitHub Actions** usando una imagen publicada en **GitHub Container Registry (GHCR)**.

## Estructura del repositorio

| Ruta | Descripción |
|------|-------------|
| `include/` | Cabeceras públicas (`vehicle.hpp`, `car.hpp`, `fleet.hpp`) |
| `src/` | Implementaciones (`.cpp`) |
| `tests/` | `unit_car_test.cpp`, `integration_fleet_test.cpp` |
| `CMakeLists.txt` | Biblioteca `ci_example`, FetchContent de GoogleTest, CTest |
| `Dockerfile` | Imagen base Ubuntu con herramientas de compilación (solo toolchain) |
| `environment.yml` | Entorno **conda** opcional para desarrollo local |
| `.github/workflows/` | Flujos `build-base-container` y `CI` |

## Clases (dominio)

Todas viven en el espacio de nombres `ci_example`.

### `Vehicle` (abstracta)

- No se instancia sola: define el contrato polimórfico.
- Métodos virtuales puros: `describe()` y `type_name()`.
- Cada vehículo tiene un **`id()`** numérico asignado al construirse.
- Estado **estático** (compartido por la clase): contador de instancias vivas **`instance_count()`**, útil en pruebas de integración.
- No se permite copiar ni mover (eliminados explícitamente).

### `Car` (concreta, hereda de `Vehicle`)

- Representa un coche con **marca**, **modelo** y **número de puertas**.
- Implementa `describe()` (texto legible) y `type_name()` (por ejemplo `"Car"`).
- Getters: `make()`, `model()`, `doors()`.

### `Fleet`

- Mantiene un **`std::vector<Vehicle*>`**; la flota **es dueña** de los punteros (`delete` en `clear()` y en el destructor).
- `add(Vehicle*)` ignora `nullptr`.
- `describe_all()` concatena las descripciones en orden, separadas por `"; "`.
- Los tests de integración comprueban el polimorfismo y que **`Vehicle::instance_count()`** encaje al añadir o vaciar la flota.

## Compilar y probar en local

Requisitos: **CMake ≥ 3.16**, **compilador C++17**, **Git** (para FetchContent de GoogleTest).

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Con **conda** (opcional):

```bash
conda env create -f environment.yml
conda activate ci-example-cpp
# mismos comandos cmake / ctest
```

Tras configurar, CMake puede generar `build/compile_commands.json`; en la raíz del repo puede crearse un enlace simbólico útil para el IDE (según tu flujo de trabajo).

## Docker (imagen base local)

La imagen solo instala el toolchain; el código se monta o copia al usarla:

```bash
docker build -t ci-cpp-base -f Dockerfile .
docker run --rm -v "$PWD:/workspace" -w /workspace ci-cpp-base bash -lc \
  'cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON && cmake --build build --parallel && ctest --test-dir build --output-on-failure'
```

## CI en GitHub Actions

Hay **dos workflows** complementarios.

### 1. `build-base-container`

- **Cuándo:** cambios en **`Dockerfile`** en ramas `main` o `master` (push o pull request).
- **Qué hace:** construye la imagen con **Docker Buildx**, la publica en GHCR como **`ghcr.io/<dueño>/<repo-en-minúsculas>:base`** (en push a la rama por defecto; en PR no hace push) y ejecuta una **compilación de humo** del proyecto dentro de un contenedor (**sin** ejecutar `ctest`).

### 2. `CI`

- **Cuándo:** cada push o PR hacia `main` / `master` (todo el código).
- **Dependencias entre jobs:**
  1. **`ensure-base-image`** — Resuelve el nombre de la imagen, inicia sesión en GHCR e intenta **`docker pull`** de la etiqueta `:base`. Si la imagen no existe o no hay permiso, el job falla con mensajes explícitos y sugerencias (por ejemplo ejecutar antes el workflow que publica el `Dockerfile`).
  2. **`build-and-test`** — `needs: ensure-base-image`. Corre en un **job con `container:`** usando esa imagen: checkout, CMake, compilación y **`ctest`**.

**Primera vez en un repo nuevo:** hace falta que **`build-base-container`** haya publicado al menos una vez la etiqueta **`:base`** (por ejemplo mergeando un cambio que toque el `Dockerfile`). Si **CI** falla antes, vuelve a lanzarlo cuando el paquete ya exista en GHCR.

En Actions, el permiso típico es **`GITHUB_TOKEN`** con `packages: read` / `packages: write` según el workflow; no necesitas un PAT en el repositorio para ese caso.

## GitHub Container Registry (GHCR) — token y uso local

GHCR (`ghcr.io`) almacena las imágenes asociadas al repositorio (suelen listarse en **Packages**).

### ¿Cuándo hace falta un token personal?

- **Dentro de GitHub Actions** en el mismo repositorio suele bastar **`GITHUB_TOKEN`** (configurado con `permissions` en el YAML).
- **En tu máquina** (o en otro CI externo), para `docker pull` / `docker push` contra `ghcr.io` debes hacer **`docker login`** con un **Personal Access Token (PAT)** (u otro método que GitHub documente).

### Crear un PAT

1. GitHub → **Settings** → **Developer settings** → **Personal access tokens**.
2. **Fine-grained:** elige el repositorio u organización y concede permisos de **Packages** (lectura y/o escritura).
3. **Tokens (classic):** como mínimo **`read:packages`** para solo descargar; añade **`write:packages`** si vas a subir imágenes. Si el paquete está ligado a repos privados, GitHub puede pedir también el ámbito **`repo`**.

Guía oficial: [Working with the Container registry](https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-container-registry).

### Iniciar sesión y descargar la imagen

```bash
echo TU_TOKEN | docker login ghcr.io -u TU_USUARIO_GITHUB --password-stdin
docker pull ghcr.io/dueño/repo-en-minusculas:base
```

La ruta exacta y las etiquetas aparecen en la página del paquete en GitHub. Los nombres de imagen en GHCR suelen ir en **minúsculas**.

### Si `docker pull` falla

- Comprueba **visibilidad** del paquete (público/privado) y que el token tenga **`read:packages`**.
- Verifica que la etiqueta (**`:base`**, `latest`, etc.) exista.
- No subas el PAT al repositorio; usa secretos o variables locales.

## Licencia

Si añades una licencia al proyecto, descríbela aquí.
