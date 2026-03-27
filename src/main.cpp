#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo miServo;

const int pinServo = 19;
const int umbralDistancia = 150; // 15 cm

void setup() {
  // Ajustado a 9600 como pediste
  Serial.begin(9600); 
  
  // Configuración explícita de pines I2C para ESP32
  // SDA = 21, SCL = 22
  Wire.begin(21, 22);

  // Configuración del Servo
  ESP32PWM::allocateTimer(0);
  miServo.setPeriodHertz(50);
  miServo.attach(pinServo, 500, 2400);

  Serial.println("Iniciando sistema...");

  // Iniciamos el sensor pasando la dirección I2C y el bus Wire
  if (!lox.begin(0x29, false, &Wire)) {
    Serial.println(F("Error: No se encuentra el sensor TOF050C"));
    while (1);
  }
  
  Serial.println("Sensor listo. Esperando objetos...");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); 

  if (measure.RangeStatus != 4) {  
    int distancia = measure.RangeMilliMeter;
    
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" mm");

    if (distancia < umbralDistancia) {
      Serial.println("Objeto CERCA -> Girando a 180");
      miServo.write(180);
    } else {
      Serial.println("Objeto LEJOS -> Girando a 0");
      miServo.write(0);
    }
  } else {
    Serial.println("Fuera de rango");
  }

  delay(100); 
}