#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class GestorComunicaciones {
public:
    bool conectarWiFi(uint32_t timeout_ms = 10000) {
        if (String(WIFI_SSID) == "CONFIGURAR_SSID") {
            Serial.println("[AVISO] WiFi sin credenciales configuradas");
            return false;
        }

        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        uint32_t inicio = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - inicio < timeout_ms) {
            delay(250);
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("[OK] WiFi conectado. IP: ");
            Serial.println(WiFi.localIP());
            return true;
        }
        return false;
    }

    bool wifiDisponible() const { return WiFi.status() == WL_CONNECTED; }

    void enviarEstado(uint16_t distancia_mm, bool obstaculo) {
        Serial.printf("[APP] distancia=%u mm, obstaculo=%s\n",
                      distancia_mm, obstaculo ? "SI" : "NO");
        // TODO: reemplazar por protocolo definitivo con la aplicación móvil.
    }

    void enviarAlerta(const char* mensaje) {
        Serial.print("[ALERTA] ");
        Serial.println(mensaje);
        // TODO: integrar alerta de voz mediante la aplicación móvil.
    }
};
