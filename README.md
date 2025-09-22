# SMART BOOK READER

## Descripción

SMART BOOK READER es una aplicación que permite buscar frases o palabras clave en una biblioteca de libros clásicos en formato texto, mostrando fragmentos relevantes y generando imágenes asociadas mediante la API de DallE. El sistema utiliza procesamiento de lenguaje natural para extraer palabras clave, indexa los libros usando árboles B y hash tables, y expone una API web para consultas desde una interfaz web amigable.

---

## Estructura del Proyecto

```
├── .gitignore
├── Makefile
├── dockerfile
├── main.cpp
├── main2.cpp
├── mainDallE.cpp
├── SAVESERVER
├── server.cpp
├── httplib.h
├── bookIndex.h
├── default/
│   ├── book.h
│   ├── BTree.h
│   ├── BTreeNode.h
│   ├── clave.h
│   ├── fragment.h
│   └── paragraph.h
├── lib/
│   ├── DallEapi.cpp
│   ├── GPT.h
│   ├── GPTapi.cpp
│   ├── json.hpp
│   ├── Makefile
│   └── openai.hpp
├── Libros/
│   └── [Libros en texto plano]
├── page/
│   ├── index.html
│   ├── style.css
│   └── script.js
└── .vscode/
    └── [configuración IDE]
```

---

## Componentes Principales

### 1. **Backend (C++)**

- **Indexación de Libros:**  
  Los libros se procesan y se indexan usando árboles B ([default/BTree.h](default/BTree.h)), nodos ([default/BTreeNode.h](default/BTreeNode.h)), y claves ([default/clave.h](default/clave.h)).  
  El archivo [`bookIndex.h`](bookIndex.h) contiene funciones para tokenizar, lematizar y buscar palabras clave en los textos.

- **Procesamiento de Frases:**  
  Cuando el usuario ingresa una frase, el backend utiliza GPT (vía [`lib/GPTapi.cpp`](lib/GPTapi.cpp) y [`lib/GPT.h`](lib/GPT.h)) para extraer palabras clave relevantes, filtra palabras comunes y busca fragmentos en los libros.

- **API REST:**  
  El servidor web está implementado en [`server.cpp`](server.cpp) usando [`httplib.h`](httplib.h). Expone endpoints para recibir frases y devolver resultados en formato JSON.

- **Generación de Imágenes:**  
  Se integra la API de DallE mediante [`lib/DallEapi.cpp`](lib/DallEapi.cpp) para generar imágenes relacionadas con los fragmentos encontrados.

### 2. **Frontend (HTML/JS/CSS)**

- **Interfaz Web:**  
  El archivo [`page/index.html`](page/index.html) contiene el formulario para ingresar frases y muestra los resultados.

- **Estilos:**  
  [`page/style.css`](page/style.css) define la apariencia visual, usando la fuente Montserrat y estilos modernos.

- **Lógica de Cliente:**  
  [`page/script.js`](page/script.js) gestiona el envío de frases al backend, muestra un indicador de carga, y renderiza los fragmentos y sus imágenes en la página.

---

## Instalación y Ejecución

### 1. **Requisitos**

- C++17 o superior
- [libcurl](https://curl.se/libcurl/) (para llamadas HTTP)
- Docker (opcional, para despliegue)
- Navegador web

### 2. **Compilación**

Puedes compilar el backend con el Makefile principal:

```sh
make
```

Esto compila y ejecuta `main.cpp` para pruebas.  
Para el servidor web, compila y ejecuta `server.cpp` (puedes adaptar el Makefile):

```sh
g++ -o server server.cpp -lcurl
./server
```

### 3. **Despliegue con Docker (opcional)**

Incluye un `dockerfile` para facilitar el despliegue en contenedores.  
Ejemplo de comando:

```sh
docker build -t my-server .
docker run -it --rm -p 8080:8080 my-server
```

### 4. **Uso**

1. Inicia el servidor backend en el puerto 8080.
2. Abre [`page/index.html`](page/index.html) en tu navegador.
3. Ingresa una frase en el formulario y presiona "Buscar".
4. Visualiza los fragmentos encontrados, junto con el autor, libro, página, sentimiento y una imagen generada.

---

## Explicación Técnica

- **Procesamiento de Frases:**  
  El backend utiliza GPT para extraer palabras clave, filtra palabras comunes y busca coincidencias en los libros indexados.
- **Indexación:**  
  Los libros se procesan y se indexan en estructuras eficientes para búsquedas rápidas.
- **API REST:**  
  El servidor recibe frases, procesa la consulta y responde con un JSON que contiene los fragmentos relevantes.
- **Frontend:**  
  El cliente web envía la frase, muestra un indicador de carga y renderiza los resultados dinámicamente.

---

## Créditos y Licencias

- Libros clásicos en dominio público.
- Uso de [cpp-httplib](https://github.com/yhirose/cpp-httplib) para el servidor HTTP.
- Integración con OpenAI/DallE para generación de imágenes.

---

## Personalización

- Puedes agregar más libros en la carpeta `Libros/`.
- Modifica el estilo en `page/style.css`.
- Ajusta la lógica de búsqueda en `bookIndex.h` y el procesamiento en `server.cpp`.

---

## Contacto

Para dudas o mejoras, abre un issue en el repositorio o contacta al autor.
