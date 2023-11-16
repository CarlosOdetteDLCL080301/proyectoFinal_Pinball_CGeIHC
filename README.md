# Proyecto de computación grafica: Desarrollar un Pinball

# Manual Técnico de la Aplicación de Mesa de Pinball en OpenGL

## Introducción

El presente manual técnico proporciona información detallada sobre el desarrollo de una aplicación de mesa de pinball utilizando OpenGL. Esta aplicación simula una mesa de pinball funcional con modelos jerárquicos y texturas descargadas de internet.

## Requisitos del Sistema

### Hardware
- Procesador dual-core o superior.
- Tarjeta gráfica compatible con OpenGL.
- Memoria RAM: 4 GB o superior.
- Resolución de pantalla recomendada: 1920x1080.

### Software
- Sistema operativo compatible con OpenGL.
- GLFW versión 3.3 o superior.
- GLEW versión 2.1.0 o superior.
- GLM versión 0.9.9.8 o superior.
- Biblioteca de imágenes STB versión 2.26 o superior.
- Visual Studio Community 2022

## Arquitectura y Diseño

### Estructura del Proyecto
- **src/:** Contiene los archivos fuente del proyecto.
- **include/:** Contiene los archivos de encabezado.
- **models/:** Almacena modelos 3D descargados de internet.
- **textures/:** Contiene las texturas utilizadas en el proyecto.
- **shaders/:** Contiene los archivos de código fuente de los shaders.

### Modelos Jerárquicos
- Se han utilizado modelos jerárquicos para representar la mesa de pinball y los elementos decorativos.

### Texturización
- Las texturas se aplican a los modelos mediante coordenadas de textura.
- Se utilizan imágenes descargadas de internet para texturizar los elementos de la mesa y la bola.

### Iluminación
- Se implementa iluminación en la escena utilizando diferentes tipos de luces: Direccional, Puntual y Focal.
- Se ha creado un sistema de materiales para controlar las propiedades de reflectancia y brillo de los objetos.

## Desarrollo del Programa

### Clases Principales

1. **Window:**
   - Gestiona la creación y gestión de la ventana de la aplicación.

2. **Mesh:**
   - Representa un modelo 3D y su renderizado.

3. **Shader_light:**
   - Maneja los shaders utilizados en la iluminación.

4. **Camera:**
   - Controla la posición y orientación de la cámara en la escena.

5. **Texture:**
   - Carga y gestiona texturas.

6. **Sphere:**
   - Representa la bola de pinball.

7. **Model:**
   - Implementa la carga y renderizado de modelos 3D.

8. **Skybox:**
   - Crea un cielo envolvente para la escena.

### Bibliotecas Externas

1. **STB Image:**
   - Utilizada para cargar imágenes y texturas.

2. **GLFW:**
   - Maneja la creación de ventanas y la gestión de eventos.

3. **GLEW:**
   - Proporciona acceso a las extensiones OpenGL.

4. **GLM:**
   - Matemáticas de OpenGL, utilizada para operaciones de vectores y matrices.

### Iluminación

1. **DirectionalLight:**
   - Representa una fuente de luz direccional.

2. **PointLight:**
   - Modela una fuente de luz puntual.

3. **SpotLight:**
   - Simula una luz focalizada.

4. **Material:**
   - Define las propiedades de los materiales.

## Instalación y Compilación

1. Clone el repositorio desde [enlace al repositorio].
2. Utilice CMake para generar los archivos de construcción.
3. Compile el proyecto utilizando su sistema de construcción preferido.

## Operación y Uso

1. Ejecute la aplicación resultante después de la compilación.
2. Utilice las teclas de dirección para controlar los flippers y lanzar la bola.
3. Experimente con la interacción de la bola y los elementos decorativos de la mesa.

## Mantenimiento y Solución de Problemas

### Mantenimiento Regular
- Actualice las bibliotecas externas según sea necesario.
- Añada nuevos modelos y texturas siguiendo la estructura del proyecto.

### Solución de Problemas Comunes
- Verifique que las bibliotecas externas estén correctamente vinculadas.
- Inspeccione los registros de la aplicación para identificar errores de ejecución.

## Referencia Técnica

- Consulte la documentación de GLFW, GLEW, GLM y STB Image para obtener información detallada sobre sus funciones y uso.
- Revise los archivos de código fuente para entender la implementación específica de cada clase y función.

---

Este manual técnico proporciona una visión general del desarrollo de la aplicación de mesa de pinball en OpenGL. Si tienes alguna pregunta o necesitas más detalles sobre alguna parte específica, no dudes en preguntar. ¡Espero que disfrutes explorando la mesa de pinball que has creado!

## Recurso utilizados de internet
- [Paul Briot]. (2019, 14 Febrero). Beppi The Clown. Sketch. https://skfb.ly/6UYML

    ![Beppi The Clown](https://media.sketchfab.com/models/614dfc63e5624a2583ed2522b615375f/thumbnails/f6ea183e2d2542d1ab66dc459da59b3e/blob.jpeg)
