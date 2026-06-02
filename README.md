# Particle Swarm Optimization (PSO) from Scratch in C

Este repositorio contiene una implementación nativa y desde cero del algoritmo metaheurístico de **Optimización por Enjambre de Partículas (PSO)** utilizando el lenguaje de programación C. El proyecto está diseñado para resolver problemas de optimización continua multidimensional y cuenta con persistencia de datos mediante el guardado de historiales en formato binario.

## 🚀 Características

- **Implementación limpia de PSO**: Sin librerías externas complejas, utilizando estructuras de datos dinámicas para gestionar las partículas.
- **Funciones de Evaluación Matemáticas (Benchmarks)**:
  - **Función Esfera (Sphere)**: Problema convexo y continuo.
  - **Función Rastrigin**: Problema altamente multimodal con numerosos mínimos locales.
  - **Función Rosenbrock**: Problema no convexo (valle de Rosenbrock).
- **Persistencia en Binario**: Guarda automáticamente el estado del mejor fitness y la mejor posición global de cada iteración en un archivo `historial.bin`.
- **Lector de Historial**: Incluye un módulo integrado para leer, decodificar y mostrar en consola los datos guardados en el archivo binario.

## 🧠 ¿Cómo funciona el algoritmo?

El algoritmo simula el comportamiento social de una bandada de aves. Cada partícula ajusta su trayectoria dinámicamente basándose en:
1. Su propia inercia ($w = 0.8$).
2. Su mejor experiencia previa individual ($c_1 = 0.7$).
3. La mejor experiencia compartida por todo el enjambre ($c_2 = 0.3$).

La velocidad se actualiza en cada dimensión utilizando la ecuación:
$$v_{ij}^{t+1} = w \cdot v_{ij}^t + c_1 \cdot r_1 \cdot (pbest_{ij} - x_{ij}^t) + c_2 \cdot r_2 \cdot (gbest_j - x_{ij}^t)$$

## 🛠️ Estructura del Código

El archivo principal gestiona dos estructuras críticas:
- `Particula`: Almacena vectores dinámicos de posición, velocidad, mejor posición histórica local y valores de fitness asociados.
- `HistorialdeRegistroB`: Define la cabecera binaria estructurada que guarda el número de iteración y el mejor fitness global alcanzado hasta ese momento.

## 💻 Compilación y Ejecución

Para compilar y ejecutar este proyecto necesitas tener instalado un compilador de C (como `gcc`).

### Compilar:
Abre la terminal en la carpeta del proyecto y ejecuta:
```bash
gcc main.c -o pso -lm
