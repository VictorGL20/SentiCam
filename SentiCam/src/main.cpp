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
bool obstaculoDetectado = false;
uint8_t intentosRecuperacion = 0;

float inclinacionX(const DatosSensores& d) {
    return atan2f(d.ay, d.az) * 180.0f / PI;
}

float inclinacionY(const DatosSensores& d) {
    return atan2f(-d.ax, sqrtf(d.ay*d.ay + d.az*d.az)) * 180.0f / PI;
}

bool datosValidos(const DatosSensores& d) {
    return d.vl53_ok && d.mpu_ok;
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("SentiCam - firmware inicial");
}

void loop() {
    switch (estado) {

    case Estado::INICIALIZACION: {
        bool sensoresOK = sensores.begin();
        bool actuadoresOK = actuadores.begin();
        comunicaciones.conectarWiFi();

        estado = (sensoresOK && actuadoresOK)
            ? Estado::MONITOREO
            : Estado::ERROR_RECUPERACION;
        break;
    }

    case Estado::MONITOREO:
        datos = sensores.leer();
        estado = datosValidos(datos)
            ? Estado::PROCESAMIENTO
            : Estado::ERROR_RECUPERACION;
        break;

    case Estado::PROCESAMIENTO:
        obstaculoDetectado =
            datos.distancia_mm > 0 &&
            datos.distancia_mm <= UMBRAL_DISTANCIA_MM;

        comunicaciones.enviarEstado(datos.distancia_mm, obstaculoDetectado);

        // TODO: enviar imágenes/datos a la aplicación móvil para IA.
        estado = Estado::DECISION_OBSTACULO;
        break;

    case Estado::DECISION_OBSTACULO:
        estado = obstaculoDetectado
            ? Estado::ALERTA
            : Estado::AJUSTE_ACTUADORES;
        break;

    case Estado::ALERTA:
        comunicaciones.enviarAlerta("Obstaculo detectado");
        estado = Estado::AJUSTE_ACTUADORES;
        break;

    case Estado::AJUSTE_ACTUADORES:
        actuadores.ajustar(inclinacionX(datos), inclinacionY(datos));
        estado = Estado::RETORNO_MONITOREO;
        break;

    case Estado::RETORNO_MONITOREO:
        delay(TIEMPO_MONITOREO_MS);
        estado = Estado::MONITOREO;
        break;

    case Estado::ERROR_RECUPERACION: {
        Serial.println("[ESTADO] ERROR / RECUPERACION");
        actuadores.modoSeguro();

        if (++intentosRecuperacion > MAX_INTENTOS_RECUPERACION) {
            Serial.println("[FALLO SEGURO] Se mantiene posicion segura");
            intentosRecuperacion = 0;
            delay(2000);
        }

        bool recuperado = sensores.begin();

        if (!comunicaciones.wifiDisponible())
            comunicaciones.conectarWiFi(5000);

        if (recuperado) {
            intentosRecuperacion = 0;
            estado = Estado::MONITOREO;
        } else {
            delay(RETARDO_RECUPERACION_MS);
        }
        break;
    }

    }
}
