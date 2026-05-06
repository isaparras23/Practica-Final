# 📘 Proyecto: Comparación DialSort vs StdSort  
**Curso:** ST0245 - Estructuras de Datos y Algoritmos  
**Universidad:** EAFIT  
**Autores:** Isabel Parra y Mariana Montoya  
**Fecha:**  Mayo 2026  

---

## 🎯 Objetivo
Este proyecto consiste en **diseñar, implementar y analizar experimentalmente** un benchmark que compare el algoritmo **DialSort** con el algoritmo estándar de C++ (**StdSort**, basado en Introsort).  

El trabajo incluye:  
- Implementación de ambos algoritmos de ordenamiento.
  
- Creación de un conjunto de pruebas variando tamaño de entrada, universo y distribución de datos.
  
- Medición de tiempos de ejecución, desviación estándar, throughput y memoria.
  
- Visualización paso a paso del comportamiento interno de cada algoritmo.
  
- Presentación de resultados en un informe técnico y defensa en clase.  

---

## ⚙️ Compilación y ejecución
### Usando CLion
1. Abrir el proyecto en CLion.  
2. Compilar con el botón **Run**.  
3. Ejecutar el programa desde el menú interactivo.

--- 

## 🖥️ Funcionalidades principales
El menú principal (main.cpp) permite:

- Visualizar DialSort con un arreglo pequeño paso a paso.

- Visualizar StdSort mostrando el estado inicial y final.
  
- Comparación rápida con parámetros personalizados (n y U).
  
- Benchmark completo que genera resultados en CSV.
  
- Verificar correctitud comparando ambos algoritmos contra std::sort.

---

## 🔍 Complejidad teórica
# DialSort:
- Tiempo: O(n + U)
- Espacio: O(n + U)
- Ventaja: muy rápido si U es pequeño.
- Desventaja: consume mucha memoria si U es grande.

---

# StdSort (std::sort):
- Tiempo: O(n log n) promedio.
- Espacio: O(n) (vector).
- Ventaja: eficiente en general, independiente de U.
- Desventaja: puede ser más lento que DialSort en universos pequeños.

---

## 👩‍💻 Autores

Isabel Parra

Mariana Montoya

---

## 📚 Uso de IA
Durante el desarrollo del proyecto se utilizó inteligencia artificial como apoyo educativo.
La IA ayudó en:

- Comprender conceptos complejos.
- Optimizar partes del código.
- Mejorar la organización y claridad del proyecto.
  

El uso de IA fue únicamente con fines educativos y de aprendizaje, no para reemplazar nuestro trabajo. Todo el código fue revisado, entendido y adaptado por nosotras.


