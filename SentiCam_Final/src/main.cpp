#include <Arduino.h>
#include <math.h>
#include "config.h"
#include "sensores.h"
#include "actuadores.h"
#include "comunicaciones.h"

enum class Estado {
    INICIALIZACION,
    MONITOREO,
    PROCESAMIENTO,
    DECISION_OBSTACULO,
    ALERTA,
    AJUSTE_ACTUADORES,
    RETORNO_MONITOREO,
    ERROR_RECUPERACION
};

GestorSensores sensores;
GestorActuadores actuadores;
GestorComunicaciones comunicaciones;

Estado estado = Estado::INICIALIZACION;
DatosSensores datos;
bool obstaculo = false;
float incX = 0.0f;
float incY = 0.0f;
uint8_t intentos = 0;

float calcularX(const DatosSensores& d) {
    return atan2f(d.ay, d.az) * 180.0f / PI;
}

float calcularY(const DatosSensores& d) {
    return atan2f(-d.ax, sqrtf(d.ay*d.ay + d.az*d.az)) * 180.0f / PI;
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("SentiCam iniciado");
}

void loop() {
    switch (estado) {
        case Estado::INICIALIZACION: {
            bool s = sensores.begin();
            bool a = actuadores.begin();
            bool c = comunicaciones.begin();
            estado = (s && a && c) ? Estado::MONITOREO : Estado::ERROR_RECUPERACION;
            break;
        }

        case Estado::MONITOREO:
            datos = sensores.leer();
            estado = datos.valido ? Estado::PROCESAMIENTO : Estado::ERROR_RECUPERACION;
            break;

        case Estado::PROCESAMIENTO:
            incX = calcularX(datos);
            incY = calcularY(datos);
            obstaculo = datos.distancia_mm > 0 && datos.distancia_mm <= UMBRAL_DISTANCIA_MM;
            comunicaciones.enviarEstado(datos.distancia_mm, obstaculo, incX, incY);
            estado = Estado::DECISION_OBSTACULO;
            break;

        case Estado::DECISION_OBSTACULO:
            estado = obstaculo ? Estado::ALERTA : Estado::AJUSTE_ACTUADORES;
            break;

        case Estado::ALERTA:
            comunicaciones.enviarAlerta("Obstaculo detectado");
            estado = Estado::AJUSTE_ACTUADORES;
            break;

        case Estado::AJUSTE_ACTUADORES:
            actuadores.ajustar(incX, incY);
            estado = Estado::RETORNO_MONITOREO;
            break;

        case Estado::RETORNO_MONITOREO:
            delay(TIEMPO_MONITOREO_MS);
            estado = Estado::MONITOREO;
            break;

        case Estado::ERROR_RECUPERACION: {
            actuadores.modoSeguro();
            bool sensoresOK = sensores.begin();
            bool comunicacionOK = comunicaciones.disponible() || comunicaciones.recuperar();

            if (sensoresOK && comunicacionOK) {
                intentos = 0;
                estado = Estado::MONITOREO;
            } else {
                intentos++;
                if (intentos >= MAX_INTENTOS_RECUPERACION) {
                    intentos = 0;
                    actuadores.modoSeguro();
                }
                delay(RETARDO_RECUPERACION_MS);
            }
            break;
        }
    }
}
