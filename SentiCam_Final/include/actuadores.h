#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"

class GestorActuadores {
public:
    bool begin() {
        servoCamara.setPeriodHertz(50);
        servoSensor.setPeriodHertz(50);
        int c = servoCamara.attach(PIN_SERVO_CAMARA, SERVO_MIN_US, SERVO_MAX_US);
        int s = servoSensor.attach(PIN_SERVO_SENSOR, SERVO_MIN_US, SERVO_MAX_US);
        centrar();
        return c > 0 && s > 0;
    }

    void centrar() {
        servoCamara.write(ANGULO_CENTRO);
        servoSensor.write(ANGULO_CENTRO);
    }

    void ajustar(float inclinacionX, float inclinacionY) {
        int camara = constrain(ANGULO_CENTRO + int(inclinacionY * 2.0f), ANGULO_MIN, ANGULO_MAX);
        int sensor = constrain(ANGULO_CENTRO + int(inclinacionX * 2.0f), ANGULO_MIN, ANGULO_MAX);
        servoCamara.write(camara);
        servoSensor.write(sensor);
    }

    void modoSeguro() { centrar(); }

private:
    Servo servoCamara;
    Servo servoSensor;
};
