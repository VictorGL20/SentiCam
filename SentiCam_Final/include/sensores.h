#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <VL53L0X.h>

struct DatosSensores {
    uint16_t distancia_mm = 0;
    float ax = 0, ay = 0, az = 0;
    float gx = 0, gy = 0, gz = 0;
    bool valido = false;
};

class GestorSensores {
public:
    bool begin() {
        Wire.begin();
        mpuDisponible = mpu.begin();
        if (mpuDisponible) {
            mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
            mpu.setGyroRange(MPU6050_RANGE_500_DEG);
            mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
        }

        vl53.setTimeout(100);
        vl53Disponible = vl53.init();
        if (vl53Disponible) vl53.startContinuous(50);

        return mpuDisponible && vl53Disponible;
    }

    DatosSensores leer() {
        DatosSensores d;
        if (!mpuDisponible || !vl53Disponible) return d;

        d.distancia_mm = vl53.readRangeContinuousMillimeters();
        if (vl53.timeoutOccurred()) return d;

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        d.ax = a.acceleration.x; d.ay = a.acceleration.y; d.az = a.acceleration.z;
        d.gx = g.gyro.x; d.gy = g.gyro.y; d.gz = g.gyro.z;
        d.valido = true;
        return d;
    }

private:
    Adafruit_MPU6050 mpu;
    VL53L0X vl53;
    bool mpuDisponible = false;
    bool vl53Disponible = false;
};
