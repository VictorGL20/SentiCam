#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class GestorComunicaciones {
public:
    bool begin() {
        WiFi.mode(WIFI_AP);
        return WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    }

    bool disponible() const {
        return WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA;
    }

    void enviarEstado(uint16_t distancia, bool obstaculo, float inclinacionX, float inclinacionY) {
        Serial.printf(
            "{\"distancia_mm\":%u,\"obstaculo\":%s,\"inclinacion_x\":%.2f,\"inclinacion_y\":%.2f}\n",
            distancia, obstaculo ? "true" : "false", inclinacionX, inclinacionY
        );
    }

    void enviarAlerta(const char* mensaje) {
        Serial.printf("{\"alerta\":\"%s\"}\n", mensaje);
    }

    bool recuperar() {
        WiFi.softAPdisconnect(true);
        delay(100);
        return begin();
    }
};
