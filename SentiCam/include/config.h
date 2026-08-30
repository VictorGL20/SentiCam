#pragma once
#include <Arduino.h>

constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t TIEMPO_MONITOREO_MS = 150;
constexpr uint16_t UMBRAL_DISTANCIA_MM = 500;

// Verificar GPIO reales antes de conectar el hardware.
constexpr int PIN_SERVO_CAMARA = 5;
constexpr int PIN_SERVO_SENSOR = 6;
constexpr int SERVO_MIN_US = 500;
constexpr int SERVO_MAX_US = 2500;
constexpr int ANGULO_CENTRO = 90;
constexpr int ANGULO_MIN = 25;
constexpr int ANGULO_MAX = 155;

constexpr char WIFI_SSID[] = "CONFIGURAR_SSID";
constexpr char WIFI_PASSWORD[] = "CONFIGURAR_PASSWORD";

constexpr uint8_t MAX_INTENTOS_RECUPERACION = 3;
constexpr uint32_t RETARDO_RECUPERACION_MS = 1000;
