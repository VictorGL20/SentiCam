# SentiCam

**Bastón Inteligente Portátil de Asistencia para Personas con Discapacidad Visual mediante Visión Artificial, Sensores Inteligentes y XIAO ESP32-S3 Sense**

SentiCam integra adquisición de distancia e inclinación, estabilización mediante servomotores y comunicación inalámbrica en una arquitectura embebida orientada a asistencia durante el desplazamiento.

## Arquitectura

- XIAO ESP32-S3 Sense como unidad principal.
- VL53L0X para medición de distancia.
- MPU6050 para aceleración, movimiento e inclinación.
- Dos servomotores MG90S para orientación de cámara y sensor.
- WiFi para comunicación con el sistema móvil.
- Máquina de estados con recuperación ante fallas.

## Software

El firmware está organizado de forma modular:

- `src/main.cpp`: control principal y máquina de estados.
- `include/sensores.h`: adquisición del VL53L0X y MPU6050.
- `include/actuadores.h`: control de los servomotores.
- `include/comunicaciones.h`: conectividad y transmisión de información.
- `include/config.h`: parámetros generales del sistema.

## Máquina de estados

El sistema utiliza los estados:

1. Inicialización.
2. Monitoreo.
3. Procesamiento.
4. Decisión de obstáculo.
5. Alerta.
6. Ajuste de actuadores.
7. Retorno a monitoreo.
8. Error/Recuperación.

Ante una falla de sensores o comunicación, el sistema mantiene los actuadores en una posición segura e intenta recuperar los módulos antes de regresar al monitoreo.
