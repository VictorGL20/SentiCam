#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"

class GestorActuadores {
public:
    bool begin() {
        servoCamara.setPeriodHertz(50);
        servoSensor.setPeriodHertz(50);
        servoCamara.attach(PIN_SERVO_CAMARA, SERVO_MIN_US, SERVO_MAX_US);
        servoSensor.attach(PIN_SERVO_SENSOR, SERVO_MIN_US, SERVO_MAX_US);
        centrar();
        return true;
    }

    void centrar() {
        servoCamara.write(ANGULO_CENTRO);
        servoSensor.write(ANGULO_CENTRO);
    }

    void ajustar(float inclinacionX, float inclinacionY) {
        int angCam = ANGULO_CENTRO + int(inclinacionY * 2.0f);
        int angSensor = ANGULO_CENTRO + int(inclinacionX * 2.0f);
        servoCamara.write(constrain(angCam, ANGULO_MIN, ANGULO_MAX));
        servoSensor.write(constrain(angSensor, ANGULO_MIN, ANGULO_MAX));
    }

    void modoSeguro() { centrar(); }

private:
    Servo servoCamara;
    Servo servoSensor;
};
